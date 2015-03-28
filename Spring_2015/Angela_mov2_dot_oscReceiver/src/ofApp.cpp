#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

    vidPlayer.loadMovie("3_A1.mp4");
    vidPlayer.play();
	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";
    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }
    
    ofSetCircleResolution(80);
//    ofBackground(100, 100, 100);
    ofBackground(0);
//    ofBackground(255, 255, 255);
    ofSetBackgroundAuto(false);
    ofSetLineWidth(0.1);
    
    refBandIndex = 10;
    colorDot.wWidth = ofGetWidth();
    colorDot.wHeight = ofGetHeight();
    bigDot.wWidth = ofGetWidth();
    bigDot.wHeight = ofGetHeight();

}

//--------------------------------------------------------------
void ofApp::update(){
    vidPlayer.update();
	// hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}

	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/channel_01/FFT"){
            int index = m.getArgAsInt32(0);
            float val = m.getArgAsFloat(1);
            receivedFft[index] = val;
            //cout << "recvd: " << index << " :\t"<<val<<endl;
        }
        
        else {
            string msg_string;
            msg_string = m.getAddress();
            cout << "unrecognized message" << msg_string << endl;
        }
	}
    
    colorDot.update();
    bigDot.update();

}


//--------------------------------------------------------------
void ofApp::draw(){
//    ofBackground(0);
//    ofSetColor(255, 170, 220);
//    vidPlayer.draw(0,0,ofGetWidth(),ofGetHeight());
    
//    ofEnableAlphaBlending();
//    ofSetColor(255,255,255,100);
//    ofRect(25,25,ofGetWidth()-50,225);
//    ofDisableAlphaBlending();
//    
//    // draw the fft resutls:
//    ofSetColor(255,255,255,255);
//    
//    float width = 3;
//    for (int i = 0;i < nBandsToUse; i++){
//        //cout << i << "\t"<<fftSmoothed[i] << endl;
//        ofRect(25+i*width,250,width,-(receivedFft[i] * 750));
//        if(i%10 == 0){
//            ofDrawBitmapString(ofToString(i), 25+i*width, 265);
//        }
//    }
////    ofDrawBitmapString("listening on PORT "+ofToString(PORT), 25, ofGetHeight()-80);
//    
//    ofDrawBitmapString("THIS IS YOUR APP, DO WHATEVER YOU WANT WITH THIS DATA!!"+ofToString(PORT), 25, ofGetHeight()-50);
    
    
    colorDot.baseFft = receivedFft[refBandIndex];
    bigDot.baseFft = receivedFft[refBandIndex];

    ofNoFill();
    ofEnableAlphaBlending();
    
    if(colorDot.baseFft <= 0.02) {
        ofDisableAlphaBlending();
        ofSetColor(ofRandom(colorDot.baseFft * 200,220),
                   ofRandom(200, 230),
                   ofRandom(195, 255),
                   255);
    }
    else {
        ofFill();
        ofSetColor(230,
                   ofRandom(colorDot.baseFft * 255, 255)*209/255,
                   ofRandom(colorDot.baseFft * 500, 220)*220/255,
                   ofRandom(30, 60));
    }
    colorDot.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
            case 'f':
            ofToggleFullscreen();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
