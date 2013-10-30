#pragma once
#include "ofMain.h"

class Particle{
    public:
    Particle();
    void setup();
    void update();
    void draw();
    
    ofVec2f pos,vel,acc;
    float flySize;
    float angleX,angleY,angleZ;
    vector<ofImage> butterflyList;
    string stringList[5];
    
};
