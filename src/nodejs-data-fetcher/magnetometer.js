var util = require('util');

var async = require('async');

var SensorTag = require('./node_sensortag/index');

SensorTag.discover(function(sensorTag) {
    async.series([
        function(callback) {
            console.log('connect');
            sensorTag.connect(callback);
        },
        function(callback) {
            console.log('discoverServicesAndCharacteristics');
            sensorTag.discoverServicesAndCharacteristics(callback);
        },
        function(callback) {
            console.log('enableMagnetometer');
            sensorTag.enableMagnetometer(callback);
        },
        function(callback) {
            setTimeout(callback, 2000);
        },
        function(callback) {
            console.log('readMagnetometer');
            i = 0;
            sensorTag.readMagnetometer(function(x, y, z) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '%d %d %d', x.toFixed(2), y.toFixed(2), z.toFixed(2));
//            console.log(dateString + '\ty = %d G', y.toFixed(1));
//            console.log(dateString + '\tz = %d G \n', z.toFixed(1));
                i++;
                if (i >= 1000)
                    callback();
            });

            sensorTag.on('magnetometerChange', function(x, y, z) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '%d %d %d', x.toFixed(2), y.toFixed(2), z.toFixed(2));
//            console.log(dateString + '\ty = %d G', y.toFixed(1));
//            console.log(dateString + '\tz = %d G \n', z.toFixed(1));
                i++;
                if (i >= 1000) {
                    callback();
                }
            });

            sensorTag.notifyMagnetometer(function() {

            });
        },
        function(callback) {
            console.log('disableMagnetometer');
            sensorTag.disableMagnetometer(callback);
        },
        function(callback) {
            console.log('disconnect');
            sensorTag.disconnect(callback);
        }
    ],
            function() {
                process.exit(0);
            }
    );
});
