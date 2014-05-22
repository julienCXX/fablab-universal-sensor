var util = require('util');

var async = require('async');

var SensorTag = require('./index');

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
            console.log('enableIrTemperature');
            sensorTag.enableIrTemperature(callback);
        },
        function(callback) {
            setTimeout(callback, 2000);
        },
        function(callback) {
            i = 0;
            console.log('readIrTemperature');
            sensorTag.readIrTemperature(function(objectTemperature, ambientTemperature) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '%d', objectTemperature.toFixed(1));
//                console.log(dateString + '%d', ambientTemperature.toFixed(1));
                i++;
                if (false) {
                    //               //console.log("Sortie");
                    callback();
                }
            });

            sensorTag.on('irTemperatureChange', function(objectTemperature, ambientTemperature) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '%d', objectTemperature.toFixed(1));
//                console.log(dateString + '%d', ambientTemperature.toFixed(1));
                i++;
                if (false) {
                    //               //console.log("Sortie");
                    callback();
                }
            });

            sensorTag.notifyIrTemperature(function() {

            });
        },
        function(callback) {
            console.log('disableIrTemperature');
            sensorTag.disableIrTemperature(callback);
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
