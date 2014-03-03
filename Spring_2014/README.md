Spring 2014 :: NWS Collab
------

Fall 2013 and Spring 2014 saw two separate collaborations between Parsons MFA DT students in NYC and the New World Symphony based in Miami. Various selected pieces were augmented by Parsons and brought to life visually in the NWS auditorium, culminating in a performance both semesters.
**Note:** to run OF projects, clone inside of the root OF folder, alongside addons, apps, examples, etc.

###Setup Notes###
* each sensor unit has a unique ID number
* all sensors are programmed to connect to a specific WiFi network:
  * SSID: internetz
  * pass: 1nt3rn3tz
* all sensors are looking for a HOST at the IP Address: **10.0.1.14**
* all sensors are programmed to talk on port **11999**
* we are using the Blackmagic [UltraStudio Thunderbolt](http://www.blackmagicdesign.com/products/ultrastudiothunderbolt) video breakout
	* video will come in from their system to this (SDI)
	* all OF video output will go to their system through this (HDMI/SDI)
	* joe can lend this to Gabriel/Bernardo ASAP. no specific drivers needed.
* audio will come from them in 1/4" audio patch cables. we will use an [Akai EIE](http://www.akaipro.com/product/eie#downloads) for this.
	* install the drivers from that link
	* joe can lend this to Gabriel/Bernardo ASAP.

Current WiFly OF test app is [wifly-oF-connection](https://github.com/jmsaavedra/NWS_Collabs/tree/master/Spring_2014/wifly-oF-connection)
Current Arduino Firmware is [nws_sensor11](https://github.com/jmsaavedra/NWS_Collabs/tree/master/Spring_2014/firmware/nws_sensor11)

###TO DO LIST###
1. currently sensor readings are mapped from -20 through 20 to 0 through 255. **a resolution of 40 is unacceptable.**
* overall sensor update to OF speed is ~28fps, assuming no dropped data packets. this can get better with changing baud rate of WiFly to 57600 (currently at 9600)
* currently all sensor data is stored in a vector for accel and a vector for mag. vectors need to be cleared after x readings. should be made into a class for every sensor unit
* bracelets need another piece of velcro, permanent labeling, any other protection/adhesives to keep battery together
* test **multiple bracelets** to 1 OF app!
* more progress on OF apps:
	* include ofxSyphon, publish textures to a syphon instance
	* merge in hardware code
	* check out [FFT OSC app example here](https://github.com/jmsaavedra/NWS_Collabs/tree/master/Spring_2014/FFTAudioAnalysis)
	* consider using found footage/materials to incorporate
	* consider creating more of a "narrative"
* take pictures of all units -- DOCUMENT !
* paint rainbow wire? something to consider? might be cool to see the colors



