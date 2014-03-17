//
//  ModeThree.h
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#pragma once
#include "ofMain.h"

#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#include "lineParticle.h"
#include "movingBackground.h"



class ModeThree {
public:
    
    void setup( vector<MovingBackground> &bkgListThree );
    void update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel);
    void draw();
    
    vector<LineParticle> pList;
    float volume;
    
    int width, height;
    
    ofFbo fbo;
    
};