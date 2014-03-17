//
//  Particle.h
//  spacebrewSoundflower_example
//
//  Created by Bernardo Santos Schorr on 3/1/14.
//
//

#pragma once
#include "ofMain.h"
#include "ofxOpticalFlowFarneback.h"

class Curtain {
public:
    
    void setup(ofVec2f _pos);
    //void update(ofVec2f _mouse);
    void update (ofxOpticalFlowFarneback &_flowSolver);
    void draw();
    
    ofVec2f initPos, pos, vel, accel;
    
    int resolution;
    
    
    
};
