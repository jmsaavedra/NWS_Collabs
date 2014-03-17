//
//  ModeTwo.cpp
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#include "ModeTwo.h"

void ModeTwo::setup( vector<MovingBackground> &bkgListTwo ) {
    
    ofEnableAlphaBlending();
    ofSetLineWidth(2.0);
    multiplier = 0.25;
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    bkgListTwo[0].setup("bkg_modeTwo_0.png");
    bkgListTwo[1].setup("bkg_modeTwo_1.png");
    bkgListTwo[2].setup("bkg_modeTwo_2.png");
    bkgListTwo[3].setup("bkg_modeTwo_3.png");
    
    fbo.allocate(width, height, GL_RGBA);
    
    fbo.begin();
    ofClear(0);
    fbo.end();
}

void ModeTwo::update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel) {
    
    for (int i = 0; i < _FFTanalyzer.nAverages; i++){
        
        int limit = _FFTanalyzer.averages[i];
        float barWidth = width / _FFTanalyzer.nAverages;
        if (limit < 0) {
            limit = 0;
        }
        
        for (int j = 0; j < limit * multiplier; j++) {
            ParticleOrbit p;
            float tempLife = limit * ofRandom(1, 3);
            ofVec2f tempPos;
            float tempAngle = ofRandom(360);
            tempPos.set(cos(tempAngle)*tempLife*3, sin(tempAngle)*tempLife*3);
            tempPos += ofGetWindowSize()*0.5;
            p.setup(tempPos, int(tempLife), tempLife * 0.08);
            pList.push_back(p);
        }
        
    }
    
    
    for (vector<ParticleOrbit>::iterator it=pList.begin(); it!=pList.end();) {
        it->update();
        
        if (it->life <= 0) {
            it = pList.erase(it);
        } else {
            it++;
        }
    }
    
}

void ModeTwo::draw() {
    
    ofBackground(0);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    ofSetLineWidth(2.0);
    
    fbo.begin();
    ofSetColor(0, 20);
    ofRect(0, 0, width, height);
    
    ofSetColor(255);
    
    //particle stuffz
    
    glBegin(GL_LINES);
    for (int i = 0; i<pList.size(); i++) {
        pList[i].draw();
    }
    glEnd();

    fbo.end();
    fbo.draw(0, 0);
    
    ofDisableAlphaBlending();
    ofDisableSmoothing();

    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
}