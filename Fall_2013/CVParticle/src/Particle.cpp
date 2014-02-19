//
//  Particle.cpp
//  CVParticle
//
//  Created by xjjulia on 28/10/13.
//
//

#include "Particle.h"

Particle::Particle(){
    
}

void Particle::setup(){
    pos.x=ofRandom(ofGetWindowWidth());
    pos.y=ofRandom(ofGetWindowHeight());
    vel.x=ofRandom(-1, 1);
    vel.y=ofRandom(-1, 1);
    
}

void Particle::update(){
    pos += vel;
    if (pos.x<0.0||pos.x>ofGetWindowWidth()) {
        vel.x *= -1.0;
    }
    if (pos.y<0.0||pos.y>ofGetWindowHeight()) {
        vel.y *= -1.0;
        }
}

void Particle::draw(){
    ofSetColor(color);
    ofCircle(pos, 10);
}
