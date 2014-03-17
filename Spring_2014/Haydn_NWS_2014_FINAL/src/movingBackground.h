//
//  movingBackground.h
//  haydn_01_visuals
//
//  Created by Bernardo Santos Schorr on 3/5/14.
//
//

#pragma once

#include "ofMain.h"

class MovingBackground {
public:
    
    MovingBackground(float * _multiplier);
    void setup(string file);
    void update(float individualSensorID, ofVec3f individualAccel, ofVec3f individualMagne);
    void draw();
    
    ofImage bkg;
    ofVec2f pos, vel, accel, prevAccel;
    
    float * multiplier;
    
    
};
