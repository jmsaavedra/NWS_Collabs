//
//  movingBackground.cpp
//  haydn_01_visuals
//
//  Created by Bernardo Santos Schorr on 3/5/14.
//
//

#include "movingBackground.h"

MovingBackground::MovingBackground (float * _multiplier) {
    multiplier = _multiplier;
}

void MovingBackground::setup(string file){
    
    vel.set(0.0);
    pos.set(ofRandomWidth(), ofRandomHeight());
    pos.y = 560;
    bkg.loadImage(file);
    
}

void MovingBackground::update(float individualSensorID, ofVec3f individualAccel, ofVec3f individualMagne){
    
    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();
    
    vel *= 0.98;
    
    accel = prevAccel - individualAccel;
    
    //update velocity and position of background according to accelerometer
    accel *= *multiplier;
    vel += ofVec2f(accel.x, accel.y);
    pos += vel;
    
    if (pos.x > width + (bkg.width * 0.5)) pos.x = -bkg.width * 0.5;
    if (pos.x < -bkg.width * 0.5) pos.x = width + bkg.width * 0.5;
    
    if (pos.y > height + bkg.height * 0.5) pos.y = -bkg.height * 0.5;
    if (pos.y < -bkg.height * 0.5) pos.y = height + bkg.height * 0.5;
    
    prevAccel = individualAccel;
    //just for safety
    accel.set(0.0);
}

void MovingBackground::draw(){
    
    ofSetColor(255, 220);
    bkg.draw(pos.x - bkg.width *0.5, pos.y - bkg.height * 0.5);
}