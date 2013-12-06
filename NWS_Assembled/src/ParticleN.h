#pragma once
#include "ofMain.h"

class ParticleN {

public:
    ParticleN();
    void setup(ofVec2f position, ofVec2f velocity);
    void setup(float positionX, float positionY, float velocityX, float velocityY);
    void resetForce();
    void addForce(ofVec2f force);
    void addForce(float forceX, float forceY);
    void updateForce();
    void updatePos();
    void update();
    void bounceOffWalls();
    void draw();
    void addRepulsionForce(float x, float y, float radius, float scale);
    void addRepulsionForce(ParticleN &p, float radius, float scale);

    void addAttractionForce(float x, float y, float radius, float scale);
    void addAttractionForce(ParticleN &p, float radius, float scale);
    
    void addCounterClockwiseForce(ParticleN &p, float radius, float scale);
    
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f force;
    float friction;
    float radius;
    bool bFixed;
};