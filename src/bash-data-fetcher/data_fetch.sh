#! /bin/sh


GATTTOOL=gatttool
fetch_interval=1

# shows this script's usage
usage() {
    cat << EOF
Usage: $(basename $0) DEV_MAC [FETCH_INTERVAL]
Fetches data from a SensorTag and prints it on the standard output
Arguments:
DEV_MAC        the device's MAC address, can be obtained with "hcitool lescan"
FETCH_INTERVAL the interval (in seconds) between 2 data fetches
               defaults to 5, cannot be lower than 1
EOF
}

# sends a read order to the Bluetooth device, using a specified handle
read_from_dev() {
    $GATTTOOL -b "$device_mac" --char-read -a "$1"
}

# sends a write order to the Bluetooth device,
# using a specified handle and a specified value
write_to_dev() {
    $GATTTOOL -b "$device_mac" --char-write -a "$1" -n "$2"
}

# returns 0 if the argument is an integer or another value otherwise
isInt() {
    test "$1" && printf '%d' "$1" >/dev/null
    echo $?
}

# extracts the group of hex values from gatttool command read
extract_raw_values() {
    echo "$1" | cut -s -d ' ' -f 3-
}

# converts an hexadecimal value to a decimal one
hex_to_dec() {
    upper_hex=$(echo "$1" | tr '[:lower:]' '[:upper:]')
    echo "ibase=16; $upper_hex" | bc
}

# given 4 hex values, converts the group of the 2 1st or 2 last hex values
# to an integer
hex_group_to_int() {
    lsb_id=$(echo "1+2*$2" | bc)
    msb_id=$(echo "2+2*$2" | bc)
    lsb=$(echo "$1" | cut -s -d ' ' -f $lsb_id)
    msb=$(echo "$1" | cut -s -d ' ' -f $msb_id)
    hex_to_dec "$msb$lsb"
}

# converts raw 2-bytes value given as un integer to the ambiant temperature (°C)
# as comimg from the combo ambiant/IR temperature measure
raw_to_ambiant_temp_from_ir_data() {
    echo "scale=2; $1/128.0" | bc -l
}

# converts raw 2-bytes value as integer to the ambiant temperature (in °C)
# as comimg from the combo ambiant temperature/relative humidity measure
raw_to_ambiant_temp_from_humidity_data() {
    echo "scale=2; -46.85+(175.72*$1)/65536" | bc -l
}


# retrieve arguments
if test $# -eq 0; then
    echo "Error: please specify the device's MAC address"
    usage
    exit 1
fi

device_mac="$1"

if test $# -ge 2; then
    res=$(isInt "$2")
    if test "$res" -ne 0; then
        echo "Error: fetch interval is not a decimal value"
        usage
        exit 1
    fi
    fetch_interval="$2"
fi

loop_wait=$(echo "$fetch_interval"-1 | bc)


while true; do
    # enable IR temp sensor
    write_to_dev 0x29 01
    sleep 1
    # read combo ambient temp and IR temp sensor
    raw=$(read_from_dev 0x25)
    xtr=$(extract_raw_values "$raw")
    raw_ambiant=$(hex_group_to_int "$xtr" 1)
    value_ambiant=$(raw_to_ambiant_temp_from_ir_data "$raw_ambiant")
    fetch_date=$(date +%FT%T)
    echo "$fetch_date" "$value_ambiant"
    sleep $loop_wait
done

