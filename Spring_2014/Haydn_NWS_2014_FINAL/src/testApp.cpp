#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    ofEnableAlphaBlending();
    
    /****************************************************************
     Audio Configs, including FFT
     ****************************************************************/
    
    //this is where you find out which input is soundflower
    //(more info at http://cycling74.com/soundflower-landing-page/ ).
    //Check out the console to see which number is Soundflower.
    soundStream.listDevices();
	
    //this is where you set OF to get SoundFlower as input.
    //In my computer it's 3. Might be different in yours.
    //Check the values ouputted in the console by the previous function.
	soundStream.setDeviceID(2);
    
    //soundstream setup
    soundStream.setup(this, 0, 2, 44100, BUFFER_SIZE, 4);
    
    //FFT setup
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
	
    /****************************************************************
     ofVboMesh setup, using it for mode 1.
     ****************************************************************/
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
    
    /****************************************************************
     Setting up the video grabber in a separate function
     ****************************************************************/
    setupGrabber();
    
    /****************************************************************
     Setup for moving overlaying backgrounds
     ****************************************************************/
    multValue = 1.0;
    multiplier = &multValue;
    
    for (int i = 0; i < 4; i++ ) {
        MovingBackground tempBkgOne(multiplier);
        MovingBackground tempBkgTwo(multiplier);
        MovingBackground tempBkgThree(multiplier);
        MovingBackground tempBkgFour(multiplier);
        
        bkgListOne.push_back(tempBkgOne);
        bkgListTwo.push_back(tempBkgTwo);
        bkgListThree.push_back(tempBkgThree);
        bkgListFour.push_back(tempBkgFour);
    }
    
    /****************************************************************
     Setup of the different visual modes
     ****************************************************************/
    modeSelector = 1;
    modeOne.setup( bkgListOne );
    modeTwo.setup( bkgListTwo );
    modeThree.setup( bkgListThree );
    modeFour.setup( vidGrabber, bkgListFour );
    
    /****************************************************************
     Establishing UDP connections and initial values for sensors
     ****************************************************************/
    
    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);
    
    //set things to 0
    accel.set(0.0, 0.0, 0.0);
    magne.set(0.0, 0.0, 0.0);
    
    /****************************************************************
     Outputting texture via Syphon using an fbo.
     ****************************************************************/
    
    individualTextureSyphonServer.setName("Texture Output");
    
	mClient.setup();
    
    //using Syphon app Simple Server, found at http://syphon.v002.info/
    mClient.set("","Simple Server");
    
    //allocating FBO to be output
    fbo.allocate(width, height, GL_RGBA);

    //Clearing FBO from graphics card garbage
    fbo.begin();
    ofClear(0);
    fbo.end();
	
}

//--------------------------------------------------------------
void testApp::update(){
    
    //updating the grabbed video
    vidGrabber.update();
    
    //updating Syphon
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	//updating FFT
    float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);
    
    //Checking current mode and updating it.
    
    if (modeSelector == 1) {
        modeOne.update(FFTanalyzer, accel);
        for (int i = 0; i < sensors.indSensors.size(); i++) {
            bkgListOne[sensors.indSensors[i].individualSensorID-1].update(sensors.indSensors[i].individualSensorID, sensors.indSensors[i].individualAccel, sensors.indSensors[i].individualMagne);
        }
    } else if (modeSelector == 2) {
        modeTwo.update(FFTanalyzer, accel);
        for (int i = 0; i < sensors.indSensors.size(); i++) {
            bkgListTwo[sensors.indSensors[i].individualSensorID-1].update(sensors.indSensors[i].individualSensorID, sensors.indSensors[i].individualAccel, sensors.indSensors[i].individualMagne);
        }
    } else if (modeSelector == 3) {
        modeThree.update(FFTanalyzer, accel);
        for (int i = 0; i < sensors.indSensors.size(); i++) {
            bkgListThree[sensors.indSensors[i].individualSensorID-1].update(sensors.indSensors[i].individualSensorID, sensors.indSensors[i].individualAccel, sensors.indSensors[i].individualMagne);
        }
    } else if (modeSelector == 4) {
        modeFour.update(vidGrabber);
        for (int i = 0; i < sensors.indSensors.size(); i++) {
            bkgListFour[sensors.indSensors[i].individualSensorID-1].update(sensors.indSensors[i].individualSensorID, sensors.indSensors[i].individualAccel, sensors.indSensors[i].individualMagne);
        }
    }
    
    //UDP update
    char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
    sensors.update(message);
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //Checking current mode and drawing to the FBO
    fbo.begin();
    if (modeSelector == 1) {
        modeOne.draw( bkgListOne );
    } else if (modeSelector == 2) {
        modeTwo.draw();
        for (int i = 0; i < bkgListTwo.size(); i++) {
            bkgListTwo[i].draw();
        }
    } else if (modeSelector == 3) {
        modeThree.draw();
        for (int i = 0; i < bkgListThree.size(); i++) {
            bkgListThree[i].draw();
        }
    } else if (modeSelector == 4) {
        //mode four doesn't use moving backgrounds
        modeFour.draw(vidGrabber);
    }
    
    fbo.end();
    
    //make sure tint for the fbo is white.
    ofSetColor(255);
    ofEnableAlphaBlending();
    
    fbo.draw(0, 0);
    
    //publishing the fbo to syphon.
    //Notice that nothing that is drawn outside of the fbo is sent.
    ofTexture tex;
    tex = fbo.getTextureReference();
    individualTextureSyphonServer.publishTexture(&tex);
    
    //draw some useful information to the screen.
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 30, 30);
    ofDrawBitmapString("Press keys 1-4 to change modes. Press UP/DOWN arrows to change mutiplier.", 30, 50);
    ofDrawBitmapString("Mode: " + ofToString(modeSelector), 30, 70);
    ofDrawBitmapString("Accel Mult: " + ofToString(*multiplier), 30, 90);
    
    
    //draw whatever incoming sensor data
    for (int i = 0; i < sensors.indSensors.size(); i++ ) {
        
        string sensorStr = "Sensor" + ofToString(i+1) + ": ";
        sensorStr += "ID: " + ofToString (sensors.indSensors[i].individualSensorID);
        sensorStr += " Accel: " + ofToString (sensors.indSensors[i].individualAccel);
        sensorStr += " Magne: " + ofToString (sensors.indSensors[i].individualMagne);
        ofDrawBitmapString(sensorStr, 30, 90 + i*20);
        
    }
    
    ofSetColor(255);
    
    //if we're in mode 4, draw the video input to the screen
    if (modeSelector == 4) vidGrabber.draw(0, 220);
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    fbo.begin();
    ofClear(0);
    fbo.end();
    
    switch (key) {
            
        case OF_KEY_UP:
            *multiplier += 0.01;
            break;
            
        case OF_KEY_DOWN:
            *multiplier -= 0.01;
            break;
            
        case '1':
            modeSelector = 1;
            break;
            
        case '2':
            modeSelector = 2;
            break;
            
        case '3':
            modeSelector = 3;
            break;
            
        case '4':
            modeSelector = 4;
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::setupGrabber() {
    
	// define size for input video
	videoWidth = 160;
	videoHeight = 120;
	
	// set up video camera
	if (!vidGrabber.isInitialized()) {
		vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
        vidRecorder->listVideoDevices();
		vidGrabber.setGrabber(vidRecorder);
		vidGrabber.initGrabber(videoWidth, videoHeight);
        //13 was the number of the blackmagic during the performance. Verify which number is right for you.
		vidRecorder->setVideoDeviceID(vidRecorder->getVideoDeviceID() + 13);
	}
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
