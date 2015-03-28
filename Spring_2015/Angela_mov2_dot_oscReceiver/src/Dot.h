#ifndef dot_h
#define dot_h

#include "ofMain.h"

class Dot {
public:
    // Constructor
    Dot();
    
    // Methods
    void update();
    void draw();
    
    // Properties
    int wWidth;
    int wHeight;
    
    float px, py, vx, vy;
    float prevx, prevy, baseVx, baseVy;
    float baseFft;
    
    int circleSize;
    int shadowCircleSize;
    int vecTimes;
    
    ofColor color;
};

#endif