data-vizualiser
====================

A data parser and filter for the SensorTag's raw data.
Also prepares it for displaying.

User guide:

1. Run `make`.
2. Run `./filter_run (MIN_VAR) (SHAKE_COUNT_ACTIVE) (SHAKE_COUNT_INACTIVE) < (INPUT_FILE)`.

Options are:
* MIN_VAR = minimum variation between 2 measures to consider it as a
shaking
* SHAKE_COUNT_ACTIVE = number of consecutive shakes required to switch teh
  detected heat state from "inactive" to "active"
* SHAKE_COUNT_INACTIVE = same as previous, but for the opposite state change
* INPUT_FILE = the file containig the raw data

For instance: `./filter_run 0.2 2 4 < ../../measures/accelLowShaking.dat`.
3. Open `output-chart.html`.
4. Enjoy!

