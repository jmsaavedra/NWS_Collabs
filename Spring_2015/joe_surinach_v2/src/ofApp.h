#pragma once


#include "ofMain.h"
#include "oscReceiver.h"

//all other classes
#include "kai_01.h"
#include "soundizeMeApp.h"

// listen on port 12345
//#define PORT 9001
//#define NUM_MSG_STRINGS 4

class ofApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);


    ofxOscReceiver receiver;
    float * receivedFft;
    int nBandsToUse;
    //oscReceiver myOsc;
    
    int movementNumber;
    
    kai_01 kai;
    santiSoundize santi;


};
