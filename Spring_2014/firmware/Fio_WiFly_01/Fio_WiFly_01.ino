/*

Based on the WiFlyHQ udpclient.ino example

https://github.com/lpercifield/WiFlyHQ
forked from
https://github.com/harlequin-tech/WiFlyHQ

 WiFly setup uses Serial1 as well as Serial 
 - the Atmega32u4 has multiple hardware serial ports.
 - usually you would use SoftwareSerial lib, but
 hardware serial comm is far more robust (and can go faster!)
 
 */


#include <WiFlyHQ.h>

/* Change these to match your WiFi network */
const char mySSID[] = "internetz";  /* network name */
const char myPassword[] ="1nt3rn3tz";/* network password */

WiFly wifly;

int analogData;
//uint32_t lastSend = 0; //sometimes helpful for debugging
//uint32_t sendCount=0;

void setup()
{
    char buf[32];

    Serial.begin(9600); //start up the USB serial port for debugging
    Serial.println("Starting");
//    Serial.print("Free memory: ");
//    Serial.println(wifly.getFreeMemory(),DEC);

    Serial1.begin(9600); //start up the Serial1 port for communication with WiFly
    //TODO: set the WiFly to run at 115200 baud. we'll get faster (higher res) 
    //  sensor data going to OF app

    if (!wifly.begin(&Serial1,&Serial)) { 
        Serial.println("Failed to start wifly");
    }
    
    if (wifly.getFlushTimeout() != 10) { //flush timeout is an internal WiFly setting
        Serial.println("Restoring flush timeout to 10msecs");
        wifly.setFlushTimeout(10);  //we want it to be set to 10ms
	wifly.save();
	wifly.reboot();
    }

    /* Join wifi network if not already associated */
    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
	Serial.println("Joining network");
	wifly.setSSID(mySSID);
	wifly.setPassphrase(myPassword);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	}
    } else {
        Serial.println("Already joined network");
    }
    /* print out information about our unit, for debugging */
    Serial.print("MAC: ");
    Serial.println(wifly.getMAC(buf, sizeof(buf)));
    Serial.print("IP: ");
    Serial.println(wifly.getIP(buf, sizeof(buf)));
    Serial.print("Netmask: ");
    Serial.println(wifly.getNetmask(buf, sizeof(buf)));
    Serial.print("Gateway: ");
    Serial.println(wifly.getGateway(buf, sizeof(buf)));

    /* 
    we can set a device ID -- will be helpful when we 
    have 4 units going at once for the string quartet
    */
//    wifly.setDeviceID("Wifly-WebClient"); 
//    Serial.print("DeviceID: ");
//    Serial.println(wifly.getDeviceID(buf, sizeof(buf)));

    /* Setup for UDP packets, sent automatically */
    /* http://en.wikipedia.org/wiki/User_Datagram_Protocol */
    wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);
    wifly.setHost("10.0.1.13", 11997);	// Send UDP packet to this server and port

    wifly.setDeviceID("Wifly-UDP");
    wifly.setHost("10.0.1.13", 11999);	// Send UDP packets to this server and port
    Serial.println("WiFly ready");
}


void loop() {
  
    analogData = analogRead(A5);
    Serial.println(analogData); /*this is helpful testing, but want to comment it out for any *real* use*/
    wifly.print(analogData);
    wifly.print("[/p]");

    /* students found this helped keep the connection stable, 
    but we'll want to work on shortening this delay, 
    or finding a way to remove altogether */
    delay(50);               
}
