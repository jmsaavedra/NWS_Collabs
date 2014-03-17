//
//  ModeOne.h
//  haydn_01_connected
//
//  Created by Bernardo Santos Schorr on 3/5/14.
//
//

#pragma once

#include "ofMain.h"
#include "ParticleEmitter.h"
#include "movingBackground.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#include "sensorData.h"

class ModeOne {
public:
    
    void setup( vector<MovingBackground> &bkgListOne );
    void update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel);
    void draw( vector<MovingBackground> &bkgListOne );
    
    vector <ParticleEmitter> pList;
    
    /****************************************************************
     Using ofVboMesh to draw thousands of images super fast. Check
     the billboard example under examples/gl in OF.
     ****************************************************************/
    ofShader billboardShader;
    ofImage texture;
    ofVboMesh billboards;

    //fbos to draw reflected images
    ofFbo fboOrig, fboOne, fboTwo;
    
    //multiplier to control number of particles emitted
    float multiplier;
    
    //width and height of the window
    int width, height;
    

    
};
