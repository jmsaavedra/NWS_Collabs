/*
 *  Tiles.h
 *  ciel_etoile_background
 *
 *  Created by Julie Huynh on 2/26/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"

class Tiles : public ofBaseApp{
	
public:
	void setup(int _nTiles, int _i, string _mode, int tempX, int tempY);
	void update(Boolean GUImode, string _mode, float mouseX, float mouseY, float freq[], float _threshold, float _hue, ofPoint _average, ofPoint _lastAverage);
	void draw (float mouseX, float mouseY);
    void createTileVertices();
    void createFragmentVertices();
    
    int nTiles;
    int i;
    
    float size;         //tile size
    ofPoint gridPos;    //tile position
    
    //fragments mode
    vector<ofPoint> tileVertices;       //regular tiles vertices
    vector<ofPoint> fragmentVertices;   //fragmented vertices
    vector<ofPoint> currVertices;       //interpolated vertices
    float breakage;                     //fragmentation index
    
    string mode;
    
    ofColor color;
    float hue;
    
    float expansion;    /*This is gonna control the animation for all modes
                          it will be updated based on the frequency array */
    float dir;          //Expansion direction
    float speed;        //Expansion speed
    float threshold;    //Minimum sound input to change expansion
    
};

