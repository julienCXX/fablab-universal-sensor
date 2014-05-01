filter
=============================

Data filter for the bash-data-fetcher output.

Usage:
* make
* ./filter_run 16 2 3 < raw.dat > proc.dat
* ./plot.sh
* open the file named "proc_plot.pdf"

Key:
* black line: raw temperature data
* red rectangle(s): time periods where the heater is considered as "on"
* blue line: the filter's threshold (can be changed in plot.conf)

