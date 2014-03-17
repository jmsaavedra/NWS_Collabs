//
//  Particle.cpp
//  spacebrewSoundflower_example
//
//  Created by Bernardo Santos Schorr on 3/1/14.
//
//

#include "Curtain.h"


void Curtain::setup(ofVec2f _pos) {
    
    pos = _pos;
    initPos = pos;
    
    resolution = ofGetWindowWidth() / 160;
    
}

void Curtain::update(ofxOpticalFlowFarneback &_flowSolver) {
    
    accel = _flowSolver.getVelAtPixel(pos.x/resolution, pos.y/resolution);
    
    ofVec2f tempAccel = (initPos - pos) * 0.01;
    accel += tempAccel;
    
    pos += vel;
    vel += accel;
    vel *= 0.9;
    
}

void Curtain::draw() {
        
}