#include "ofApp.h"

int BandAlpha = 50;		//Band index in spectrum, affect something
int BandSize = 10;		//Band index in spectrum, affect something else

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";
    
//    ofSetFrameRate(100);

	ofBackground(30, 30, 130);
    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }

    /***** YU start here *****/
    
    alpha = 100;
    pixelSize = 10;
    
    theme = 1;
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
    /***** YU start here *****/

    
    ofEnableAlphaBlending();
    img.setAnchorPercent(0.8, 0.8);
    ofBackground(0);
    
    
    if (theme == 1) {
        img.loadImage("1.jpg");
        
    }
    else if (theme == 2) {
        img.loadImage("2.jpg");
        
    }
    else if (theme == 3) {
        img.loadImage("3.jpg");
        
    }
    else if (theme == 4) {
        img.loadImage("4.jpg");
        
    }
    
    w = img.width;
    h = img.height;
    img.allocate(w, h, OF_IMAGE_COLOR);
    
    
    alpha = ofMap(receivedFft[BandAlpha], 0,0.2, 200, 255);
    pixelSize = ofMap(receivedFft[BandSize],0, 0.8, 2, 30);
//    alpha = 80;
//    pixelSize = 3;
}


//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofEnableAlphaBlending();
//    ofSetColor(255,255,255,100);
//    ofRect(25,25,ofGetWidth()-50,225);
//    ofDisableAlphaBlending();
    
    // draw the fft resutls:
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
//    ofDrawBitmapString("listening on PORT "+ofToString(PORT), 25, ofGetHeight()-80);
    
 //   ofDrawBitmapString("THIS IS YOUR APP, DO WHATEVER YOU WANT WITH THIS DATA!!"+ofToString(PORT), 25, ofGetHeight()-50);
    
    /***** YU start here *****/

    
    // get pixel information
    unsigned char * pixels = img.getPixels();
    
    int skip = 10;
    
    for (int i=0; i <w; i= i+skip){
        for( int j=0; j<h; j = j+skip){
            
            //    int r = pixels[j * w * 5 + i * 5];
            //    int g = pixels[j * w * 5 + i * 5 + 100];
            //    int b = pixels[j * w * 5 + i * 5 + 200];
            //
            
            int r = pixels[j * w * 3 + i * 3];
            int g = pixels[j * w * 3 + i * 3 + 1];
            int b = pixels[j * w * 3 + i * 3 + 2];
            
            ofColor sampleColor (r,g,b);
            
            float brightness = sampleColor.getBrightness();
            float pct = 1.0 - brightness / 255.0f;  //get dark value
            
            pct += ofMap(ofNoise(i, j, ofGetFrameNum()*0.01)*receivedFft[BandAlpha]*10, 0, 1, -3, 3);
            
            ofSetColor(r, g, b,alpha);
            
            // ofSetColor(brightness,brightness,brightness, alpha);
            
            ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
            ofRect(i, j,pixelSize*pct,pixelSize*pct);
            
            //ofCircle(i, j,pixelSize*pct);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
            
            
        case '1' :
            theme = 1;
            break;
            
            
        case '2' :
            theme = 2;
            break;
            
            
        case '3' :
            theme = 3;
            break;
            
        case '4' :
            theme = 4;
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
