//
//  oscFFT.h
//  surinach_v2
//
//  Created by performer on 3/28/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define PORT 12345

class surinachOscFft {
    
public:
    void setup();
    void update();
    
    //    float * receivedRms;
    float * chan1_fft;
    float * chan2_fft;
    int nBandsToUse;
    
    //private:
    ofxOscReceiver receiver;
    
};