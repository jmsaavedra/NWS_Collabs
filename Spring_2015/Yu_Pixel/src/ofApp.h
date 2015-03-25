#pragma once

#include "ofMain.h"
#define BUFFER_SIZE 512

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // image 
    ofImage img;
    int w;
    int h;
    int alpha;
    float pixelSize;
    float size;
    
    ofSoundPlayer sound;	//Sound sample
    
    
    ofVideoPlayer my_video;
    
    
};
