//
//  Particle.cpp
//  Perlin_Points
//
//  Created by xjjulia on 21/10/13.
//
//

#include "Particle.h"

Particle::Particle(){
    
}

void Particle::setup(ofVec2f Pos, float Vel,float hue){
    pos = Pos;
    vel.x=ofRandom(-Vel, Vel);
    vel.y=ofRandom(-Vel,Vel);
    lifeTime = ofRandom(200,300);
    age = 0;
    isDead = false;
    c.setHsb(hue+ofRandom(-20.0,20.0), 255,200);
   
    
}

void Particle::passValues(float *_xNoiseValue, float *_yNoiseValue, float *_zNoiseValue, float *_xNoiseScale, float *_yNoiseScale, float *_xAngleOffset,float *_yAngleOffset) {
    
    xNoiseValue = _xNoiseValue;
    yNoiseValue = _yNoiseValue;
    zNoiseValue = _zNoiseValue;

    xNoiseScale = _xNoiseScale;
    yNoiseScale = _yNoiseScale;
    
    xAngleOffset = _xAngleOffset;
    yAngleOffset = _yAngleOffset;
    
}

void Particle::update(float vecScale){
//    noiseFloat = ofNoise(pos.x * 0.0025, pos.y * 0.0025, ofGetElapsedTimef() * 0.001);
//    noiseVec.x = cos(((noiseFloat -0.3) * TWO_PI) * 10);
//    noiseVec.y = sin(((noiseFloat - 0.3) * TWO_PI) * 10);
//    cout<<noiseVec<<endl;
    
    
    noiseFloat = ofNoise(pos.x * *xNoiseValue, pos.y * *yNoiseValue, ofGetElapsedTimef() * *zNoiseValue);
    noiseVec.x = vecScale * cos(((noiseFloat -0.3) * (TWO_PI + *xAngleOffset)) * *xNoiseScale);
    noiseVec.y = vecScale * sin(((noiseFloat - 0.3) * (TWO_PI + *yAngleOffset)) * *yNoiseScale);
    
    
    vel += noiseVec;
    vel *=0.8;
    pos += vel;
    if(1.0-(age/lifeTime) <= 0){
        isDead = true;
    }
    
    if(pos.x < 0 || pos.x > ofGetWindowWidth() || pos.y < 0 || pos.y > ofGetWindowHeight()){
        isDead = true;
    }
    
    age++;
    
}

void Particle::draw(){
    ofSetColor(c,100);
    ofCircle(pos.x, pos.y, 1.5*(1-(age/lifeTime)));
    
}
