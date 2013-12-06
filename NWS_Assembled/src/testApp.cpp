/*
 
 HANDS
 
 A project by Matt Felsen, Jia Xu, and Norihito Yuki
 Expanded Cinema & Sound
 Parsons MFA Design+Technology
 Fall 2013
 
 Developed for a performance of the New World Symphony in Miami Beach
 
 Code for particle & vector field classes taken from...
 Zach Lieberman's Algorithmic Animation class, Fall 2012
 Charlie Whitney's Algorithmic Animation class, Fall 2013
 
 Also making use of the following ofx addons:
 ofxOpenCv
 ofxOpticalFlowFarneback
 ofxUI
 ofxXmlSettings
 ofxSyphon
 
 Many thanks to the authors of all the above code for their contributions
 which made this project possible!
 
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(0);
	
    ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// set up all the different modes
	mode1.setup();
	mode2.setup();
	mode3.setup();
	
	useNori = true;
	appRunning = false;
	
	// set up fbos for each mode
	fbo1.allocate(ofGetWidth(), ofGetHeight());
	fbo2.allocate(ofGetWidth(), ofGetHeight());
	fbo3.allocate(ofGetWidth(), ofGetHeight());
	
	fbo2.begin();
	ofClear(255, 255, 255, 0);
	fbo2.end();

	// set up common FFT stuff

    // 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	//ofSoundStreamSetup(0, 2, this, 44100, BUFFER_SIZE, 4);
	ofSoundStreamSetup(2, 4, this, 44100, BUFFER_SIZE, 4);
	
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.8f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies

	// set up syphon server
	syphon.setName("hands");

}

//--------------------------------------------------------------
void testApp::update(){
	
	if (!appRunning) return;
	
    /*FFT*/
	float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	FFTanalyzer.calculate(freq);
	
	mode1.update(&FFTanalyzer);
	
	if (useNori) {
		mode2.update(&FFTanalyzer);
	} else {
		mode3.update(&FFTanalyzer);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	fbo1.begin();
	//ofClear(255, 255, 255, 0);
	//ofClear(0, 20);
	ofClear(255, 0);
	mode1.draw();
	fbo1.end();
	ofSetColor(255);
	fbo1.draw(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
	//fbo1.draw(0, 0);
	//mode1.draw();
	
	if (useNori) {
		
		fbo2.begin();
		//ofClear(0, 225);
		mode2.draw();
		fbo2.end();
		ofSetColor(255);
		fbo2.draw(0, 0, ofGetWidth()/2, ofGetHeight());
		//fbo2.draw(0, 0);
		//mode2.draw();
		
	} else {
		
		fbo3.begin();
		ofClear(0, 225);
		mode3.draw();
		fbo3.end();
		ofSetColor(255);
		fbo3.draw(0, 0, ofGetWidth()/2, ofGetHeight());
		//fbo3.draw(0, 0);
		//mode3.draw();
		
	}
	
	// publish it to syphon finally
	syphon.publishScreen();
	
}

void testApp::audioReceived(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*4+1];
		right[i] = input[i*4+2];
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == ' ') {
		appRunning = !appRunning;
	}
	
	mode1.keyPressed(key);
	mode2.keyPressed(key);
	
	if (key == '9') {
		useNori = true;
	}
	if (key == '0') {
		useNori = false;
	}

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