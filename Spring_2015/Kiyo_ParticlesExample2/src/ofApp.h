#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ParticleSystem.h"

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
    
    vector<ParticleSystem> systems;     //Vector of more particle systems
    vector<ofColor> colors;             //Palette
    ofTexture image;                    //Image for the particles
    float time0;                        //starting time
    
    float threshold;
    
    ofSoundPlayer track;                //Track
    const int N = 2;                    //Number of particle systems
    vector<float> spectrum;             //Current spectrum
    vector<float> prevSpectrum;         //Spectrum of the previous step
    
    
    ofPoint gravity = ofPoint(0, 9.8);    //Gravity
    
};
