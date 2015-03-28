#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    setupComplete = false;
    oscSender.setup(HOST,PORT);
 
    ofSoundStreamListDevices();
    
    AudioIn.setDeviceID(2);
    AudioIn.setup(this, 2, NUM_CHANNELS, 44100, BUFFER_SIZE, 4);
    AudioIn.start();

    
    nBandsToGet = 1024;
    
    // the fft needs to be smoothed out, so we create an array of floats
    nBandsToGet = 1024;
    nBandsToUse = nBandsToGet/4;
    
    fftSmoothed = new float[nBandsToGet];
    for (int i = 0; i < nBandsToGet; i++){
        fftSmoothed[i] = 0;
    }
    
    setupComplete = true;
}


//--------------------------------------------------------------
void ofApp::update(){
    
    ofBackground(50);
    
    for(int i=0; i<NUM_CHANNELS; i++){
        scaledVol[i] = ofMap(smoothedVol[i], 0.0, 0.15, 0.0, 1.0, true);
    }
    
    
    float * val = ofSoundGetSpectrum(nBandsToGet);//(nBandsToGet);		// request 128 values for fft
    for (int i = 0;i < nBandsToGet; i++){
        
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.90f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
    }

    sendOsc();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(setupComplete){
        
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,100);
        ofRect(25,25,NUM_CHANNELS*50,225);
        ofDisableAlphaBlending();
        
        // draw the fft resutls:
        ofSetColor(255,255,255,255);
        
        float width = 25;
        for (int i = 0;i < NUM_CHANNELS; i++){
            //cout << i << "\t"<<scaledVol[i] << end;
            ofRect(35+i*width*2,250,width,-(scaledVol[i] * 750)); //multiplying by 750 to make the numbers BIGGER
            ofDrawBitmapString(ofToString(i+1), 45+i*width*2, 265);
        }
        //cout << endl;
        
        ofSetColor(255);
        ofDrawBitmapString("BROADCASTING OSC \nTO PORT: "+ofToString(PORT), 450, 50);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
//    switch (key){
//        case ' ':
//            if(audioFile.getIsPlaying()){
//                audioFile.setPaused(true);
//            } else audioFile.setPaused(false);
//            break;
//            
//        case OF_KEY_RIGHT:
//            audioFile.setPositionMS( audioFile.getPositionMS() + 10000);
//            break;
//            
//        case OF_KEY_LEFT:
//            audioFile.setPositionMS( audioFile.getPositionMS() - 10000);
//            break;
//    }
}

//--------------------------------------------------------------
void ofApp::sendOsc(){
    
    for (int i = 0;i < NUM_CHANNELS; i++){
        ofxOscMessage FFT;
        FFT.setAddress("/channel_01/FFT");
        FFT.addIntArg(i);
        FFT.addFloatArg(scaledVol[i]);
        oscSender.sendMessage(FFT);
    }
}


//--------------------------------------------------------------
void ofApp::audioReceived 	(float * input, int bufferSize, int nChannels){

    // samples are "interleaved"
    for (int i=0; i<NUM_CHANNELS; i++){
        for(int j=0; j<bufferSize; j++){
            channel[i][j] = input[i * nChannels + i];
            volume[i] = channel[i][j] * channel[i][j];
        }
    }
    
    for(int i=0; i<NUM_CHANNELS; i++){
        volume[i] /= (float)bufferSize;
        volume[i] = sqrt(volume[i]);
        smoothedVol[i] *= 0.93;
        smoothedVol[i] += 0.07*volume[i];
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

