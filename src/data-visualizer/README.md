data-vizualiser
====================

A data parser and filter for the SensorTag's raw data.
Also prepares it for displaying.

User guide:

* Run `make`.
* Run `./filter_run (MIN_VAR) (SHAKE_COUNT_ACTIVE) (SHAKE_COUNT_INACTIVE) < (INPUT_FILE)`.

Options are:

1. MIN_VAR = minimum variation between 2 measures to consider
it as a shaking

2. SHAKE_COUNT_ACTIVE = number of consecutive shakes required to switch
the  detected heat state from "inactive" to "active"

3. SHAKE_COUNT_INACTIVE = same as previous, but for the opposite state change

4. INPUT_FILE = the file containig the raw data

For instance: `./filter_run 0.2 2 4 < ../../measures/accelLowShaking.dat`.

* Open `output-chart.html` with a Web browser.
* Enjoy!
