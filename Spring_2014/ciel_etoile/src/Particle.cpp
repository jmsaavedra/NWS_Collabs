//
//  Particle.cpp
//  ciel_etoile_0_1
//
//  Created by Gabriel Gianordoli on 2/26/14.
//
//

#include "Particle.h"

void Particle::init(int _nParticles, int _i, float _shapeSize, float _size, string _shape){
    nParticles = _nParticles;
    i = _i;
//    color = 0;
//    color.set(255);
    color.set(0);
    size = _size;
//    shapeSize = _shapeSize;
    shapeSize = 100;
    shape = _shape;
    
    centerPos.x = ofGetWidth()/2;
    centerPos.y = ofGetHeight()/2;
    
    createRandomPos();
    createShapePos();
    pos = randomPos;
}

void Particle::update(Boolean GUImode, string _mode, string _shape, float _expansion, float _shapeSize, float _nVertices, float _size, float _rotation, ofPoint _average, ofPoint _lastAverage){
//    cout << mode;
    float expansion = _expansion;


    nVertices = _nVertices;
    shape = _shape;
    string mode = _mode;
    
    if(GUImode){
        size = _size;
        shapeSize = _shapeSize;
        rotation = _rotation;
    }else{

        ofPoint diff = _average - _lastAverage;
        diff.normalize();
        
            
        shapeSize += diff.z * 10;
        shapeSize = ofClamp(shapeSize, ofGetWidth()/20, ofGetWidth()/5);
        
        rotation += diff.y * 10;
        rotation = ofClamp(rotation, 0, 360);

        size -= diff.x * 10;
        size = ofClamp(size, 5, 25);

//        cout << "ACCEL x: " + ofToString(accelAverage.x) << " y: " + ofToString(accelAverage.y) << " z: " + ofToString(accelAverage.z) << endl;
//
//        cout << "MAGNE x: " + ofToString(magneAverage.x) << " y: " + ofToString(magneAverage.y) << " z: " + ofToString(magneAverage.z) << endl;

//        shapeSize = ofMap(average(magne).x, 0, 255, 0, ofGetHeight()/2 - 100);
//        rotation = ofMap(average(magne).z, 0, 255, 0, 0, 360);
        
    }
    
    float alpha = ofMap(size, 5, 25, 255, 10);
    alpha = ofClamp(alpha, 10, 255);
    color.set(0, alpha);
    
    float angle = ofDegToRad(360/float(nParticles));
    
    if(_mode == "fuzz"){
        randomPos.x = cos(angle*i) * (ofRandom(-ofGetHeight()/2, ofGetHeight()/2));
        randomPos.y = sin(angle*i) * (ofRandom(-ofGetHeight()/2, ofGetHeight()/2));
    }else if(_mode == "walk"){
        randomPos.x += ofRandom(-2, 2);
        randomPos.y += ofRandom(-2, 2);
    }
    createShapePos();
    pos.x = ofLerp(randomPos.x, shapePos.x, expansion);
    pos.y = ofLerp(randomPos.y, shapePos.y, expansion);
}

void Particle::draw(){
    ofSetColor(color);
    ofPushMatrix();
        ofTranslate(centerPos.x, centerPos.y);
        ofRotate(rotation);
            ofCircle(pos.x, pos.y, size, size);
    ofPopMatrix();
}

void Particle::createRandomPos(){
    float angle = ofDegToRad(360/float(nParticles));
    randomPos.x = cos(angle*i) * (ofRandom(-ofGetHeight()/2, ofGetHeight()/2));
    randomPos.y = sin(angle*i) * (ofRandom(-ofGetHeight()/2, ofGetHeight()/2));
}

void Particle::createShapePos(){
    float angle = ofDegToRad(360/float(nParticles));
    if(shape == "circle"){
        shapePos.x = cos(angle*i) * shapeSize;
        shapePos.y = sin(angle*i) * shapeSize;

    }else if(shape == "spiral"){
        float radius2 = shapeSize * 1.5;
        float radius1 = shapeSize/5;
        float radius = ofMap(sin(angle * (i % int(nVertices))), -1, 1, radius1, radius2);
        
        shapePos.x = cos(angle * i) * radius;
        shapePos.y = sin(angle * i) * radius;

    }else if(shape == "star"){
        float radius2 = shapeSize;
        float radius1 = shapeSize * 0.75;
        float radius = ofMap(sin(float(i)/nVertices), -1, 1, radius1, radius2);
        
        shapePos.x = cos(angle * i) * radius;
        shapePos.y = sin(angle * i) * radius;
 
    }else if(shape == "grid"){
        int nColumns = sqrt(nParticles);
        float spacing = ((shapeSize * 2) / nColumns);
        
        shapePos.x = -shapeSize + (i % nColumns) * spacing;
        shapePos.y = -shapeSize + int(i / nColumns) * spacing;
    }

}
