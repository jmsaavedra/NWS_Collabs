//
//  kiyo_circle.cpp
//  surinach_v2
//
//  Created by performer on 3/29/15.
//
//

#include "kiyo_circle.h"

void kiyoCircle::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(128);
    fft_size = 512;
}

//-----------------------------------------------------------
void kiyoCircle::update(float fft1[], float fft2[]){
    
    magnitudeL = fft1;
    magnitudeR = fft2;
}

//-----------------------------------------------------------
void kiyoCircle::draw(){
    
    float w = (float)ofGetWidth()/ (float)fft_size / 2.0f;
    for (int i = 0; i < fft_size; i++) {
        
        
        ofColor col;
        col.setHsb(i * 255.0f / (float)fft_size, 255, 255, 31);
        ofSetColor(col);
        
        ofCircle(ofGetWidth()/2 - w * i*2, ofGetHeight()/2, magnitudeL[i] * ofGetWidth()/100.0);
        ofCircle(ofGetWidth()/2 + w * i*2, ofGetHeight()/2, magnitudeR[i] * ofGetWidth()/100.0);
    }
    
}

//-----------------------------------------------------------
void kiyoCircle::keyPressed(int key){
    
    
}

