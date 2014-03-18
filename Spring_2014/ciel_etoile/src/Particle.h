//
//  Particle.h
//  ciel_etoile_0_1
//
//  Created by Gabriel Gianordoli on 2/26/14.
//
//

#pragma once
#include "ofMain.h"

class Particle{
public:
    //functions
    void init(int _nParticles, int _i, float _shapeSize, float _size, string _shape);
    void update(Boolean GUImode, string _mode, string _shape, float _expansion, float _shapeSize, float _nVertices, float _size, float _rotation, ofPoint _average, ofPoint _lastAverage);

    void draw();

    void createRandomPos();
    void createShapePos();
    
    //variables
    int nParticles;
    int i;
    ofPoint pos;
    ofPoint randomPos;
    ofPoint shapePos;
    ofPoint centerPos;
    ofColor color;
    float size;
    float shapeSize;
    string shape;
    float nVertices;
    float rotation;
};