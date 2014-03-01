#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    //audio configs
    //this is where you find out which input is soundflower. Check out the console
    soundStream.listDevices();
	
	//if you want to set a different device id
    //this is where you set OF to get SoundFlower as input. In my computer it's 2. Might be different in yours.
	soundStream.setDeviceID(2);
    
    //soundstream setup
    soundStream.setup(this, 0, 2, 44100, BUFFER_SIZE, 4);
    
    //Mindblowing FFT setup
	FFTanalyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	FFTanalyzer.peakHoldTime = 15; // hold longer
	FFTanalyzer.peakDecayRate = 0.95f; // decay slower
	FFTanalyzer.linearEQIntercept = 0.9f; // reduced gain at lowest frequency
	FFTanalyzer.linearEQSlope = 0.01f; // increasing gain at higher frequencies
    
    
    //spacebrew magic configs
    Spacebrew::Config config;
    config.clientName = "Spacebrew-OF iPhone Data";
    config.addSubscribe("acceleration", "ofvec3f");
    config.addSubscribe("rotation", "ofvec3f");
    
    spacebrew.connect( Spacebrew::SPACEBREW_CLOUD, config );
    ofAddListener(spacebrew.onMessageEvent, this, &testApp::onSpacebrewMessage);
    
    //other things
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    sqPos.set (width/2, height/2);
    background.loadImage("tint4.jpg");
    
    //billboard
    billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
    
    if(ofGetGLProgrammableRenderer()){
		billboardShader.load("shadersGL3/Billboard");
	}else{
		billboardShader.load("shadersGL2/Billboard");
	}
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("texture2.png");

    ofEnableBlendMode(OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void testApp::update(){
    
	//FFT wizardry being done
    float avg_power = 0.0f;
	myfft.powerSpectrum(0,(int)BUFFER_SIZE/2, left,BUFFER_SIZE,&magnitude[0],&phase[0],&power[0],&avg_power);
    
	for (int i = 0; i < (int)(BUFFER_SIZE/2); i++){
		freq[i] = magnitude[i];
	}
	
	FFTanalyzer.calculate(freq);
    
    //update the accelerometer square
    sqVel *= 0.98;
    
    //let's also keep track of acceleration as a constant that will define the square size
    sqSizeMult = 10 + (abs(accel.x) + abs(accel.y) + abs(accel.z))*3.0;
    sqSize = sqSize * 0.9 + sqSizeMult * 0.1;
    
    //sqAccel according to accel
    sqAccel.x = accel.x * 0.5;
    sqAccel.y = accel.y * -0.5;

    //update velocity and position of background according to accelerometer
    sqVel += ofVec2f(sqAccel.x, sqAccel.y);
    sqPos += sqVel;
    
    if (sqPos.x > width || sqPos.x < 0) {
        if(sqPos.x < 0) sqPos.x = 0;
        if(sqPos.x > width) sqPos.x = width;
        sqVel.x *= -1;
    }
    
    if (sqPos.y > height || sqPos.y < 0) {
        if(sqPos.y < 0) sqPos.y = 0;
        if(sqPos.y > height) sqPos.y = height;
        sqVel.y *= -1;

    }
    
    
    //just for safety
    accel.set(0.0);
    
    //ofVboMesh wizardry
    int numParticles = pList.size();
    
    billboards.getVertices().resize(numParticles);
	billboards.getColors().resize(numParticles);
	billboards.getNormals().resize(numParticles,ofVec3f(0));
    
	for (int i=0; i<numParticles; i++) {
		billboards.getVertices()[i].set(pList[i].pos);
        billboards.setNormal(i, ofVec3f( pList[i].size, 0, 0));
	}
    
    
    //Particle stuff
    //first we erase things we don't need anymore
    for (vector<Particle>::iterator it=pList.begin(); it!=pList.end();) {
        it->update();
        if (it->life <= 0) {
            it = pList.erase(it);
        } else {
            it++;
        }
    }
    
    //then we update the system for the next loop
    for (int i = 0; i < FFTanalyzer.nAverages; i++){
        
        int limit = FFTanalyzer.averages[i];
        float barWidth = width / FFTanalyzer.nAverages;
        if (limit < 0) {
            limit = 0;
        }
        
        for (int j = 0; j < limit * 0.7; j++) {
            Particle p;
            float tempVel = -1 * limit * ofRandom(0.9, 1.1);
            p.setup(ofVec2f (i*barWidth + ofRandom(0, barWidth), height * ofRandom(0.95, 1.05)), ofVec2f (0, tempVel));
            pList.push_back(p);
        }
        
    }
    


}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    ofSetColor(255);
    
    //more vbo wizardry
    billboardShader.begin();
	
	ofEnablePointSprites(); // not needed for GL3/4
	texture.getTextureReference().bind();
	billboards.draw();
	texture.getTextureReference().unbind();
	ofDisablePointSprites(); // not needed for GL3/4
	
	billboardShader.end();
    
    //drawing the tinting background
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    background.draw(sqPos.x - width, sqPos.y - height);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
    ofDrawBitmapString(ofToString(pList.size()), 10, 30);
    
    //utilities we are not using (but might)
    /*
    //Draw the magnetoscope
    ofPushMatrix();{
    
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofTranslate(200, 200),
        
        ofRotateX(rot.x);
        ofRotateY(rot.y);
        ofRotateZ(rot.z);
        
        ofRect(0, 0, 100, 100);
    
    }ofPopMatrix();
    
    //Draw the accelerometer
    ofRect (sqPos, sqSize, sqSize);
    
    //label everything
    
    ofDrawBitmapString("press 't' to toggle Averages", 30, 30);
    ofDrawBitmapString("I'm the magnetometer", 150, 130);
    ofDrawBitmapString("I'm the accelerometer", sqPos.x, sqPos.y - 20);
    ofDrawBitmapString("I'm the FFT", 200, 620);
    */
    
}

//--------------------------------------------------------------
void testApp::onSpacebrewMessage( Spacebrew::Message & m ){
    if ( m.name == "acceleration"){
        Json::Value  pointJSON;
        Json::Reader reader;
        reader.parse(m.value, pointJSON);
        
        accel.x = pointJSON["x"].asFloat();
        accel.y = pointJSON["y"].asFloat();
        accel.z = pointJSON["z"].asFloat();
        
    }
    
    if ( m.name == "rotation"){
        Json::Value  pointJSON;
        Json::Reader reader;
        reader.parse(m.value, pointJSON);
        
        //cout << m.value << endl;
        
        rot.x = pointJSON["alpha"].asFloat();
        rot.y = pointJSON["beta"].asFloat();
        rot.z = pointJSON["gamma"].asFloat();
        
    }
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
    
    switch (key) {
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
