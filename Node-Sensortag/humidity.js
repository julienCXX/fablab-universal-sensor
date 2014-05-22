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
            console.log('enableHumidity');
            sensorTag.enableHumidity(callback);
        },
        function(callback) {
            console.log('readHumidity');
            i = 0;
            sensorTag.readHumidity(function(temperature, humidity) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '\tTemperature = %d °C', temperature.toFixed(1));
                console.log(dateString + '\tHumidity= %d %', humidity.toFixed(1));
                i++;
                if (i >= 5)
                    callback();
            });

            sensorTag.on('humidityChange', function(temperature, humidity) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString() + ' ';
                console.log(dateString + '\tTemperature = %d °C', temperature.toFixed(1));
                console.log(dateString + '\tHumidity= %d %', humidity.toFixed(1));
                i++;
                if (i >= 5)
                    callback();
            });

            sensorTag.notifyHumidity(function() {

            });
        },
        function(callback) {
            console.log('disableHumidity');
            sensorTag.disableHumidity(callback);
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
