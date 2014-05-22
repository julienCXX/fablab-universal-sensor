#! /bin/sh

DEV_MAC="90:59:af:0:0:b:b:aa" # SensorTag's MAC address
FETCH_FREQUENCY="60" # data fetch frequency, in seconds

./data_fetch.sh "$DEV_MAC" "$FETCH_FREQUENCY" > out.dat

