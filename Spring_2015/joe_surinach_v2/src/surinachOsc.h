//
//  surinachOsc.h
//  surinach_v2
//
//  Created by Joseph Saavedra on 3/28/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

//#define PORT 9001

class surinachOsc {
    
public:
    void setup(int port);
    void update();
    
    float * receivedRms;
    float * chan1_fft;
    float * chan2_fft;
    int nBandsToUse;
    
private:
        ofxOscReceiver receiver;
    
};