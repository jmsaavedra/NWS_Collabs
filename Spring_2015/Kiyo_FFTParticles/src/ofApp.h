#pragma once

#include "ofMain.h"
#include "ofxFFTLive.h"
#include "ofxFFTFile.h"
#include "ofxGui.h"
#include "Particle.h"

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
    
    static const int NUM = 1000;
    Particle particle[NUM];
    ofColor color[NUM];
    
    //FFT
    ofSoundPlayer soundPlayer;
    
    //ofSoundStream my_stream;
    //void audioln(float*input, int bufferSize, int nChannels);
    
    ofxFFTLive fftLive;
    ofxFFTFile fftFile;
    
    //GUI
    bool display = false;
    ofxPanel gui;
    ofxSlider<float> audioThreshold;
    ofxSlider<float> audioPeakDecay;
    ofxSlider<float> audioMaxDecay;
    ofxSlider<float> radius;
    ofxSlider<float> gravity;
    ofxToggle audioMirror;
    
    //Mode
    int mode = 1;
    
    //Blend
    ofBlendMode blendMode;
    int blend = 1;
};