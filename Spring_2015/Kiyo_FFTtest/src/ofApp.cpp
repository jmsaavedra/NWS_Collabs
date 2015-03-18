#include "ofApp.h"

void ofApp::setup(){
    
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    
    fft_size = 16384;
    buffer_size = fft_size * 2;
    
    
    audio_input = new float[buffer_size];
    magnitude = new float[fft_size];
    phase = new float[fft_size];
    power = new float[fft_size];
    locOffsetX = new float[fft_size];
    locOffsetY = new float[fft_size];
    magnitude_average = new float[fft_size];
    magnitude_average_snapshot = new float[fft_size];
    circle_phase = new float[fft_size];
    
    
    ofSoundStreamSetup(0, 1, this, 44100, buffer_size, 4);
    
    soundPlayer.loadSound("Alone In The Ranse.mp3");
    soundPlayer.setLoop(true);
    soundPlayer.play();
}

void ofApp::update() {
    
    
    float avg_power = 0.0f;
    myfft.powerSpectrum(0, (int)fft_size, audio_input, buffer_size,	magnitude, phase, power, &avg_power);
    
    
    for (int i = 0; i < fft_size; i++) {
        magnitude[i] = powf(magnitude[i], 0.5);
    }
    
    for (int i = 0; i < fft_size; i++) {
        float x = 0.1;
        magnitude_average[i] = (magnitude[i] * x) + (magnitude_average[i] * (1 - x));
    }
    
    for (int i = 0; i < fft_size; i++) {
        float v = fabs(magnitude_average[i] - magnitude_average_snapshot[i]);
        circle_phase[i] = v;
    }
}

void ofApp::draw() {
    
    
    circleSize = ofGetWidth()/(float)fft_size/4.0;
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    for (int i = 0; i < fft_size; i++){
        ofSetColor((255-255*i/fft_size)/2,100*i/fft_size,255*i/fft_size,200);
        float xx = cos(i/TWO_PI+circle_phase[i]*2.0) * i * circleSize;
        float yy = sin(i/TWO_PI+circle_phase[i]*2.0) * i * circleSize;
        ofCircle(xx, yy, circle_phase[i]*4.0);
    }
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels) {
    
    
    memcpy(audio_input, input, sizeof(float) * bufferSize);
}

void ofApp::keyPressed(int key){
    
    
    if (key == 'f') {
        ofToggleFullscreen();
    }
}