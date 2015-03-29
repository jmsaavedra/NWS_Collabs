#include "ofApp.h"

void ofApp::setup(){
    
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(128);
    
    
    fft_size = 512;
    buffer_size = fft_size * 2;
    
    
    left = new float[buffer_size];
    magnitudeL = new float[fft_size];
    phaseL = new float[fft_size];
    powerL = new float[fft_size];
    
    right = new float[buffer_size];
    magnitudeR = new float[fft_size];
    phaseR = new float[fft_size];
    powerR = new float[fft_size];
    
    my_stream.setup(this, 0, 2, 44100, 256, 4);
    
    //ofSoundStreamSetup(0, 2, this, 44100, buffer_size, 4);
    //soundPlayer.loadSound("Alone In The Ranse.mp3");
    //soundPlayer.setLoop(true);
    //soundPlayer.play();
}

void ofApp::update() {
    
    
    avg_powerL = 0.0;
    avg_powerR = 0.0;
    myfft.powerSpectrum(0, fft_size, left, buffer_size,	magnitudeL, phaseL, powerL, &avg_powerL);
    myfft.powerSpectrum(0, fft_size, right, buffer_size, magnitudeR, phaseR, powerR, &avg_powerR);
}

void ofApp::draw() {
    
    
    float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
    for (int i = 0; i < fft_size; i++) {
        
        
        ofColor col;
        col.setHsb(i * 255.0f / (float)fft_size, 255, 255, 31);
        ofSetColor(col);
        
        ofCircle(ofGetWidth()/2 - w * i, ofGetHeight()/2, magnitudeL[i] * ofGetWidth()/100.0);
        ofCircle(ofGetWidth()/2 + w * i, ofGetHeight()/2, magnitudeR[i] * ofGetWidth()/100.0);
    }
}

void ofApp::keyPressed(int key){
    
    ofToggleFullscreen();
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels) {
    
    
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }
}
