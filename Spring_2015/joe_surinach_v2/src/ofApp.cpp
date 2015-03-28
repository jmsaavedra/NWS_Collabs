#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
    //myOsc.setup();
	receiver.setup(PORT);

	ofBackground(30, 30, 130);
//    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }

    movementNumber = 2;
    
//    kai.setup();
    santi.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    

    //========== OSC METHODS ===============
    //    myOsc.update();
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
    
    //========== MOVEMENT UPDATE METHODS ===============
    switch (movementNumber) {
        case 1:
            kai.update(receivedFft[50]);
            break;
        case 2:
            santi.update(receivedFft);
            break;
        default:
            break;
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    //========== MOVEMENT DRAW METHODS ===============
    switch (movementNumber) {
        case 1:
            kai.draw();
            break;
        case 2:
            santi.draw();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == OF_KEY_LEFT){
        movementNumber++;
        
        if(movementNumber == 2){
            santi.setup();
        }
        
    } else {
        switch (movementNumber){
            case 1:
                kai.keyPressed(key);
                break;
            case 2:
                santi.keyPressed(key);
                break;
            default:
                break;
        }
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
