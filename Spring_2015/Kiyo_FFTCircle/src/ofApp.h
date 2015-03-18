#pragma once

#include "ofMain.h"
#include "fft.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed  (int key);
    void audioIn(float* input, int bufferSize, int nChannels);
    
    float *left, *right;
    float *magnitudeL, *phaseL, *powerL;
    float *magnitudeR, *phaseR, *powerR;
    float avg_powerL, avg_powerR;
    
    int buffer_size;
    int fft_size;
    
    
    ofSoundStream my_stream;
    void audioln(float*input, int bufferSize, int nChannels);
    
    fft myfft;
    
};
