//
//  oscReceiver.h
//  surinach_v2
//
//  Created by Joseph Saavedra on 3/28/15.
//
//
#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define PORT 9001

class oscReceiver {
  
    public:
        
        void setup();
        void update();
        
        ofxOscReceiver receiver;
        
        float * receivedFft;
        int nBandsToUse;
        
    private:
        bool test;
        
};