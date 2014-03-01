//
//  Particle.cpp
//  test_01
//
//  Created by Bernardo Santos Schorr on 2/27/14.
//
//

#include "Particle.h"

void Particle::setup(ofVec2f _pos, ofVec2f _vel) {
    
    pos = _pos;
    vel = _vel;
    life = 255;
    inc = 100/vel.y;
    
}

void Particle::update() {
    
    
    float noise = ofNoise(pos.x * 0.005, pos.y*0.005, ofGetElapsedTimef() * 1.0) * 10.0;
    
    float agePct;
    
    if (life > 0) {
        agePct = 1.0 - (255.0 / life);
    } else {
        agePct = 0.0;
    }
    
    pos += ofVec2f( cos(noise), sin(noise) ) * (1.0-agePct) * 0.5;
    pos += vel;
    vel *= 0.98;
    life -= 5;
    size = ofMap(life, 0, 255, 0, 40);
    
}

//we're not even using this
void Particle::draw() {
    
    ofSetColor(255, life);
    ofCircle(pos, 3);
    
}