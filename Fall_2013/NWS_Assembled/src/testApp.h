#pragma once

#include "ofMain.h"

#include "ofxSyphon.h"

#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#include "vectorField.h"

#include "Mode1.h"
#include "Mode2.h"
#include "Mode3.h"

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// create classes for different modes
	Mode1 mode1;
	Mode2 mode2;
	Mode3 mode3;
	
	bool useNori;
	bool appRunning;
	
	// create fbos for each mode
	ofFbo fbo1, fbo2, fbo3;
	
	// FFT stuff
	void audioReceived(float* input, int bufferSize, int nChannels);

	ofSoundStream soundStream;
    FFTOctaveAnalyzer FFTanalyzer;
	fft myfft;
    
    float left[BUFFER_SIZE];
    float right[BUFFER_SIZE];

    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    float freq[BUFFER_SIZE/2];
	
	// syphon
	ofxSyphonServer syphon;
	
};
