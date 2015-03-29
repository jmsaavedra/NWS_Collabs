#pragma once


#include "ofMain.h"
#include "surinachOsc.h"
#include "oscFFT.h"

//all other classes
#include "kai_01.h"
#include "soundizeMeApp.h"
#include "Angela.h"


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
    
    int movementNumber;
    
    kai_01 kai;
    santiSoundize santi;
    Angela angela;
    
    surinachOsc oscRMS;
    surinachOscFft oscFFT;

};
