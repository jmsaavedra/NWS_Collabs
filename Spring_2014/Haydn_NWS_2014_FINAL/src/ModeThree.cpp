//
//  ModeThree.cpp
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#include "ModeThree.h"

void ModeThree::setup( vector<MovingBackground> &bkgListThree) {
    
    for (int i = 0; i < 200; i++) {
        LineParticle lp;
        lp.setup();
        pList.push_back(lp);
    }
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    bkgListThree[0].setup("bkg_modeThree_0.png");
    bkgListThree[1].setup("bkg_modeThree_1.png");
    bkgListThree[2].setup("bkg_modeThree_2.png");
    bkgListThree[3].setup("bkg_modeThree_3.png");
    
    fbo.allocate(width, height, GL_RGBA);
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
}

void ModeThree::update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel) {
    
    volume = 0.0;
    
    for (int i = 0; i < _FFTanalyzer.nAverages; i++){
        volume += _FFTanalyzer.averages[i];
    }
    
    volume /= _FFTanalyzer.nAverages;
    volume = ofMap(volume, 0, 14, -80, 80);
    
    ofNoFill();
    
    for (int i = 0; i < pList.size(); i++) {
        pList[i].update();
    }
    
}

void ModeThree::draw() {
    
    ofBackground(0);
    
    fbo.begin();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofFill();
    ofSetColor(0, 30);
    ofRect(0, 0, width, height);
    ofEnableAlphaBlending();
    
    ofSetColor(255);
    ofSetLineWidth(0.5);
    
    for (int i=0; i<pList.size()-1; i++) {
        for (int j=i+1; j<pList.size(); j++) {
            if (ofDist(pList[i].pos.x, pList[i].pos.y, pList[j].pos.x, pList[j].pos.y)< RADIUS + volume) {
                
                ofLine(pList[i].pos.x, pList[i].pos.y, pList[j].pos.x, pList[j].pos.y);
                pList[i].nbConnex++;
                pList[j].nbConnex++;
                
            }
        }
    }
    
    fbo.end();

    fbo.draw(0, 0);
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

}