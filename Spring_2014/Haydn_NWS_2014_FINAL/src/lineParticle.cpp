//
//  lineParticle.cpp
//  haydn_03_visuals
//
//  Created by Bernardo Santos Schorr on 3/6/14.
//
//

#include "lineParticle.h"


void LineParticle::setup() {
    
    angle = ofRandom(TWO_PI);
    rad = ofRandom(0.2, 5) * RADIUS;
    speed = ofRandom(-.005, .005);
    pos.x = ofGetWindowWidth()/2;
    pos.y = ofGetWindowHeight()/2;
    nbConnex = 0;
    diam = 0;
    
}

void LineParticle::update() {
    
    angle += speed;
    pos.x = ease(pos.x, ofGetWindowWidth()/2 + cos(angle) * rad, 0.1);
    pos.y = ease(pos.y, ofGetWindowHeight()/2 + sin(angle) * rad, 0.1);
    diam = ease(diam, min(nbConnex, 7)*(rad/RADIUS), 0.1);
    
    nbConnex = 0;
    
    
}

void LineParticle::draw() {
    
}

float LineParticle::ease(float variable, float target, float easingVal) {
    float d = target - variable;
    if (abs(d)>1) variable+= d*easingVal;
    return variable;
}