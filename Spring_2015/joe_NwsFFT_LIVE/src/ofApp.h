#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 9001

//#define BUFFER_SIZE 512
#define BUFFER_SIZE 1024
#define NUM_CHANNELS 8

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
    void audioReceived 	(float * input, int bufferSize, int nChannels);
    
    void sendOsc();
    
    ofSoundPlayer audioFile;
    ofSoundStream AudioIn;
    ofxOscSender  oscSender;


    int nBandsToGet;
    int nBandsToUse;
//    float         * fftSmoothed;
    float fftSmoothed[NUM_CHANNELS][BUFFER_SIZE];
    vector<float> audioBuffer;
    
    float channel[NUM_CHANNELS][BUFFER_SIZE];
    float volume[NUM_CHANNELS];
    float smoothedVol[NUM_CHANNELS];
    float scaledVol[NUM_CHANNELS];

    bool          setupComplete;
};
