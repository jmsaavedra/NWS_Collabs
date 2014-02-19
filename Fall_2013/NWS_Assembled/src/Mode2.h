//
//  Mode2.h
//  NWS_Assembled
//
//  Created by Matt on 11/7/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#include "ParticleN.h"
#include "vectorField.h"


#define BUFFER_SIZE 512

class Mode2 : public ofSimpleApp {
	
public:
	
	void setup();
	void update(FFTOctaveAnalyzer *FFTanalyzer);
	void draw();
	
	void keyPressed  (int key);
	void keyReleased (int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	
	vector <ParticleN> particles;
	vectorField VF;
	
	float prevMouseX;
	float prevMouseY;
    
    // GUI
    ofxPanel gui;
    ofxFloatSlider friction;
    ofxFloatSlider radius;
    ofxColorSlider color;
    ofxButton resetButton;
    ofxButton clearButton;
    ofxButton randomVF;
    
    //Gui Event Listener
    void frictionChanged(float & friction);
    void radiusChanged(float & radius);
    void resetButtonPressed();
    void clearButtonPressed();
    void randomButtonPressed();
    
	// moved to testapp
	/*
    void audioReceived(float* input, int bufferSize, int nChannels);
	ofSoundStream soundStream;

    FFTOctaveAnalyzer FFTanalyzer;
    
    float left[BUFFER_SIZE];
    float right[BUFFER_SIZE];
    
    fft		myfft;
    
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    float freq[BUFFER_SIZE/2];
	 */
    
    
    float sum, ave, Lave, Have; //Lave average of low frequency, Have average of hight frequency
    float sumL, sumH;
    int alpha;
    int blue, red;
    float nradius;
	
    ofFbo   mFbo;
	
    
};
