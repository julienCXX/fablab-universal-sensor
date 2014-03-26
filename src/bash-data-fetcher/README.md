bash-data-fetcher
=============================

A small data fetcher from a SensorTag device.

User guide:

1. Install the **BlueZ** library and **Gnuplot**. Old versions of these programs may
   cause this tool to malfunction.
2. Set the SensorTag in discovery mode, by pressing the side button.
3. Use `hcitool lescan` (in root) to determine the SensorTag's Mac address.
4. Replace the MAC address in `run_data_fetch.sh` with the value you got from (3). You can change the fetch frequency if you want.
5. Run `run_data_fetch.sh`.
6. Stop the script's execution with **^C** when you have enough data.
7. Run `plot.sh`.
8. Open `plot.pdf`. Enjoy the result.

Note: an example of obtained data can be found in the `examples` directory.
