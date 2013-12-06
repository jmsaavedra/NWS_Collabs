#pragma once
#include "ofMain.h"

class ParticleJ{
public:
    ParticleJ();
    void setup(ofVec2f Pos, float Vel,float hue);
    //void passValues(vector <float *> values);
    void passValues(float *_xNoiseValue, float *_yNoiseValue, float *_zNoiseValue, float *_xNoiseScale, float *_yNoiseScale, float *_xAngleOffset,float *_yAngleOffset);
    void update(float vecScale);
    void draw();
    
    ofVec2f pos, vel, noiseVec;
    float noiseFloat, lifeTime, age;
    bool isDead;
    ofColor c;
    
    float *xNoiseValue,*yNoiseValue,*zNoiseValue;
    float *xNoiseScale,*yNoiseScale;
    float *xAngleOffset,*yAngleOffset;
    
    
    
};
