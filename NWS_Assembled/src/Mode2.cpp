//
//  Mode2.cpp
//  NWS_Assembled
//
//  Created by Matt on 11/7/13.
//
//

#include "Mode2.h"


//--------------------------------------------------------------
void Mode2::setup(){
	
	//Set Up
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	//ofSetBackgroundAuto(false);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetCircleResolution(3);
    
    // Event Listener
    friction.addListener(this, &Mode2::frictionChanged);
    radius.addListener(this, &Mode2::radiusChanged);
    clearButton.addListener(this, &Mode2::clearButtonPressed);
    resetButton.addListener(this, &Mode2::resetButtonPressed);
    randomVF.addListener(this, &Mode2::randomButtonPressed);
    
    // Gui
    gui.setup();
    gui.add(friction.setup("friction", 0.0015, 0.0, 0.003));
    gui.add(radius.setup("radius", 1.5, 0.0, 3.0));
    gui.add(color.setup("color", ofColor(15,31,63), ofColor(0,0), ofColor(255,255)));
    gui.add(resetButton.setup("reset particles"));
    gui.add(clearButton.setup("clear VecotorField"));
    gui.add(randomVF.setup("randomize VectorFirld"));
	
	//Setting particles in the middle of the window
    
    for (int j=0; j<5; j++) {
        for (int i = 0; i < 10000; i++){
            ParticleN p;
            float angle, length;
            length = ofRandom(3);
            angle = ofRandom(TWO_PI);
            ofVec2f pos;
            pos.x = cos(angle) * length + ofGetWidth()/2 + 200 * cos(TWO_PI*1/5*j);
            pos.y = sin(angle) * length + ofGetHeight()/2 + 200 * sin(TWO_PI*1/5*j);
            p.setup(pos, ofVec2f(0, 0));
            p.radius = 1.2;
            p.friction = friction;
            particles.push_back(p);
        }
		
    }
    
    VF.setupField(400,200,ofGetWidth(), ofGetHeight());
    VF.randomizeField(0.1);
    
    /* moved to testapp
    //FFT
    // 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);
	
	//left = new float[BUFFER_SIZE];
	//right = new float[BUFFER_SIZE];
	
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.8f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	 */
    
    //parmeters initialize
    ave = 0;
    sum = 0;
    alpha = 0;
    blue = 0;
    red = 0;
    
    //FBO
    mFbo.allocate( ofGetWindowWidth(), ofGetWindowHeight() );
	
}

//--------------------------------------------------------------
void Mode2::update(FFTOctaveAnalyzer *FFTanalyzer){
    
    
    ave = 0;
    sum = 0;
    
	for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
		ofVec2f force;
		force = VF.getForceFromPos(particles[i].position.x, particles[i].position.y);
		particles[i].addForce(ofVec2f(force.x, force.y)/4);
		particles[i].updateForce();
        particles[i].bounceOffWalls();
		particles[i].update();
	}
	VF.fadeField(1.0);
    
	// moved to testapp
    /*FFT
    float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	FFTanalyzer.calculate(freq);
	ofSetHexColor(0xffffff);
	for (int i = 0; i < (int)(BUFFER_SIZE/2 - 1); i++){
		//ofRect(200+(i*4),600,4,-freq[i]*10.0f);
	}
	 */
	for (int i = 0; i < FFTanalyzer->nAverages; i++){
		//		ofRect(60+(i*20),600,20,-FFTanalyzer.averages[i] * 6);
        sum += FFTanalyzer->averages[i]*6;
	}
	/*
    ofSetHexColor(0xff0000);
	for (int i = 0; i < FFTanalyzer.nAverages; i++){
		//		ofRect(60+(i*20),600-FFTanalyzer.peaks[i] * 6,20,-4);
	}
	*/
    
    ave = sum / FFTanalyzer->nAverages;
    nradius = ofMap(ave, 0, 50, 0.2, 1.4);
    blue = int(ofMap(FFTanalyzer->averages[3], 0, 20, 0, 255));
    red = int(ofMap(FFTanalyzer->averages[8], 0, 100, 0, 250));
	
}

//--------------------------------------------------------------
void Mode2::draw(){
    
    // GUI
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    //gui.draw();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0,15);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
    ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	ofSetCircleResolution(3);
	
	for (int i = 0; i < particles.size(); i++){
        ofSetColor(red, 29 ,blue, 30);
		particles[i].draw();
        particles[i].radius = nradius;
	}
    
}

// moved to testapp
/*
void Mode2::audioReceived 	(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}
 */

// Event Listener
void Mode2::frictionChanged(float & friction){
    for (int i = 0; i < particles.size(); i++){
        particles[i].friction = friction;
    }
}

void Mode2::radiusChanged(float & radius){
    for (int i = 0; i < particles.size(); i++){
        particles[i].radius = radius;
    }
}

void Mode2::resetButtonPressed(){
    for (int i = 0; i < particles.size(); i++){
        float angle, length;
        length = ofRandom(3);
        angle = ofRandom(PI * 2.0);
        ofVec2f pos;
        pos.x = cos(angle) * length + ofGetWidth()/2;
        pos.y = sin(angle) * length + ofGetHeight()/2;
        particles[i].setup(pos, ofVec2f(0, 0));
        particles[i].friction = friction;
    }
}

void Mode2::clearButtonPressed(){
    VF.clear();
}


void Mode2::randomButtonPressed(){
    VF.randomizeField(0.1);
}


//--------------------------------------------------------------
void Mode2::keyPressed  (int key){
	
	switch (key) {
		case 'c':
			VF.clear();
			break;
			
		case 'R':
			particles.clear();
            for (int j=0; j<5; j++) {
                for (int i = 0; i < 10000; i++){
                    ParticleN p;
                    float angle, length;
                    length = ofRandom(3);
                    angle = ofRandom(TWO_PI);
                    ofVec2f pos;
                    pos.x = cos(angle) * length + ofGetWidth()/2 + 200 * cos(TWO_PI*1/5*j);
                    pos.y = sin(angle) * length + ofGetHeight()/2 + 200 * sin(TWO_PI*1/5*j);
                    p.setup(pos, ofVec2f(0, 0));
                    p.radius = 1.2;
                    p.friction = friction;
                    particles.push_back(p);
                }
                
            }
            VF.randomizeField(0.1);
			break;
            
        case 'r':
            VF.randomizeField(0.1);
            break;
	}
}

//--------------------------------------------------------------
void Mode2::keyReleased  (int key){
}

//--------------------------------------------------------------
void Mode2::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void Mode2::mouseDragged(int x, int y, int button){
	
	float diffx = x - prevMouseX;
	float diffy = y - prevMouseY;
	
	//VF.addVectorCircle((float)x, (float)y, diffx*0.3, diffy*0.3, 10, 0.3);
    VF.addClockwiseCircle(x, y, 100, 0.01);
    //VF.addInwardCircle(x,y, 100, 0.01);
	
	prevMouseX = x;
	prevMouseY = y;
}

//--------------------------------------------------------------
void Mode2::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode2::mouseReleased(){
}
