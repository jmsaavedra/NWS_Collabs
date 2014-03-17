//
//  Particle.h
//  test_01
//
//  Created by Bernardo Santos Schorr on 2/27/14.
//
//

#pragma once
#include "ofMain.h"

class ParticleEmitter {
public:
    
    void setup(ofVec2f _pos, ofVec2f _vel);
    void update();
    void draw();
    
    //position, velocity and size of particle
    ofVec2f pos, vel;
    int size;
    
    //life and increment to its death
    float life;
    
    //if I take this out, it breaks mode one and I have no idea why.
    //suggestions accepted.
    float inc;
};