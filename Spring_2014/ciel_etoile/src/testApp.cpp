#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofBackground(0);
    
    /*------------------ SYPHON ------------------*/
//	syphon.setName("ciel_etoile");
    individualTextureSyphonServer.setName("Texture Output");
	mClient.setup();
    mClient.set("","Simple Server");
    /*--------------------------------------------*/

    
    /*-------------------- FBO -------------------*/
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(0);
    fbo.end();
    /*---------------------------------------------*/
    
    /*------------------- WI-FLY ------------------*/
    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);
    /*---------------------------------------------*/

    /*-------------------- FOG --------------------*/
    //    fogMovie.loadMovie("movies/fog.mov");
    fogMovie.loadMovie("movies/smoke_duck.mp4");
	fogMovie.play();
    //    fogMovie.setLoopState();
    videoAlpha = 100;
    /*---------------------------------------------*/
    
    /*------------------- SOUND -------------------*/
    soundStream.listDevices();
  	soundStream.setDeviceID(2);
    
    //soundstream setup
    soundStream.setup(this, 0, 2, 44100, BUFFER_SIZE, 4);
    
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
    /*---------------------------------------------*/
    
    
    /*----------------- PARTICLES -----------------*/
    modes.push_back("static");
    modes.push_back("walk");
    modes.push_back("fuzz");
    selectedMode = modes[2];
    
    shapes.push_back("circle");
    shapes.push_back("spiral");
    shapes.push_back("star");
    shapes.push_back("grid");
    selectedShape = shapes[0];
    
    expansion = 0;
    rotation = 0;
    shapeSize = ofGetHeight()/2 - 100;
    particleSize = 1;
    nVertices = 1;
    
    particleGUImode = true;
    
    for(int i=0; i < NUM_PARTICLES; i++){
        Particle thisParticle;
        thisParticle.init(NUM_PARTICLES, i, shapeSize, particleSize, selectedShape);
        myParticles.push_back(thisParticle);
    }
    /*---------------------------------------------*/
    
    
    /*---------------- BACKGROUND -----------------*/
	tileModes.push_back("3D");
	tileModes.push_back("fragments");
	tileModes.push_back("rotation");
    selectedTileMode = tileModes[0];
    
    int nTiles = int(ofGetWidth()/40) *  int(ofGetHeight()/40);
    int i = 0;
	for (int gridY=0; gridY <= ofGetHeight(); gridY += 40) {
		for (int gridX=0; gridX <= ofGetWidth(); gridX+= 40) {
			Tiles thisTile;
			thisTile.setup(nTiles, i, selectedTileMode, gridX, gridY);
			myTiles.push_back(thisTile);
            //            cout << gridX << endl;
            i++;
		}
	}
    /*---------------------------------------------*/
    
    setGUI1();
    setGUI2();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    /*------------------- WI-FLY ------------------*/
    updateConnection();
    /*---------------------------------------------*/
    
    
    /*------------------- SOUND -------------------*/
    float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
//        cout << freq[i] << endl;
	}
	FFTanalyzer.calculate(freq);

    /*---------------------------------------------*/

    
    //BACKGROUND
    ofPoint currAverageAccel2 = average(accel2);
	for (int i=0; i < myTiles.size(); i++) {
		myTiles[i].update(tileGUImode, selectedTileMode, mouseX, mouseY, freq, threshold, hue, currAverageAccel2, lastAverageAccel2);
	}
    lastAverageAccel2 = currAverageAccel2;
    
    //VIDEO
    fogMovie.update();
    
    // PARTICLES
    ofPoint currAverageAccel1 = average(accel1);
    for(int i=0; i < myParticles.size(); i++){
        
        myParticles[i].update(particleGUImode, selectedMode, selectedShape, expansion, shapeSize, nVertices, particleSize, rotation, currAverageAccel1, lastAverageAccel1);
    }
    lastAverageAccel1 = currAverageAccel1;
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    fbo.begin();
        ofSetColor(0, 20);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        
        //BACKGROUND
        for (int i = 0; i < myTiles.size(); i++) {
            myTiles[i].draw(mouseX, mouseY);
        }
        
        //VIDEO
        ofSetColor(255, videoAlpha);
        fogMovie.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        //PARTICLES
        for(int i=0; i < myParticles.size(); i++){
            myParticles[i].draw();
        }
    fbo.end();
    
    ofSetColor(255, 255);
    
    ofEnableAlphaBlending();
    fbo.draw(0, 0);
    
