//
//  ModeFour.h
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOpticalFlowFarneback.h"
#include "Curtain.h"
#include "movingBackground.h"

class ModeFour {
public:
    
    void setup( ofVideoGrabber &vidGrabber, vector<MovingBackground> &bkgListFour );
    void update( ofVideoGrabber &vidGrabber );
    void draw( ofVideoGrabber &vidGrabber );
    
    vector<Curtain> pList;
    vector<ofPolyline> lineList;
    
    ofImage background;
    
    ofxOpticalFlowFarneback flowSolver;
    
    int width, height;
};