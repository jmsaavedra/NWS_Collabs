#include "testApp.h"
		
//--------------------------------------------------------------
void testApp::setup(){	 
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);	
	
	//left = new float[BUFFER_SIZE];
	//right = new float[BUFFER_SIZE];

	ofSetHexColor(0x666666);
	
	
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
	ofSetVerticalSync(true);
	ofSetRectMode(OF_RECTMODE_CENTER);

}


//--------------------------------------------------------------
void testApp::update(){
		ofBackground(0);
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
	float avg_power = 0.0f;	
		
	/* do the FFT	*/
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	 
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);
	
	float bandWidth = ofGetWidth() / FFTanalyzer.nAverages;
	
	ofSetHexColor(0xffffff);
	//for (int i = 0; i < (int)(BUFFER_SIZE/2 - 1); i++){
	//ofRect(200+(i*4),600,4,-freq[i]*10.0f);
	//}

	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		
		// Add some amount of spin based on the volume, but decrease it over
		// time by scaling it down 15%
		// make sure it doesn't go below 0
		spin[i] += ofMap(FFTanalyzer.averages[i] * 0.005, 0, 0.2, 0, 8);
		spin[i] *= 0.8;
		spin[i] = fmax(spin[i], 0);
		
		// increase our current angle by the amount of spin
		// wrap around 360 so our angle var doesn't get huge
		theta[i] += spin[i];
		theta[i] = fmod(theta[i], 360);

		ofPushMatrix();
		ofTranslate(bandWidth * i, ofGetHeight() / 2);
		ofRotateZ(theta[i]);
//		ofRect(bandWidth * i, 600, bandWidth, -FFTanalyzer.averages[i] * 6);
		ofRect(0, 0, bandWidth * 2, FFTanalyzer.averages[i]);
		
//		for (int j = 0; j < 4; j ++) {
//			ofPushMatrix();
//			ofTranslate(sin(ofGetElapsedTimef() * (j+1))*50, cos(ofGetElapsedTimef() * (j+1))*50);
//			ofRotateX(ofGetElapsedTimef()*2);
//			ofRect(0, 0, 5, 3);
//			ofPopMatrix();
//		}
		
		ofPopMatrix();

	}
	
	ofSetHexColor(0xff0000);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		//ofRect(bandWidth * i,600-FFTanalyzer.peaks[i] * 6, bandWidth,-4);
	}
		 
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
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
void testApp::mouseReleased(){

}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

