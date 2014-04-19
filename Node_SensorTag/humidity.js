var SensorTag = require('sensortag');
 
var result = SensorTag.discover(function(sensorTag) {
  console.log('discover');
  sensorTag.connect(function() {
    console.log('connect');
    sensorTag.discoverServices(function() {
      console.log('discoverServices');
      sensorTag.discoverCharacteristics(function() {
        console.log('discoverCharacteristics');
        sensorTag.enableHumidity(function() {
          sensorTag.on('humidityChange', function(temperature, humidity) {
            console.log('\ttemperature = %d °C', temperature.toFixed(1));
            console.log('\thumidity = %d %', humidity.toFixed(1));
          });
 
          sensorTag.notifyHumidity(function() {
            console.log('notifyHumidity');
          });
        });
      });
    });
  });
});
console.log('discover');
// SensorTag(SensorTag.discover);
