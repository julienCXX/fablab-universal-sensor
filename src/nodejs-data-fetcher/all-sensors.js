var util = require('util');

var async = require('async');

require('./file_io');

require('./signal_handler');

var SensorTag = require('./node_sensortag/index');

var filePath = '../../measures/';
var fetchDelay = 1000; // in ms, do not put less than 1000
var intervalHandle;
var cleanDisconnect = false;

var numPrecision = 2;

var measure = {
    date : null,
    objTemp : 0.0,
    ambTemp : 0.0,
    pressure : 0.0,
    humidity : 0.0,
    accel : {
        x : 0.0,
        y : 0.0,
        z : 0.0
    },
    magnet : {
        x : 0.0,
        y : 0.0,
        z : 0.0
    },
    gyro : {
        x : 0.0,
        y : 0.0,
        z : 0.0
    }
};

var setCurDate = function(date) {
    measure.date = date;
};

var setCurObjTemp = function(objTemp) {
    measure.objTemp = objTemp.toFixed(numPrecision);
};

var setCurAmbTemp = function(ambTemp) {
    measure.ambTemp = ambTemp.toFixed(numPrecision);
};

var setCurObjAmbTemp = function(objTemp, ambTemp) {
    setCurObjTemp(objTemp);
    setCurAmbTemp(ambTemp);
};

var setCurAccel = function (x, y, z) {
    measure.accel.x = x.toFixed(numPrecision);
    measure.accel.y = y.toFixed(numPrecision);
    measure.accel.z = z.toFixed(numPrecision);
};

var setCurHumidity = function(humidity) {
    measure.humidity = humidity.toFixed(numPrecision);
};

var setCurMagnet = function (x, y, z) {
    measure.magnet.x = x.toFixed(numPrecision);
    measure.magnet.y = y.toFixed(numPrecision);
    measure.magnet.z = z.toFixed(numPrecision);
};

var setCurPressure = function(pressure) {
    measure.pressure = pressure.toFixed(numPrecision);
};

var setCurGyro = function (x, y, z) {
    measure.gyro.x = x.toFixed(numPrecision);
    measure.gyro.y = y.toFixed(numPrecision);
    measure.gyro.z = z.toFixed(numPrecision);
};

var triplet3dToString = function (element) {
    return element.x + " " + element.y + " " + element.z;
};

var dateToString = function(date) {
    return date.getFullYear() + '-' + (date.getMonth() + 1) + '-'
        + date.getDate() + 'T' + date.getHours()
        + ':' + date.getMinutes() + ':' + date.getSeconds()
        + '.' + leftPad(date.getMilliseconds(), 3);
};

var fieldDesc = '# date irTemp ambientTemp pressure humidity acceleration(x y z) magnetometer(x y z) gyroscope(x y z)';

var measureToLine = function() {
    return dateToString(measure.date) + " " + measure.objTemp + " "
        + measure.ambTemp + " " + measure.pressure + " " + measure.humidity
        + " " + triplet3dToString(measure.accel) + " "
        + triplet3dToString(measure.magnet) + " "
        + triplet3dToString(measure.gyro);
};

var connectAndInitProcedure = function(sensorTag, callback) {
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
            console.log('enableAccelerometer');
            sensorTag.enableAccelerometer(callback);
        },
        function(callback) {
            console.log('enableHumidity');
            sensorTag.enableHumidity(callback);
        },
        function(callback) {
            console.log('enableMagnetometer');
            sensorTag.enableMagnetometer(callback);
        },
        function(callback) {
            console.log('enableBarometricPressure');
            sensorTag.enableBarometricPressure(callback);
        },
        function(callback) {
            console.log('enableGyroscope');
            sensorTag.enableGyroscope(callback);
        },
        function(callback) {
            // initialize sensor-reading methods
            setTimeout(function() {
                sensorReadLoop(sensorTag, false);
            }, 2000);
            setTimeout(callback, 2000);
        }
    ], callback);
}

var sensorReadLoop = function(sensorTag, append) {
    setCurDate(new Date());
    async.series([
        function(callback) {
            sensorTag.readIrTemperature(setCurObjAmbTemp);
            callback();
        },
        function(callback) {
            sensorTag.readAccelerometer(setCurAccel);
            callback();
        },
        function(callback) {
            sensorTag.readHumidity(setCurHumidity);
            callback();
        },
        function(callback) {
            sensorTag.readMagnetometer(setCurMagnet);
            callback();
        },
        function(callback) {
            sensorTag.readBarometricPressure(setCurPressure);
            callback();
        },
        function(callback) {
            sensorTag.readGyroscope(setCurGyro);
            callback();
        },
        function(callback) {
            if (append) {
                appendToCurrFile(measureToLine());
            }
            callback();
        },
    ], function() {});
}

console.log('Use Ctrl-C to stop this program');
console.log('Waiting for SensorTag discovery');
SensorTag.discover(function(sensorTag) {
    async.series([
        function(callback) {
            connectAndInitProcedure(sensorTag, callback);
        },
        function(callback) {
            console.log('read all sensors');
            setAutomaticFilePath(filePath);
            appendToCurrFile(fieldDesc);
            intervalHandle = setInterval(function() {
                sensorReadLoop(sensorTag, true);
            }, fetchDelay);
            // setting interrupt callback
            setSigIntCallback(callback);
            sensorTag.on('disconnect', function() {
                if (cleanDisconnect) {
                    return;
                }
                clearInterval(intervalHandle);
                console.log('lost connection to SensorTag');
                connectAndInitProcedure(sensorTag, function() {
                    changeCurFileNameToNextIndex();
                    appendToCurrFile(fieldDesc);
                    intervalHandle = setInterval(function() {
                        sensorReadLoop(sensorTag);
                    }, fetchDelay);
                });
            });
        },
        function(callback) {
            cleanDisconnect = true;
            clearInterval(intervalHandle);
            console.log('interrupt recevied');
            callback();
        },
        function(callback) {
            console.log('disconnect');
            sensorTag.disconnect(callback);
        }
    ], function() {
        process.exit(0);
    });
});

