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
            console.log('enableAccelerometer');
            sensorTag.enableAccelerometer(callback);
        },
        function(callback) {
            setTimeout(callback, 2000);
        },
        function(callback) {
            console.log('readAccelerometer');
            i = 0;
            sensorTag.readAccelerometer(function(x, y, z) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '%d %d %d', x.toFixed(2), y.toFixed(2), z.toFixed(2));
//            console.log(dateString + '\ty = %d G', y.toFixed(1));
//            console.log(dateString + '\tz = %d G \n', z.toFixed(1));
                i++;
                if (i >= 1000)
                    callback();
            });

            sensorTag.on('accelerometerChange', function(x, y, z) {
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

            sensorTag.notifyAccelerometer(function() {

            });
        },
        function(callback) {
            console.log('disableAccelerometer');
            sensorTag.disableAccelerometer(callback);
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
