#pragma once


#include "ofMain.h"
//#include "surinachOsc.h"
#include "oscFFT.h"

//all other classes
#include "kai_01.h"
#include "soundizeMeApp.h"
#include "Angela.h"
#include "kiyo_circle.h"
#include "yu.h"

#define nBANDS 512

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
    float chan1Amp;
    surinachOscFft oscFFT;
    
    float smoothedFft_01[nBANDS];
    float smoothedFft_02[nBANDS];

    float smoothedAmp[4];

    
    
    kai_01 kai;
    santiSoundize santi;
    Angela angela;
    kiyoCircle kiyo1;
    Yu yu;


};
