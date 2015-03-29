//
//  kiyo_circle.h
//  surinach_v2
//
//  Created by performer on 3/29/15.
//
//

#pragma once

#include "ofMain.h"

class kiyoCircle{
    
public:
    void setup();
    void update(float fft1[], float fft2[]);
    void draw();
    void keyPressed(int key);
    
    int fft_size;
    
    float *magnitudeL;
    float *magnitudeR;
};