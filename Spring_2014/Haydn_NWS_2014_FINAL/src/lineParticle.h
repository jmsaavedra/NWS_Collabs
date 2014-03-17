//
//  lineParticle.h
//  haydn_03_visuals
//
//  Created by Bernardo Santos Schorr on 3/6/14.
//
//

#pragma once
#include "ofMain.h"

#define RADIUS 80

class LineParticle {
public:
    
    void setup();
    void update();
    void draw();
    float ease(float variable, float target, float easingVal);
    
    float angle, rad, speed, diam;
    int nbConnex;
    
    ofVec2f pos;
    
    
    
};
