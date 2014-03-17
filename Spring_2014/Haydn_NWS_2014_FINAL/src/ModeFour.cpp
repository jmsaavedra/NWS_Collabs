//
//  ModeFour.cpp
//  Haydn_NWS_2014_FINAL
//
//  Created by Bernardo Santos Schorr on 3/8/14.
//
//

#include "ModeFour.h"

void ModeFour::setup( ofVideoGrabber &vidGrabber, vector<MovingBackground> &bkgListFour ) {
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    //setting up lines and control points
    int numLines = ofGetWindowWidth() / 10;
    int numBreaks = ofGetWindowHeight() / 8;
    
    for (int i = -15; i <numLines+15; i++){
        
        ofPolyline line;
        lineList.push_back(line);
        
        for (int j = -15; j < numBreaks+15; j++){
            Curtain c;
            c.setup(ofVec2f (i*10, j*8));
            pList.push_back(c);
            lineList[i+15].addVertex(ofVec2f(c.pos.x,c.pos.y));
        }
    }
    
    background.loadImage("tint.jpg");
    
    //setting up flowsolver
    flowSolver.setup(vidGrabber.getWidth(), vidGrabber.getHeight(), 0.5, 3, 10, 1, 7, 1.5, false, false);
    
    bkgListFour[0].setup("bkg_modeFour_0.png");
    bkgListFour[1].setup("bkg_modeFour_1.png");
    bkgListFour[2].setup("bkg_modeFour_2.png");
    bkgListFour[3].setup("bkg_modeFour_3.png");
    
}

void ModeFour::update( ofVideoGrabber &vidGrabber ) {
    
    for (int i = 0; i < lineList.size(); i++) {
        for (int j = 0; j < lineList[i].getVertices().size(); j++) {
            
            int particleIndex = lineList[i].getVertices().size()*i + j;
            pList[particleIndex].update(flowSolver);
            lineList[i].getVertices()[j].set(pList[particleIndex].pos);
            
        }
    }
    
    //optical flow
    if(vidGrabber.isFrameNew()){
        flowSolver.update(vidGrabber);
    }
    
}

void ModeFour::draw( ofVideoGrabber &vidGrabber ) {
    
    ofDisableBlendMode();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofBackground(0);
    ofSetLineWidth(0.75);
    
    for (int i = 0; i < lineList.size(); i++) {
        //lineList[i].draw();
        lineList[i].getSmoothed(10).draw();
    }
    
    
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    background.draw(0, 0, width, height);
    
}