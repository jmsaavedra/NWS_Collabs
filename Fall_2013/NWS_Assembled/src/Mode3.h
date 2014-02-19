//
//  Mode3.h
//  NWS_Assembled
//
//  Created by Matt on 11/7/13.
//
//

#pragma once

#include "ofMain.h"

#include "ParticleJ.h"
#include "ofxUI.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#define BUFFER_SIZE 512



class Mode3 : public ofBaseApp{
	
public:
	void setup();
	void update(FFTOctaveAnalyzer *FFTanalyzer);
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
    
    void addParticle(ofVec2f pos, float vel, float hue);
    vector<ParticleJ> pList;
    ofVec2f pos;
    float vel;
    float hue;
    
    ofxUICanvas *gui;
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    
    //vector <float &> values;
    float xNoiseValue,yNoiseValue,zNoiseValue;
    float xNoiseScale,yNoiseScale;
    float xAngleOffset,yAngleOffset;
    
    float speedScale;

	
};
