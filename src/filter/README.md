filter
=============================

Data filter for the bash-data-fetcher output.

Usage:
* make
* ./filter_run (threshold) (inertia active) (inertia inactive) < (raw data measure file) > (processed file)
* ./plot.sh
* open the file named "proc_plot.pdf"

Notes: threshold = threshold temperature (°C) determining the heater's state change
when this value is "crossed".

Inertia active: the pipe's thermal inertia (in number of measures) determining
how long the heated was on when it reached the threshold temperature.

Inertia inactive: same as previous, but for when the heater goes off.

Key:
* black line: raw temperature data
* red rectangle(s): time periods where the heater is considered as "on"
* blue line: the filter's threshold (can be changed in plot.conf)