//	syphon.publishScreen();
    ofTexture tex;
    tex = fbo.getTextureReference();
    individualTextureSyphonServer.publishTexture(&tex);
}

ofPoint testApp::average(vector<ofVec3f> myVector){
    ofPoint sum;
    for (int i = 0; i < myVector.size(); i++) {
        sum.x += myVector[i].x;
        sum.y += myVector[i].y;
        sum.z += myVector[i].z;
    }
    sum.x /= myVector.size();
    sum.y /= myVector.size();
    sum.z /= myVector.size();
    return sum;
}


void testApp::updateConnection(){
    float aX,aY,aZ,mX,mY,mZ,dataID;
    
	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
        //      cout << "raw msg: ";
        //		cout << message << endl;
        
		vector<string> fullMsg = ofSplitString(message,"/");
        //        cout << "fullMsg size "<<fullMsg.size() << endl;
        //        cout << "fullMsg 0 "<<fullMsg[0] << endl;
        //        cout << "fullMsg 1 "<<fullMsg[1] << endl;
        
        if (fullMsg.size() >= 2) { // received a full, correct message
            
            for(int k=1; k < fullMsg.size(); k++){
                vector<string> dataPoints = ofSplitString(fullMsg[k],"\t");
                //cout << "datapoints size: "<<dataPoints.size() << endl;
                if (dataPoints.size() == 7) {
                    
                    dataID = atof(dataPoints[0].c_str());
                    
                    if(dataID == 1){
                        accel1.push_back(ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                        magne1.push_back(ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));

                        int readingNum = accel1.size();
                        cout << "total readings: "<< readingNum;
                        cout << "\tid: "<< dataID; //WHICH UNIT IS THIS ONE
                        cout << "\tAccel: " << accel1[accel1.size()-1].x << ", " << accel1[accel1.size()-1].y << ", " << accel1[accel1.size()-1].z;
                        cout << "\t\tMagne: " << magne1[magne1.size()-1].x << ", " << magne1[magne1.size()-1].y << ", " << magne1[magne1.size()-1].z << endl;
                        
                        
                    }else if(dataID == 4){
                        accel2.push_back(ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                        magne2.push_back(ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));

                        int readingNum = accel2.size();
                        cout << "total readings: "<< readingNum;
                        cout << "\tid: "<< dataID; //WHICH UNIT IS THIS ONE
                        cout << "\tAccel: " << accel2[accel2.size()-1].x << ", " << accel2[accel2.size()-1].y << ", " << accel2[accel2.size()-1].z;
                        cout << "\t\tMagne: " << magne2[magne2.size()-1].x << ", " << magne2[magne2.size()-1].y << ", " << magne2[magne2.size()-1].z << endl;
                        
                        
                    }else{
                        cout << "This is not my ID!!!" << endl;
                    }
                    
                    
                    
                } else cout << "WRONG NUMBER OF DATAPOINTS RECEIVED: " << dataPoints.size() <<endl;
            }
        } else cout << "WRONG NUMBER OF MSG STRINGS RECEIVED: "<< fullMsg.size() << endl;
	}
}

void testApp::audioIn(float * input, int bufferSize, int nChannels){
    //    cout << "sound in" << endl;
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
        //        cout << right[i] << endl;
    }
}

