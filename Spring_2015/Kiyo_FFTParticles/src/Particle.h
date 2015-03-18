#pragma once
#include "ofMain.h"

class Particle {
    
public:
    Particle();
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
    void addRepulsionForce(Particle &p, float radius, float scale);
    void addAttractionForce(float x, float y, float radius, float scale);
    void addAttractionForce(Particle &p, float radius, float scale);
    void addCounterClockwiseForce(Particle &p, float radius, float scale);
    
    //Set Rect or Circle
    void setFigure(int figure);
    
    void setColor(float hue, float sat, float bri);
    ofColor getColor();
    
    void setRadius(float radius);
    
    ofVec2f position;
    ofVec2f velocity;
    ofVec2f force;
    float friction;
    float radius;
    int figure;
    bool bFixed;
    
    ofColor c;
    
};