#pragma once
#include "ofMain.h"

class Particle{
public:
    Particle();
    void setup(ofVec2f Pos, float Vel,float hue);
    //void passValues(vector <float *> values);
    void passValues(float *_xNoiseValue, float *_yNoiseValue, float *_zNoiseValue, float *_xNoiseScale, float *_yNoiseScale, float *_xAngleOffset,float *_yAngleOffset);
    void update();
    void draw();
    
    ofVec2f pos, vel, noiseVec;
    float noiseFloat, lifeTime, age;
    bool isDead;
    ofColor c;
    
    float *xNoiseValue,*yNoiseValue,*zNoiseValue;
    float *xNoiseScale,*yNoiseScale;
    float *xAngleOffset,*yAngleOffset;
    
};
