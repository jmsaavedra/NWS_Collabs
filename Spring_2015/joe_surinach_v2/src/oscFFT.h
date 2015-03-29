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

#define PORT 12348

#define NBANDS 2048


class surinachOscFft {
    
public:
    void setup();
    void update();
    
    //    float * receivedRms;
    float * chan1_fft;
    float * chan2_fft;
    float * chan3_fft;
    float * chan4_fft;
    
    float chan1_fft_bands[NBANDS];
    
    float chan1_amp;
    float chan2_amp;
    float chan3_amp;
    float chan4_amp;
    
    int nBandsToUse;
    
    //private:
    ofxOscReceiver receiver;
    
};