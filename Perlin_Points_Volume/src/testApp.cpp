#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //fft
    ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);
    FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies

    
    
    ofBackground(0);
    ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    
    xNoiseValue=0.0025;
    yNoiseValue=0.0025;
    zNoiseValue=0.001;
    xNoiseScale=10;
    yNoiseScale=10;
    xAngleOffset=0;
    yAngleOffset=0;


    
    setupGUI();


}

void testApp::setupGUI(){
    gui = new ofxUICanvas();
    gui -> addLabel("Control Panel");
    gui -> addSpacer();
    gui -> addFPSSlider("FPS");
    gui -> addSpacer();
    
    gui -> addLabel("Noise Values");
    gui -> addSlider("x", 0.0001, 0.1, &xNoiseValue);
    gui -> addSlider("y", 0.0001, 0.1, &yNoiseValue);
    gui -> addSlider("z", 0.0001, 0.1, &zNoiseValue);
    gui -> addSpacer();
    
    gui -> addLabel("Noise Scaling");
    gui -> addSlider("x", 1, 20, &xNoiseScale);
    gui -> addSlider("y", 1, 20, &yNoiseScale);
    gui -> addSpacer();
    
    gui -> addLabel("Angle Offset");
    gui -> addSlider("x", 0, TWO_PI, &xAngleOffset);
    gui -> addSlider("y", 0, TWO_PI, &yAngleOffset);
    gui ->addSpacer();
    
    gui ->autoSizeToFitWidgets();
    
}

//--------------------------------------------------------------
void testApp::update(){
    pos.x=ofRandom(ofGetWindowWidth());
    pos.y=ofRandom(ofGetWindowHeight());
    vel=ofRandom(5,7.5);
    hue=ofRandom(255);

    for ( vector<Particle>::iterator it = pList.begin(); it!=pList.end(); ) {
        
                it->update(1.5 * *FFTanalyzer.peaks); //adjust 1.5 based on overall volume level
        
        
        if ( it->isDead==TRUE ) {
            it = pList.erase(it);
        }
        else {
            it++;
        }
        }
    cout<< *FFTanalyzer.peaks<<endl;
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 25);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    float avg_power = 0.0f;
    
	/* do the FFT	*/
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);
    
	
		


    for ( vector<Particle>::iterator it=pList.begin(); it!=pList.end(); ) {
        it->draw();
        it++;
    }
    int countSize=pList.size();
    
    if (countSize<2500) {
        
    
        for (int i=0; i<50; i++) {
        
            addParticle(pos, vel,hue);
        
        }
    }
    


}

void testApp::addParticle(ofVec2f pos, float vel,float hue){
    Particle tmp;
    tmp.setup(pos, vel,hue);
    
    tmp.passValues(&xNoiseValue, &yNoiseValue, &zNoiseValue, &xNoiseScale, &yNoiseScale, &xAngleOffset, &yAngleOffset);
    
    pList.push_back( tmp );
}

void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
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
