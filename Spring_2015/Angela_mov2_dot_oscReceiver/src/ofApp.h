#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "Dot.h"

// listen on port 12345
#define PORT 9001
#define NUM_MSG_STRINGS 4

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

		ofTrueTypeFont font;
		ofxOscReceiver receiver;
    
    float * receivedFft;
    int nBandsToUse;
    int refBandIndex;
    
		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		int mouseX, mouseY;
		string mouseButtonState;
    
        ofImage receivedImage;
    Dot colorDot;
    Dot bigDot;
    
    ofVideoPlayer vidPlayer;
};
