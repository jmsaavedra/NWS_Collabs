/*
 *
 * This sketch implements a simple UDP client that sends UDP packets
 * to two UDP servers.
 *
 * This sketch is released to the public domain.
 *
 */
 
 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <SoftwareSerial.h>
#include <WiFlyHQ.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

#define ID "4" //THIS UNIT'S ID! EACH WILL NEED A UNIQUE ONE, THEY ARE LABELED
#define HOST "10.0.1.14"
#define PORT 11999


// Accelerometer Variables
float rawAccelX;                      
float rawAccelY;
float rawAccelZ;
int AccelX;
int AccelY;
int AccelZ;


// Magnetometer Variables
float rawMagneX;
float rawMagneY;
float rawMagneZ;
int MagneX;
int MagneY;
int MagneZ;


const int accelLow  = -20;              // Constrained Accelerometer Values
const int accelHigh = abs(accelLow);
const int magneLow = -20;               // Constrained Magnetometer Values
const int magneHigh = abs(magneLow);
int mapValue = 255;                     // Maximum output value (Set to byte data type)

/* Change these to match your WiFi network */
const char mySSID[] = "internetz";
const char myPassword[] = "1nt3rn3tz";

void terminal();

WiFly wifly;

void setup()
{
  char buf[32];

  Serial.begin(115200);
  
//  while(!Serial){ /*FOR TESTING*/
//    ;
//  }

  Serial.println("Starting");
  Serial.print("Free memory: ");
  Serial.println(wifly.getFreeMemory(),DEC);

  Serial1.begin(9600); //IF NEVER SET BEFORE TO 57600
//  
//  Serial.println("setting to 57600");
//  wifly.setBaud(57600);
//  wifly.save();
//  wifly.reboot(); 
//  wifly.setBaud(57600);  
//  Serial.println("DONE setting to 57600");
  
  if (!wifly.begin(&Serial1, &Serial)) {
    Serial.println("Failed to start wifly");
    terminal();
  }

  if (wifly.getFlushTimeout() != 15) {
    Serial.println("wifly factoryRestore()");
    wifly.factoryRestore();
    delay(1000);
    Serial.println("Restoring flush timeout to 10msecs");
    wifly.setFlushTimeout(10);
    wifly.save();
    wifly.reboot();
  }

  /* Join wifi network if not already associated */
  while (!wifly.isAssociated()) {
    /* Setup the WiFly to connect to a wifi network */
    Serial.println("Joining network");

    wifly.setSSID(mySSID);
    wifly.setPassphrase(myPassword);

    if (wifly.join()) {
      Serial.println("Joined wifi network");
      break;
    }
    else {
      Serial.println("Failed to join wifi network");
      //terminal();
      delay(5000);
      Serial.println("Retrying join()");
    }
  }
  //else {
  Serial.println("Already joined network");
  //}

  /* Setup for UDP packets, sent automatically */
  wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
  
  /* Set UDP packet to server and port */
  wifly.setHost(HOST, PORT);    

  Serial.print("MAC: ");
  Serial.println(wifly.getMAC(buf, sizeof(buf)));
  Serial.print("IP: ");
  Serial.println(wifly.getIP(buf, sizeof(buf)));
  Serial.print("Netmask: ");
  Serial.println(wifly.getNetmask(buf, sizeof(buf)));
  Serial.print("Gateway: ");
  Serial.println(wifly.getGateway(buf, sizeof(buf)));

  wifly.setDeviceID("Wifly-UDP");
  Serial.print("DeviceID: ");
  Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

  Serial.println("WiFly ready");

  if(!accel.begin()) while(1);
  if(!mag.begin()) while(1);
  
  Serial.println("Sensors successfully initialized");
}

uint32_t count=0;
uint8_t tick=0;
uint32_t lastSend = 0;  /* Last time message was sent */

void loop() {
  
//  wifly.print("Hello");
//  delay(1000);
  sensors_event_t event;
  
  String data;
  data = "/";
  data += ID;
  data += "\t";
  
  // GET, MAP, AND PRINT ACCELEROMETER READINGS
  accel.getEvent(&event);
  rawAccelX = constrain(event.acceleration.x, accelLow, accelHigh);
  rawAccelY = constrain(event.acceleration.y, accelLow, accelHigh);
  rawAccelZ = constrain(event.acceleration.z, accelLow, accelHigh);
  AccelX = int(map(rawAccelX, accelLow, accelHigh, 0, mapValue));
  AccelY = int(map(rawAccelY, accelLow, accelHigh, 0, mapValue));
  AccelZ = int(map(rawAccelZ, accelLow, accelHigh, 0, mapValue));
  
  data += AccelX;
  data += "\t";
  data += AccelY;
  data += "\t";
  data += AccelZ;
  data += "\t";

  // GET, MAP, AND PRINT MAGNETOMETER READINGS
  mag.getEvent(&event);
  rawMagneX = constrain(event.magnetic.x, magneLow, magneHigh);
  rawMagneY = constrain(event.magnetic.y, magneLow, magneHigh);
  rawMagneZ = constrain(event.magnetic.z, magneLow, magneHigh);
  MagneX = int(map(rawMagneX, magneLow, magneHigh, 0, mapValue));
  MagneY = int(map(rawMagneY, magneLow, magneHigh, 0, mapValue));
  MagneZ = int(map(rawMagneZ, magneLow, magneHigh, 0, mapValue));

  data += MagneX;
  data += "\t";
  data += MagneY;
  data += "\t";
  data += MagneZ;
  
//  data += "/";
  wifly.print(data);
  delay(35); //for now!
  
 }
 
 void terminal()
{
  Serial.println("Terminal ready");
  while (1) {
    if (wifly.available() > 0) {
      Serial.write(wifly.read());
    }

    if (Serial.available()) {
      wifly.write(Serial.read());
    }
  }
}

