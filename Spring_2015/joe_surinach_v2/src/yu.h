#pragma once

#include "ofMain.h"
//#include "ofxOsc.h"

// listen on port 12345
//#define PORT 9001
//#define NUM_MSG_STRINGS 4

class Yu {
	public:

		void setup();
		void update(float amp);
		void draw(float fft[]);

		void keyPressed(int key);


		ofTrueTypeFont font;
		ofxOscReceiver receiver;
    
    float * receivedFft;
    int nBandsToUse;
		int current_msg_string;
		string msg_strings[NUM_MSG_STRINGS];
		float timers[NUM_MSG_STRINGS];

		int mouseX, mouseY;
		string mouseButtonState;
    
//        ofImage receivedImage;
    
        ofImage img;
        int w;
        int h;
        int alpha;
        float pixelSize;
        float size;
    
    unsigned char * pixels;
    
    int theme;
    
//        ofVideoPlayer my_video;
};
