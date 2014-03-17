//
//  ModeOne.cpp
//  haydn_01_connected
//
//  Created by Bernardo Santos Schorr on 3/5/14.
//
//

#include "ModeOne.h"

void ModeOne::setup( vector<MovingBackground> &bkgListOne ) {
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    //loading color backgrounds
    bkgListOne[0].setup("bkg_modeOne_0.png");
    bkgListOne[1].setup("bkg_modeOne_1.png");
    bkgListOne[2].setup("bkg_modeOne_2.png");
    bkgListOne[3].setup("bkg_modeOne_3.png");
    
    //seeting billboard for vbo mesh
    billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
    
    if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard");
	}
    
    texture.loadImage("texture.png");
    
    //allocating fbos
    //one original and two for each resulting color scheme.
    
    fboOrig.allocate(width, height, GL_RGBA);
    fboOrig.begin();
    ofClear(0, 0, 0, 0);
    fboOrig.end();
    
    fboOne.allocate(width, height, GL_RGBA);
    fboOne.begin();
    ofClear(0, 0, 0, 0);
    fboOne.end();
    
    fboTwo.allocate(width, height, GL_RGBA);
    fboTwo.begin();
    ofClear(0, 0, 0, 0);
    fboTwo.end();
    
    multiplier = 0.3;
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
}

void ModeOne::update(FFTOctaveAnalyzer _FFTanalyzer, ofVec3f _accel) {
    
    //Particle stuff
    //first we erase things we don't need anymore
    for (vector<ParticleEmitter>::iterator it=pList.begin(); it!=pList.end();) {
        it->update();
        if (it->life <= 0) {
            it = pList.erase(it);
        } else {
            it++;
        }
    }
    
    //then we emit new particles according to FFT info.
    for (int i = 0; i < _FFTanalyzer.nAverages; i++){
        
        int limit = _FFTanalyzer.averages[i];
        float barWidth = width / _FFTanalyzer.nAverages;
        if (limit < 0) {
            limit = 0;
        }
        
        for (int j = 0; j < limit * multiplier; j++) {
            ParticleEmitter p;
            float tempVel = -1 * limit * ofRandom(0.5, 0.7);
            p.setup(ofVec2f (i*barWidth + ofRandom(0, barWidth), height * ofRandom(0.95, 1.05)), ofVec2f (0, tempVel));
            pList.push_back(p);
        }
    }
    
    //ofVboMesh wizardry to make the drawing run in light speed
    int numParticles = pList.size();
    
    billboards.getVertices().resize(numParticles);
	billboards.getColors().resize(numParticles);
	billboards.getNormals().resize(numParticles,ofVec3f(0));
    
	for (int i=0; i<numParticles; i++) {
		billboards.getVertices()[i].set(pList[i].pos);
        billboards.setNormal(i, ofVec3f( pList[i].size, 0, 0));
	}
    
    
    
    //fbo update
    ofEnableAlphaBlending();
    fboOrig.begin();
    ofFill();
    ofSetColor(0, 0, 0, 100);
    ofRect(0, 0, width, height);
    fboOrig.end();
    
    fboOne.begin();
    ofClear(0);
    fboOne.end();
    
    fboTwo.begin();
    ofClear(0);
    fboTwo.end();
    
}

void ModeOne::draw( vector<MovingBackground> &bkgListOne ) {
    
    ofBackground(0);
    ofSetColor(255);
 
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    //draw the actual particles on the original fbo
    fboOrig.begin();
    
    billboardShader.begin();
	
	ofEnablePointSprites(); // not needed for GL3/4
	texture.getTextureReference().bind();
	billboards.draw();
	texture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	
	billboardShader.end();
    
    fboOrig.end();
    
    //draw particles with backgrounds overlayed in the 2 remaining fbos
    fboOne.begin();
    ofSetColor(255);
    fboOrig.draw(0, 0);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    bkgListOne[0].draw();
    bkgListOne[2].draw();
    fboOne.end();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fboTwo.begin();
    ofSetColor(255);
    fboOrig.draw(0, 0);
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    bkgListOne[1].draw();
    bkgListOne[3].draw();
    fboTwo.end();
    
    ofSetColor(255);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fboOne.draw(0, -height*0.5);
    fboTwo.draw(0, height*1.5, width, -height);
    
}