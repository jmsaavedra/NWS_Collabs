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
    void nextMovie();
    
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
    
    ofVideoPlayer myPlayer;
    int currMovieIdx;
    
    string movies[19] = {
        "movies/1_A1.mp4",
        "movies/1_A3.mp4",
        "movies/1_AB1.mp4",
        "movies/1_B4.mp4",
        "movies/1_C2.mp4",
        "movies/1_C3.mp4",
        "movies/2_2.mp4",
        "movies/2_3.mp4",
        "movies/2_4.mp4",
        "movies/3_A2.mp4",
        "movies/3_B1.mp4",
        "movies/3_C1.mp4",
        "movies/3_C2.mp4",
        "movies/4_A1.mp4",
        "movies/4_A2.mp4",
        "movies/4_A3.mp4",
        "movies/4_AB.mp4",
        "movies/4_B1.mp4",
        "movies/4_C1.mp4",
    };

};
