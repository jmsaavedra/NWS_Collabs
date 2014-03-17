//
//  ModeTwo.h
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#pragma once

#include "ofMain.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#include "ParticleOrbit.h"
#include "movingBackground.h"

class ModeTwo {
public:
    
    void setup( vector<MovingBackground> &bkgListTwo );
    void update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel);
    void draw();

    vector<ParticleOrbit>pList;
    float multiplier;
    
    int width, height;
    
    ofFbo fbo;

};
