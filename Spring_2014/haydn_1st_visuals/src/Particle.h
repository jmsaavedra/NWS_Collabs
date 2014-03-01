//
//  Particle.h
//  test_01
//
//  Created by Bernardo Santos Schorr on 2/27/14.
//
//

#pragma once
#include "ofMain.h"

class Particle {
public:
    
    void setup(ofVec2f _pos, ofVec2f _vel);
    void update();
    void draw();
    
    ofVec2f pos, vel;
    int size;
    float life, inc;
    
    ofImage * sprite;
    
};