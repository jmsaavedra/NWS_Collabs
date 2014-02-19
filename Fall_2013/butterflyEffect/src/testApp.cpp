#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetVerticalSync(true);
	
	//ofSetRectMode(OF_RECTMODE_CENTER);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    for (int i=0; i<300; i++) {
        Particle p;
        p.setup();
        pList.push_back(p);
    }
	
	fbo1.allocate(ofGetWidth(), ofGetHeight());


}

//--------------------------------------------------------------
void testApp::update(){
    for (int i=0; i<pList.size(); i++) {
        pList[i].update();
    }

}

//--------------------------------------------------------------
void testApp::draw(){
	
	fbo1.begin();
	ofClear(255,255,255, 0);
	
    for (int i=0; i<pList.size(); i++) {
        pList[i].draw();
    }
	
	fbo1.end();
	
	fbo1.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
	
	ofPushMatrix();
	{
		ofTranslate(ofGetWidth()/2, 0);
		ofRotateY(180);
		ofTranslate(-ofGetWidth()/2, 0);
		
		fbo1.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
	}
	ofPopMatrix();

	ofPushMatrix();
	{
		ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
		ofRotateX(180);
		ofRotateY(180);
		ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
		
		fbo1.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
	}
	ofPopMatrix();
	
//	ofPushMatrix();
//	{
//		ofTranslate(ofGecwcwwm tWidth()/2, ofGetHeight()/2);
//		ofRotateY(180);
//		ofTranslate(-ofGetWidth()/2, 0);
//		
//		fbo1.draw(0, 0, ofGetWidth()/2, ofGetHeight()/2);
//	}
//	ofPopMatrix();
	
//	fbo1.draw(0, ofGetHeight()/2, ofGetWidth()/2, ofGetHeight()/2);
//	fbo1.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth()/2, ofGetHeight()/2);
	
	//fbo1.draw(0, 0, ofGetWidth(), ofGetHeight());
	//fbo1.draw(ofGetWidth()/2, ofGetHeight()/2, ofGetWidth(), ofGetHeight());
	

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
