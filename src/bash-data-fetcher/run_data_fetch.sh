#! /bin/sh

DEV_MAC="XX:XX:XX:XX:XX:XX" # SensorTag's MAC address
FETCH_FREQUENCY="60" # data fetch frequency, in seconds

./data_fetch.sh "$DEV_MAC" "$FETCH_FREQUENCY" > out.dat

