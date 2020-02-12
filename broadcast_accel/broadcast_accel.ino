#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>
int accelX = 1, accelY = 1, accelZ = 1;
float x, y, z;

BLEService customService("1101");
BLEUnsignedIntCharacteristic customXChar("2101", BLERead | BLENotify);
BLEUnsignedIntCharacteristic customYChar("2102", BLERead | BLENotify);
BLEUnsignedIntCharacteristic customZChar("2103", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  
  if (!IMU.begin()) {
    Serial.println("Cannot initialize IMU");
    while(1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("XtYtZ");

  pinMode(LED_BUILTIN, OUTPUT);
  
  if (!BLE.begin()) {
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1);
  }

  BLE.setLocalName("Arduino Accelerometer");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(customXChar);
  customService.addCharacteristic(customYChar);
  customService.addCharacteristic(customZChar);
  BLE.addService(customService);
  customXChar.writeValue(accelX);
  customYChar.writeValue(accelY); 
  customZChar.writeValue(accelZ);

  BLE.advertise();
  Serial.println("We gucci, y'all can connect now!");
}

void loop() {
  float x, y, z;

  BLEDevice central = BLE.central();

  if (central) {
    digitalWrite(LED_BUILTIN, HIGH);

    while (central.connected()) {
      delay(200);

      // Read from accelerometer
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(x,y,z);
        accelX = (1 + x) * 100;
        accelY = (1 + y) * 100;
        accelZ = (1 + z) * 100;
      }

      customXChar.writeValue(accelX);
      customYChar.writeValue(accelY);
      customZChar.writeValue(accelZ);
    }
  }

  digitalWrite(LED_BUILTIN, LOW);
}
