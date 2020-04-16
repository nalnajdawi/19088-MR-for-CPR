#include <ArduinoBLE.h>

// BLE Battery Service
BLEService wearableService("180F");

// BLE Battery Level Characteristic
BLEUnsignedCharCharacteristic sensor1LevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEUnsignedCharCharacteristic sensor2LevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEUnsignedCharCharacteristic sensor3LevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes
BLEUnsignedCharCharacteristic ultrasonicLevelChar("2A19",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // remote clients will be able to get notifications if this characteristic changes

int oldsensor1Level = 0;  // last level reading from analog input
int oldsensor2Level = 0;  // last level reading from analog input
int oldsensor3Level = 0;  // last level reading from analog input
int oldultrasonicLevel = 0;  // last level reading from analog input
long previousMillis = 0;  // last time the level was checked, in ms

int ffs0 = A0; // FlexiForce sensor is connected analog pin A0
int ffs1 = A1; // FlexiForce sensor is connected analog pin A1
int ffs2 = A2; // FlexiForce sensor is connected analog pin A2
//int uss0 = PWM //IDK HOW THE ULTRASONIC SENSOR WORKS THIS IS PLACEHOLDER
int ffsdata[3] = {0, 0, 0};

float vout[4] = {0, 0, 0, 0};
float cf = 19.5; //calibration factor for pressure sensors

void setup() {
  Serial.begin(9600);    // initialize serial communication
  while (!Serial);
  pinMode(ffs0, INPUT);
  pinMode(ffs1, INPUT);
  pinMode(ffs2, INPUT);
  //  pinMode(uss0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("MRCPRWearable");
  BLE.setAdvertisedService(wearableService); // add the service UUID
  wearableService.addCharacteristic(sensor1LevelChar); // add the sensor1 level characteristic
  wearableService.addCharacteristic(sensor2LevelChar); // add the sensor2 level characteristic
  wearableService.addCharacteristic(sensor3LevelChar); // add the sensor3 level characteristic
  wearableService.addCharacteristic(ultrasonicLevelChar); // add the ultrasonic level characteristic

  BLE.addService(wearableService); // Add the wearable service
  sensor1LevelChar.writeValue(oldsensor1Level); // set initial value for this characteristic
  sensor2LevelChar.writeValue(oldsensor2Level); // set initial value for this characteristic
  sensor3LevelChar.writeValue(oldsensor3Level); // set initial value for this characteristic
  ultrasonicLevelChar.writeValue(oldultrasonicLevel); // set initial value for this characteristic

  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active! Now waiting for connection...");
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // check the sensors every 200ms
    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      // if 200ms have passed, check the sensor levels:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateSensorLevel();
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void updateSensorLevel() {
  /* Read the current voltage level on the input pins.
     This is used here to update the sensors and calibrate them.
     The values are then updated if the sensors reguster a change.
  */
  ffsdata[0] = analogRead(ffs0);
  ffsdata[1] = analogRead(ffs1);
  ffsdata[2] = analogRead(ffs2);
  //Q: does the ultrasonic sensor need to be calibrated to register depth correctly?
  //if Yes, need to change the below line so it's not a straight read (see for loop)
  //  vout[3] = analogRead(uss0);

  // This loop uses the calibration factor to change the flexiforce to lbs
  for (int i = 0; i < 2; i++) {
    vout[i] = ((ffsdata[i]) * 100);
    vout[i] = vout[i] * cf;
    //if the calibration skews, check out the map() function for changing
    Serial.print("Flexi Force sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(vout[i], 3);
    Serial.println("");
  }
  //This prints out the sensor values
  Serial.print("Ultrasonic sensor: ");
  Serial.print(vout[3], 3);
  Serial.println("");


  //these loops check if the values have changed within the milli time and change according.

  if (vout[0] != oldsensor1Level) {      // if the sensor1 level has changed
    Serial.print("lbs of pressure on sensor 1 is now: "); // print it
    Serial.println(vout[0]);
    sensor1LevelChar.writeValue(vout[0]);  // and update the sensor level characteristic
    oldsensor1Level = vout[0];           // save the level for next comparison
  }
  if (vout[1] != oldsensor2Level) {      // if the sensor2 level has changed
    Serial.print("albs of pressure on sensor 2 is now: "); // print it
    Serial.println(vout[1]);
    sensor2LevelChar.writeValue(vout[1]);  // and update the sensor level characteristic
    oldsensor2Level = vout[1];           // save the level for next comparison
  }
  if (vout[2] != oldsensor3Level) {      // if the sensor3 level has changed
    Serial.print("lbs of pressure on sensor 3 is now: "); // print it
    Serial.println(vout[2]);
    sensor3LevelChar.writeValue(vout[2]);  // and update the sensor3 level characteristic
    oldsensor3Level = vout[2];           // save the level for next comparison
  }
  //  if (vout[3] != oldultrasonicLevel) {      // if the sensor level has changed
  //    Serial.print("ultrasonic depth is now: "); // print it
  //    Serial.println(vout[3]);
  //    ultrasonicLevelChar.writeValue(vout[3]);  // and update the ultrasonic level characteristic
  //    oldultrasonicLevel = vout[3];           // save the level for next comparison
  //  }
}