void testApp::guiEvent(ofxUIEventArgs &e){
	string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(e.getName() == "FULLSCREEN"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        ofSetFullscreen(toggle->getValue());
        
        
        /*----------------- PARTICLES -----------------*/
	}else if(name == "PARTICLE GUI MODE"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        particleGUImode = toggle->getValue();
        
	}else if(name == "EXPANSION"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		expansion = slider->getScaledValue();
        
    }else if(name == "ROTATION"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		rotation = slider->getScaledValue();
        
    }else if(name == "SHAPE SIZE"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		shapeSize = slider->getScaledValue();
        
    }else if(name == "VERTICES"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		nVertices = slider->getScaledValue();
        
    }else if(name == "PARTICLE SIZE"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        particleSize = slider->getScaledValue();
        
    }else if(name == "SHAPES"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
        selectedShape = radio->getActiveName();
        
    }else if(name == "MODES"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
        selectedMode = radio->getActiveName();
        
        
        /*---------------- BACKGROUND -----------------*/

	}else if(name == "TILE GUI MODE"){
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        tileGUImode = toggle->getValue();
        
    }else if(name == "TILE MODES"){
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
        selectedTileMode = radio->getActiveName();
        
    }else if(name == "HUE"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        hue = slider->getScaledValue();

    }else if(name == "VIDEO"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        videoAlpha = slider->getScaledValue();
        
    }else if(name == "THRESHOLD"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        threshold = slider->getScaledValue();
    }
}

void testApp::setGUI1(){
    
    
    gui1 = new ofxUISuperCanvas("PARTICLES");
    gui1->addSpacer();
    
    gui1->addToggle("PARTICLE GUI MODE", particleGUImode);
    gui1->addSpacer();
    
    gui1->addRadio("MODES", modes, OFX_UI_ORIENTATION_HORIZONTAL);
    gui1->addSpacer();

    gui1->addRadio("SHAPES", shapes, OFX_UI_ORIENTATION_HORIZONTAL);
    gui1->addSpacer();

    gui1->addSlider("EXPANSION", 0, 1, expansion);
    gui1->addSpacer();

    gui1->addSlider("SHAPE SIZE", 0, ofGetHeight()/2 - 100, shapeSize);
    gui1->addSpacer();

    gui1->addSlider("VERTICES", 1, 6.78, nVertices);
    gui1->addSpacer();

    gui1->addSlider("PARTICLE SIZE", 1, 50, particleSize);
    gui1->addSpacer();
    
    gui1->addSlider("ROTATION", 0, 360, rotation);
    gui1->addSpacer();

    gui1->addToggle("FULLSCREEN", TRUE);
    
    gui1->autoSizeToFitWidgets();
    ofAddListener(gui1->newGUIEvent,this,&testApp::guiEvent);
    gui1->loadSettings("gui1Settings.xml");
}

void testApp::setGUI2(){
    gui2 = new ofxUISuperCanvas("BACKGROUND");
    gui2->addSpacer();
    
    gui2->addToggle("TILE GUI MODE", tileGUImode);
    gui1->addSpacer();
    
    gui2->addRadio("TILE MODES", tileModes, OFX_UI_ORIENTATION_HORIZONTAL);
    gui2->addSpacer();

    gui2->addSlider("HUE", 120, 160, hue);
    gui2->addSpacer();
    
    gui2->addSlider("VIDEO", 0, 180, videoAlpha);
    gui2->addSpacer();
    
    gui2->addSlider("THRESHOLD", 0, 6, threshold);
    
    gui2->autoSizeToFitWidgets();
    ofAddListener(gui2->newGUIEvent,this,&testApp::guiEvent);
    gui2->loadSettings("gui2Settings.xml");
}

void testApp::exit(){
    gui1->saveSettings("gui1Settings.xml");
    delete gui1;
    gui2->saveSettings("gui2Settings.xml");
    delete gui2;
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key){
        case 'g':{
            gui1->toggleVisible();
        }
            break;
            
        case 't': case 'T':
            bDrawAverages = !bDrawAverages;
            break;
            
        default:
            break;
    }
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
