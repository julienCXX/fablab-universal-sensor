var util = require('util');

var async = require('async');

require('./file_io');

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
            console.log('enableGyroscope');
            sensorTag.enableGyroscope(callback);
        },
        function(callback) {
            setTimeout(callback, 2000);
        },
        function(callback) {
            console.log('readGyroscope');
            i = 0;
            sensorTag.readGyroscope(function(x, y, z) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString()
                + '.' + leftPad(date.getMilliseconds(), 3) + ' ';
                //console.log(dateString + '%d %d %d', x.toFixed(2), y.toFixed(2), z.toFixed(2));
                appendToCurrFile(dateString + x.toFixed(2) + ' ' +  y.toFixed(2) + ' ' +  z.toFixed(2));
//            console.log(dateString + '\ty = %d G', y.toFixed(1));
//            console.log(dateString + '\tz = %d G \n', z.toFixed(1));
                i++;
                if (i >= 1000)
                    callback();
            });

            sensorTag.on('gyroscopeChange', function(x, y, z) {
                var date = new Date();
                var dateString = date.getFullYear() + '-' + (date.getMonth() + 1) + '-' + date.getDate() + 'T' + date.toLocaleTimeString()
                + '.' + leftPad(date.getMilliseconds(), 3) + ' ';
                //console.log(dateString + '%d %d %d', x.toFixed(2), y.toFixed(2), z.toFixed(2));
                appendToCurrFile(dateString + x.toFixed(2) + ' ' +  y.toFixed(2) + ' ' +  z.toFixed(2));
//            console.log(dateString + '\ty = %d G', y.toFixed(1));
//            console.log(dateString + '\tz = %d G \n', z.toFixed(1));
                i++;
                if (i >= 1000) {
                    callback();
                }
            });

            sensorTag.notifyGyroscope(function() {

            });
        },
        function(callback) {
            console.log('disableGyroscope');
            sensorTag.disableGyroscope(callback);
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
