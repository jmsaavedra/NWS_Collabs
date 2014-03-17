//
//  Particle.cpp
//  algo_week_10_01_moreParticlesThanTheEyeCanSee
//
//  Created by Bernardo Santos Schorr on 11/4/13.
//
//

#include "ParticleOrbit.h"

void ParticleOrbit::setup( ofVec2f &_pos, int _life, float _velMult ) {
    
    pos = _pos;
    prevPos = pos;
    float tempAngle =  ofRandom (360);
    vel.set( cos(tempAngle), sin(tempAngle) );
    life = _life;
    vel *= _velMult;
    
}

void ParticleOrbit::update() {
    
    ofVec2f center = ofGetWindowSize()/2;
    ofVec2f force = center - pos;
    force.normalize();
    acc = force * 0.05;
    //acc = force;
    
    vel += acc;
    prevPos = pos;
    pos += vel;
    
    float mult = ofRandom(0.98, 1.02);
    vel *= mult;
    acc *= 0;
    
    life--;
    
}

void ParticleOrbit::draw() {
    
    glVertex2f(prevPos.x, prevPos.y);
    glVertex2f(pos.x, pos.y);
    
}