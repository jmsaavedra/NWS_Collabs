#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    setupComplete = false;
    oscSender.setup(HOST,PORT);
    
    crumb.loadSound("crumb.mp3");
    crumb.setVolume(0.7);
    crumb.play();
    crumb.setPositionMS(120000); //starting at 1 min into the song.
    crumb.setLoop(true);
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    nBandsToGet = 2048;
    nBandsToUse = nBandsToGet/4;
    
    fftSmoothed = new float[nBandsToGet];
    for (int i = 0; i < nBandsToGet; i++){
        fftSmoothed[i] = 0;
    }
    
    if(crumb.isLoaded()){
     setupComplete = true;
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    
    ofBackground(50);
    
    // update the sound playing system:
    ofSoundUpdate();
    
    // (5) grab the fft, and put in into a "smoothed" array,
    //		by taking maximums, as peaks and then smoothing downward
    float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
    for (int i = 0;i < nBandsToGet; i++){
        
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.90f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
        
    }
    if(setupComplete) sendOsc();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(setupComplete){
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
            ofRect(25+i*width,250,width,-(fftSmoothed[i] * 200));
            if(i%10 == 0){
                ofDrawBitmapString(ofToString(i), 25+i*width, 265);
            }
        }
        
        string timeStr ="current time  "+ofToString(crumb.getPositionMS()/1000/60)+ ":"+ofToString((crumb.getPositionMS()/1000)%60);
        ofDrawBitmapString(timeStr, 25,ofGetHeight()-50);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    switch (key){
        case ' ':
            if(crumb.getIsPlaying()){
                crumb.setPaused(true);
            } else crumb.setPaused(false);
            break;
            
        case OF_KEY_RIGHT:
            crumb.setPositionMS( crumb.getPositionMS() + 10000);
            break;
            
        case OF_KEY_LEFT:
            crumb.setPositionMS( crumb.getPositionMS() - 10000);
            break;
            
    }
}

//--------------------------------------------------------------
void ofApp::sendOsc(){
    
    for (int i = 0;i < nBandsToUse; i++){
        ofxOscMessage FFT;
        FFT.setAddress("/channel_01/FFT");
        FFT.addIntArg(i);
        FFT.addFloatArg(fftSmoothed[i]);
        oscSender.sendMessage(FFT);
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
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

