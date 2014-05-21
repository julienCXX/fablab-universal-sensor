nodejs-data-fetcher
=========================

A SensorTag data fetcher, written in node.js.

Installation:
1. Install node.js.
2. Enter `npm install async`, `npm install noble` and `npm install zmq`
   in order to install required dependencies. On GNU/Linux, you may need
   to install `bluez-libs` in order to compile and install the `noble` module.
3. Enter `node <sensor_name>.js`, where <sensor_name> is the sensor from which
   you want the data. On GNU/Linux, running this command as root may be
   required.

