#include "Dot.h"

Dot::Dot(){
//    color.set(255, 255, 255);
    
//    px = ofRandom(0, wWidth);
//    py = ofRandom(0, wHeight);
    
//    px = wWidth/2;
//    py = wHeight/2;
    
    vx = 0.5;
    vy = 0.2;
    
    baseVx = ofRandom(-10, 10);
    baseVy = ofRandom(-10, 10);
    
    circleSize = 800;
    circleSize2 = 0;
    shadowCircleSize = 5000;
    vecTimes = 1000;
}

void Dot::update(){
    
    baseVx = ofRandom(-50, 50);
    baseVy = ofRandom(-50, 50);
    
    // (1) we increase px and py by adding vx and vy
    px += baseVx + vx * baseFft * vecTimes;
    py += baseVy + vy * baseFft * vecTimes;
    
    // (2) check for collision, and trigger sounds:
    // horizontal collisions:
    if (px < 0){
        px = 0;
        vx *= -1;
    } else if (px > ofGetWidth()){
        px = ofGetWidth();
        vx *= -1;
    }
    else {
        vx = ofRandom(baseFft * -1, baseFft);
    }
    // vertical collisions:
    if (py < 0 ){
        py = 0;
        vy *= -1;
    } else if (py > ofGetHeight()){
        py = ofGetHeight();
        vy *= -1;
    }
    else {
        vy = ofRandom(baseFft * -1, baseFft);
    }
}

void Dot::draw(){
    
    if(circleSize2 < 8) {
        ofFill();
    }
    ofCircle(px, py, circleSize2);
}