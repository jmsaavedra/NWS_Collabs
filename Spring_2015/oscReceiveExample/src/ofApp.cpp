#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";

	ofBackground(30, 30, 130);
    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }

}

//--------------------------------------------------------------
void ofApp::update(){

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
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableAlphaBlending();
    ofSetColor(255,255,255,100);
    ofRect(25,25,ofGetWidth()-50,225);
    ofDisableAlphaBlending();
    
    // draw the fft resutls:
    ofSetColor(255,255,255,255);
    
    //float width = (float)(nBandsToGet) / nBandsToGet*4;
    //float width = (float)(nBandsToUse) / nBandsToGet;
    float width = 3;
    for (int i = 0;i < nBandsToUse; i++){
        //cout << i << "\t"<<fftSmoothed[i] << endl;
        ofRect(25+i*width,250,width,-(receivedFft[i] * 200));
        if(i%10 == 0){
            ofDrawBitmapString(ofToString(i), 25+i*width, 265);
        }
    }
    ofDrawBitmapString("listening on PORT "+ofToString(PORT), 25, ofGetHeight()-50);
//	string buf;
//	buf = "listening for osc messages on port" + ofToString(PORT);
//	ofDrawBitmapString(buf, 10, 20);
//    
//    if(receivedImage.getWidth() > 0){
//        ofDrawBitmapString("Image:", 10, 160);
//        receivedImage.draw(10, 180);
//    }
//
//	// draw mouse state
//	buf = "mouse: " + ofToString(mouseX, 4) +  " " + ofToString(mouseY, 4);
//	ofDrawBitmapString(buf, 430, 20);
//	ofDrawBitmapString(mouseButtonState, 580, 20);
//
//	for(int i = 0; i < NUM_MSG_STRINGS; i++){
//		ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
//	}



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
