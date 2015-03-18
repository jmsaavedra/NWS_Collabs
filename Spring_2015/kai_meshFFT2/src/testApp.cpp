#include "testApp.h"

//for mesh
//****************************************
//****************************************
int W = 200;		//Grid size
int H = 200;
//****************************************
//****************************************

//--------------------------------------------------------------
void testApp::setup(){
    //for mesh
    //****************************************
    //****************************************
    //Set up vertices and colors
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            mesh.addVertex(
                           ofPoint( (x - W/2) * 12*4, (y - H/2) * 12*4, 0 ) );
            mesh.addColor( ofColor( 255, 255, 255 ) );
        }
    }
    //Set up triangles' indices
    for (int y=0; y<H-1; y++) {
        for (int x=0; x<W-1; x++) {
            int i1 = x + W * y;
            int i2 = x+1 + W * y;
            int i3 = x + W * (y+1);
            int i4 = x+1 + W * (y+1);
            mesh.addTriangle( i1, i2, i3 );
            mesh.addTriangle( i3, i1, i2 );
        }
    }
    setNormals( mesh );		//Set normals
    light.enable();			//Enable lighting
    //****************************************
    //****************************************
    
    
    Sender.setup(HOST, PORT);
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    setupFailed = false;
    
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
    ofSoundStreamListDevices();
    
//	cout<< "devices: " <<  ofSoundStreamListDevices() << endl;
    
	
	//AudioIn.setDeviceID(2); // !!! watch print for list of devices !!!
    if (NCHAN > 3){
        if ( ! AudioIn.setup(this, 2, 4, 44100, BUFFER_SIZE, 4)){
            setupFailed = true;
        }
    }
    else {
        AudioIn.setup(this, 2, 2, 44100, BUFFER_SIZE, 2);
    }
    AudioIn.start();
    
    
	peakTime =15; // hold longer
    decayRate = 0.95f; // decay slower
    HiGain = 0.9f; // reduced gain at lowest frequency
    LowGain = 0.01f; // increasing gain at higher frequencies
    
    avg_power  = 0.0f;
    
	Channel01_Analyzer.setup(44100, BUFFER_SIZE/2, 2);
    Channel02_Analyzer.setup(44100, BUFFER_SIZE/2, 2);
    Channel03_Analyzer.setup(44100, BUFFER_SIZE/2, 2);
    Channel04_Analyzer.setup(44100, BUFFER_SIZE/2, 2);
	
	Channel01_Analyzer.peakHoldTime = peakTime;
	Channel01_Analyzer.peakDecayRate = decayRate;
	Channel01_Analyzer.linearEQIntercept = HiGain;
	Channel01_Analyzer.linearEQSlope = LowGain;
    
    Channel02_Analyzer.peakHoldTime = peakTime;
	Channel02_Analyzer.peakDecayRate = decayRate;
	Channel02_Analyzer.linearEQIntercept = HiGain;
	Channel02_Analyzer.linearEQSlope = LowGain;
    
    Channel03_Analyzer.peakHoldTime = peakTime;
	Channel03_Analyzer.peakDecayRate = decayRate;
	Channel03_Analyzer.linearEQIntercept = HiGain;
	Channel03_Analyzer.linearEQSlope = LowGain;
    
    Channel04_Analyzer.peakHoldTime = peakTime;
	Channel04_Analyzer.peakDecayRate = decayRate;
	Channel04_Analyzer.linearEQIntercept = HiGain;
	Channel04_Analyzer.linearEQSlope = LowGain;
	
    Channel01_Aubio.setup();
    Channel02_Aubio.setup();
    Channel03_Aubio.setup();
    Channel04_Aubio.setup();
    
	ofSetVerticalSync(true);
    
    for (int i = 0; i < Channel01_Analyzer.nAverages; i++){
        float tmp;
        Channel01_holdFreq.push_back(tmp);
        Channel01_deltaFreq.push_back(tmp);
	}
    
    for (int i = 0; i < Channel02_Analyzer.nAverages; i++){
        float tmp;
        Channel02_holdFreq.push_back(tmp);
        Channel02_deltaFreq.push_back(tmp);
	}
    
    for (int i = 0; i < Channel03_Analyzer.nAverages; i++){
        float tmp;
        Channel03_holdFreq.push_back(tmp);
        Channel03_deltaFreq.push_back(tmp);
	}
    
    for (int i = 0; i < Channel04_Analyzer.nAverages; i++){
        float tmp;
        Channel04_holdFreq.push_back(tmp);
        Channel04_deltaFreq.push_back(tmp);
	}

}


//--------------------------------------------------------------
void testApp::update(){
    //for mesh
    //****************************************
    //****************************************
    float time = ofGetElapsedTimef();	//Get time
    //Change vertices
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            int i = x + W * y;			//Vertex index
            ofPoint p = mesh.getVertex( i );
            
            //Get Perlin noise value
            float value =
            ofNoise( x * 0.1, y * 0.1, time * 1 );
            
            //Change z-coordinate of vertex
            p.z = value * 50* keyValue;
            //            p.x = x+100 * smoothedVol;
            mesh.setVertex( i, p );
            
            //Change color of vertex
            mesh.setColor( i,
                          //                          ofColor( value*255, value * 255, 255 ) );
                          ofColor( 255, 255, 0, 100 ) );
        }
    }
    setNormals( mesh );	//Update the normals
    //****************************************
    //****************************************
    
    
    if(!setupFailed){
        audioAnalisys();
        SentMessages();
        
        Channel01_FFT_size = Channel01_Analyzer.nAverages;
        Channel02_FFT_size = Channel02_Analyzer.nAverages;
        Channel03_FFT_size = Channel03_Analyzer.nAverages;
        Channel04_FFT_size = Channel04_Analyzer.nAverages;
        
        
        Channel01_Pitch = Channel01_UsefulPitch;
        Channel01_Attack = Channel01_att;
        Channel01_Amplitude = Channel01_Aubio.amplitude;
        
        Channel02_Pitch= Channel02_UsefulPitch;
        Channel02_Attack= Channel02_att;
        Channel02_Amplitude= Channel02_Aubio.amplitude;
        
        Channel03_Pitch= Channel03_UsefulPitch;
        Channel03_Attack= Channel03_att;
        Channel03_Amplitude= Channel03_Aubio.amplitude;
        
        Channel04_Pitch= Channel04_UsefulPitch;
        Channel04_Attack= Channel04_att;
        Channel04_Amplitude= Channel04_Aubio.amplitude;
        
        
        Channel01_LinearPitch = 69 + 12*log2f(Channel01_Pitch/440);
        Channel02_LinearPitch = 69 + 12*log2f(Channel02_Pitch/440);
        Channel03_LinearPitch = 69 + 12*log2f(Channel03_Pitch/440);
        Channel04_LinearPitch = 69 + 12*log2f(Channel04_Pitch/440);
    }
    
}
//--------------------------------------------------------------
void testApp::draw(){
    //for mesh
    //****************************************
    //****************************************
    ofEnableDepthTest();				//Enable z-buffering
    
    //Set a gradient background from white to gray
    ofBackgroundGradient( ofColor( 20 ), ofColor( 10 ) );
    
    
    easyCam.begin();
    
    
    ofPushMatrix();						//Store the coordinate system
    
    //Move the coordinate center to screen's center
    ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );
    
    //Calculate the rotation angle
    float time = ofGetElapsedTimef();   //Get time in seconds
    float angle = time * 20;			//Compute angle. We rotate at speed
    //20 degrees per second
    //    ofRotate( smoothedVol*PI, 1, 0, 0 );			//Rotate coordinate system
    ofRotate( angle, 0, 0, 1 );
    
    //Draw mesh
    //Here ofSetColor() does not affects the result of drawing,
    //because the mesh has its own vertices' colors
//    mesh.draw();
    mesh.drawWireframe();
    
    ofPopMatrix();      //Restore the coordinate system
    
    
    easyCam.end();
    //****************************************
    //****************************************
    
    
    float curVol = 0.0;
    if(!setupFailed){
        //debug view
        ofPushStyle();
            ofPushMatrix();
                ofTranslate(0, ofGetWindowHeight()-130);
        
                //-----  channel 01 -----
                ofPushMatrix();
                    ofTranslate(100, 0);
//                    ofFill();
//                    ofDrawBitmapString("Channel01", -90,0);
//                    ofSetColor(255,40);
//                    ofDrawBitmapString("FFT: "+ ofToString(Channel01_FFT_size) + " freq", 0,15);
//                    
//                    ofDrawBitmapString("Frequency: " + ofToString(Channel01_Pitch,0), 0,30);
//                    ofDrawBitmapString("Linear Pitch: " + ofToString(Channel01_LinearPitch,0), 0,45);
//                    ofDrawBitmapString("Amplitude: " + ofToString(Channel01_Amplitude,4), 0,60);
//                    ofDrawBitmapString("Attack: " + ofToString(Channel01_Attack,4), 0,75);
                    for (int i = 0; i < Channel01_FFT_size; i++){
                        ofSetColor(255,20);
//                        curVol=Channel01_Analyzer.averages[i]*10;
                        curVol=Channel01_Attack;//for volume
                        
                        ofRect((i*6),0,5,-Channel01_Analyzer.averages[i] * 3);
                        ofSetColor(255);
                        ofLine((i*6), -Channel01_Analyzer.averages[i] * 3, ((i+1)*6), -Channel01_Analyzer.averages[i+1] * 3);
                    }
                    ofSetColor(255, 255*Channel01_Amplitude);
                    ofCircle(-50,50, Channel01_Attack*3);
                    ofSetColor(255);
                    ofNoFill();
                    ofCircle(-50, 50, Channel01_Attack*3);
                    keyValue = curVol;
        
                ofPopMatrix();
        
                //-----  channel 02 -----
                ofPushMatrix();
                    ofTranslate(350, 0);
                    ofFill();
                    ofDrawBitmapString("Channel02", -90,0);
                    ofSetColor(255,40);
                    ofDrawBitmapString("FFT: "+ ofToString(Channel02_FFT_size) + " freq", 0,15);
                    ofDrawBitmapString("Frequency: " + ofToString(Channel02_Pitch,0), 0,30);
                    ofDrawBitmapString("Linear Pitch: " + ofToString(Channel02_LinearPitch,0), 0,45);
                    ofDrawBitmapString("Amplitude: " + ofToString(Channel02_Amplitude,4), 0,60);
                    ofDrawBitmapString("Attack: " + ofToString(Channel02_Attack,4), 0,75);
                    for (int i = 0; i < Channel02_FFT_size; i++){
                        ofSetColor(255,20);
                        ofRect((i*6),0,5,-Channel02_Analyzer.averages[i] * 3);
                        ofSetColor(255);
                        ofLine((i*6), -Channel02_Analyzer.averages[i] * 3, ((i+1)*6), -Channel02_Analyzer.averages[i+1] * 3);
                    }
                    ofSetColor(255, 255*Channel02_Amplitude);
                    ofCircle(-50,50, Channel02_Attack*3);
                    ofSetColor(255);
                    ofNoFill();
                    ofCircle(-50, 50, Channel02_Attack*3);
                ofPopMatrix();
     
                //-----  channel 03 -----
                ofPushMatrix();
                    ofTranslate(600, 0);
                    ofFill();
                    ofDrawBitmapString("Channel03", -90,0);
                    ofSetColor(255,40);
                    ofDrawBitmapString("FFT: "+ ofToString(Channel03_FFT_size) + " freq", 0,15);
                    ofDrawBitmapString("Frequency: " + ofToString(Channel03_Pitch,0), 0,30);
                    ofDrawBitmapString("Linear Pitch: " + ofToString(Channel03_LinearPitch,0), 0,45);
                    ofDrawBitmapString("Amplitude: " + ofToString(Channel03_Amplitude,4), 0,60);
                    ofDrawBitmapString("Attack: " + ofToString(Channel03_Attack,4), 0,75);
                    for (int i = 0; i < Channel03_FFT_size; i++){
                        ofSetColor(255,20);
                        ofRect((i*6),0,5,-Channel03_Analyzer.averages[i] * 3);
                        ofSetColor(255);
                        ofLine((i*6), -Channel03_Analyzer.averages[i] * 3, ((i+1)*6), -Channel03_Analyzer.averages[i+1] * 3);
                    }
                    ofSetColor(255, 255*Channel03_Amplitude);
                    ofCircle(-50,50, Channel03_Attack*3);
                    ofSetColor(255);
                    ofNoFill();
                    ofCircle(-50, 50, Channel03_Attack*3);
                ofPopMatrix();
                
                
                //-----  channel 04 -----
                ofPushMatrix();
                    ofTranslate(850, 0);
                    ofFill();
                    ofDrawBitmapString("Channel04", -90,0);
                    ofSetColor(255,40);
                    ofDrawBitmapString("FFT: "+ ofToString(Channel04_FFT_size) + " freq", 0,15);
                    ofDrawBitmapString("Frequency: " + ofToString(Channel04_Pitch,0), 0,30);
                    ofDrawBitmapString("Linear Pitch: " + ofToString(Channel04_LinearPitch,0), 0,45);
                    ofDrawBitmapString("Amplitude: " + ofToString(Channel04_Amplitude,4), 0,60);
                    ofDrawBitmapString("Attack: " + ofToString(Channel04_Attack,4), 0,75);
                    for (int i = 0; i < Channel04_FFT_size; i++){
                        ofSetColor(255,20);
                        ofRect((i*6),0,5,-Channel04_Analyzer.averages[i] * 3);
                        ofSetColor(255);
                        ofLine((i*6), -Channel04_Analyzer.averages[i] * 3, ((i+1)*6), -Channel04_Analyzer.averages[i+1] * 3);
                    }
                    ofSetColor(255, 255*Channel04_Amplitude);
                    ofCircle(-50,50, Channel04_Attack*3);
                    ofSetColor(255);
                    ofNoFill();
                    ofCircle(-50, 50, Channel04_Attack*3);
                ofPopMatrix();
        
        
            ofPopMatrix();
        ofPopStyle();
    } else { //setup failed!
    
        ofBackground(255, 20, 0);
        ofSetColor(255);
        ofDrawBitmapString(" WRONG DEVICE SELECTED ", ofGetWidth()/2-90, ofGetHeight()/2 - 50);
        ofDrawBitmapString(" CHANGE SYSTEM SETTINGS ", ofGetWidth()/2-90, ofGetHeight()/2);
        ofDrawBitmapString(" TO 4-CHANNEL INTERFACE ", ofGetWidth()/2-90, ofGetHeight()/2+50);
    }
}
//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		Channel01[i] = input[i * nChannels];
		Channel02[i] = input[i * nChannels + 1];
        Channel03[i] = input[i * nChannels + 2];
        Channel04[i] = input[i * nChannels + 3];
	}
    
    Channel01_Aubio.processAudio(Channel01, BUFFER_SIZE);
    Channel02_Aubio.processAudio(Channel02, BUFFER_SIZE);
    Channel03_Aubio.processAudio(Channel03, BUFFER_SIZE);
    Channel04_Aubio.processAudio(Channel04, BUFFER_SIZE);
}

void testApp::audioAnalisys(){
    
    /* do the FFT on Channel 1  */
    Channel01_fft.powerSpectrum(2, (int) BUFFER_SIZE / 2, Channel01, BUFFER_SIZE, & Channel01_magnitude[0], & Channel01_phase[0], & Channel01_power[0], & avg_power);
    for (int i = 0; i < (int)(BUFFER_SIZE / 2); i++) {
        Channel01_freq[i] = Channel01_magnitude[i];
    }
    Channel01_Analyzer.calculate(Channel01_freq);
    
    /* do the FFT on Channel 2  */
    Channel02_fft.powerSpectrum(0, (int) BUFFER_SIZE / 2, Channel02, BUFFER_SIZE, & Channel02_magnitude[0], & Channel02_phase[0], & Channel02_power[0], & avg_power);
    for (int i = 0; i < (int)(BUFFER_SIZE / 2); i++) {
        Channel02_freq[i] = Channel02_magnitude[i];
    }
    Channel02_Analyzer.calculate(Channel02_freq);
    
    /* do the FFT on Channel 3  */
    Channel03_fft.powerSpectrum(0, (int) BUFFER_SIZE / 2, Channel03, BUFFER_SIZE, & Channel03_magnitude[0], & Channel03_phase[0], & Channel03_power[0], & avg_power);
    for (int i = 0; i < (int)(BUFFER_SIZE / 2); i++) {
        Channel03_freq[i] = Channel03_magnitude[i];
    }
    Channel03_Analyzer.calculate(Channel03_freq);
    
    
    /* do the FFT on Channel 4  */
    Channel04_fft.powerSpectrum(0, (int) BUFFER_SIZE / 2, Channel04, BUFFER_SIZE, & Channel04_magnitude[0], & Channel04_phase[0], & Channel04_power[0], & avg_power);
    for (int i = 0; i < (int)(BUFFER_SIZE / 2); i++) {
        Channel04_freq[i] = Channel04_magnitude[i];
    }
    Channel04_Analyzer.calculate(Channel04_freq);
    
    
    /* init attack detection on Channel 01*/
    float tmp01 = 0;
    for (int i = 0; i < Channel01_Analyzer.nAverages; i++) {
        if (Channel01_Analyzer.averages[i] > Channel01_holdFreq[i]) {
            Channel01_deltaFreq[i] = Channel01_Analyzer.averages[i] - Channel01_holdFreq[i];
        }
        Channel01_holdFreq[i] = Channel01_Analyzer.averages[i];
        tmp01 = tmp01 + Channel01_deltaFreq[i];
    }
    tmp01 = tmp01 / Channel01_Analyzer.nAverages;
    Channel01_att = tmp01 * 0.5 + Channel01_att * 0.5;
    if (Channel01_peakPitch < Channel01_Aubio.pitch && Channel01_Aubio.confidence > 0.5) {
        Channel01_peakPitch = Channel01_Aubio.pitch;
    }
    Channel01_UsefulPitch = Channel01_Aubio.pitch * Channel01_Aubio.confidence + Channel01_UsefulPitch * (1 - Channel01_Aubio.confidence);
    
    
    /* init attack detection on Channel 02*/
    float tmp02 = 0;
    for (int i = 0; i < Channel02_Analyzer.nAverages; i++) {
        if (Channel02_Analyzer.averages[i] > Channel02_holdFreq[i]) {
            Channel02_deltaFreq[i] = Channel02_Analyzer.averages[i] - Channel02_holdFreq[i];
        }
        Channel02_holdFreq[i] = Channel02_Analyzer.averages[i];
        tmp02 = tmp02 + Channel02_deltaFreq[i];
    }
    tmp02 = tmp02 / Channel02_Analyzer.nAverages;
    Channel02_att = tmp02 * 0.5 + Channel02_att * 0.5;
    if (Channel02_peakPitch < Channel02_Aubio.pitch && Channel02_Aubio.confidence > 0.5) {
        Channel02_peakPitch = Channel02_Aubio.pitch;
    }
    Channel02_UsefulPitch = Channel02_Aubio.pitch * Channel02_Aubio.confidence + Channel02_UsefulPitch * (1 - Channel02_Aubio.confidence);
    
    
    /* init attack detection on Channel 03*/
    float tmp03 = 0;
    for (int i = 0; i < Channel03_Analyzer.nAverages; i++) {
        if (Channel03_Analyzer.averages[i] > Channel03_holdFreq[i]) {
            Channel03_deltaFreq[i] = Channel03_Analyzer.averages[i] - Channel03_holdFreq[i];
        }
        Channel03_holdFreq[i] = Channel03_Analyzer.averages[i];
        tmp03 = tmp03 + Channel03_deltaFreq[i];
    }
    tmp03 = tmp03 / Channel03_Analyzer.nAverages;
    Channel03_att = tmp03 * 0.5 + Channel03_att * 0.5;
    if (Channel03_peakPitch < Channel03_Aubio.pitch && Channel03_Aubio.confidence > 0.5) {
        Channel03_peakPitch = Channel03_Aubio.pitch;
    }
    Channel03_UsefulPitch = Channel03_Aubio.pitch * Channel03_Aubio.confidence + Channel03_UsefulPitch * (1 - Channel03_Aubio.confidence);
    
    
    /* init attack detection on Channel 04*/
    float tmp04 = 0;
    for (int i = 0; i < Channel04_Analyzer.nAverages; i++) {
        if (Channel04_Analyzer.averages[i] > Channel04_holdFreq[i]) {
            Channel04_deltaFreq[i] = Channel04_Analyzer.averages[i] - Channel04_holdFreq[i];
        }
        Channel04_holdFreq[i] = Channel04_Analyzer.averages[i];
        tmp04 = tmp04 + Channel04_deltaFreq[i];
    }
    tmp04 = tmp04 / Channel04_Analyzer.nAverages;
    Channel04_att = tmp04 * 0.5 + Channel04_att * 0.5;
    if (Channel04_peakPitch < Channel04_Aubio.pitch && Channel04_Aubio.confidence > 0.5) {
        Channel04_peakPitch = Channel04_Aubio.pitch;
    }
    Channel04_UsefulPitch = Channel04_Aubio.pitch * Channel04_Aubio.confidence + Channel04_UsefulPitch * (1 - Channel04_Aubio.confidence);
    
}

//-------------------------------------------------------
void testApp::SentMessages(){
    
    
    ofxOscMessage Channel01;
    Channel01.setAddress("/Channel01/AudioAnalysis");
    Channel01.addFloatArg(Channel01_Aubio.amplitude);
    Channel01.addFloatArg(Channel01_UsefulPitch);
    Channel01.addFloatArg(Channel01_att);
    Sender.sendMessage(Channel01);
    
    ofxOscMessage Channel02;
    Channel02.setAddress("/Channel02/AudioAnalysis");
    Channel02.addFloatArg(Channel02_Aubio.amplitude);
    Channel02.addFloatArg(Channel02_UsefulPitch);
    Channel02.addFloatArg(Channel02_att);
    Sender.sendMessage(Channel02);
    
    ofxOscMessage Channel03;
    Channel03.setAddress("/Channel03/AudioAnalysis");
    Channel03.addFloatArg(Channel03_Aubio.amplitude);
    Channel03.addFloatArg(Channel03_UsefulPitch);
    Channel03.addFloatArg(Channel03_att);
    Sender.sendMessage(Channel03);
    
    ofxOscMessage Channel04;
    Channel04.setAddress("/Channel04/AudioAnalysis");
    Channel04.addFloatArg(Channel04_Aubio.amplitude);
    Channel04.addFloatArg(Channel04_UsefulPitch);
    Channel04.addFloatArg(Channel04_att);
    Sender.sendMessage(Channel04);
    
    
    ofxOscMessage FFT01;
    FFT01.setAddress("/Channel01/FFT");
    FFT01.addIntArg(Channel01_Analyzer.nAverages);
    for (int i = 0; i < Channel01_Analyzer.nAverages; i++){
        FFT01.addFloatArg(Channel01_Analyzer.averages[i]);
        //        FFT01.addInt64Arg(Channel01_Analyzer.averages[i] * 10000);
        cout<< "ch01 analyzer: "<< i << " "<< Channel01_Analyzer.averages[i]<<endl;
    }
    Sender.sendMessage(FFT01);
    
    ofxOscMessage FFT02;
    FFT02.setAddress("/Channel02/FFT");
    FFT02.addIntArg(Channel02_Analyzer.nAverages);
    for (int i = 0; i < Channel02_Analyzer.nAverages; i++){
        FFT02.addFloatArg(Channel02_Analyzer.averages[i]);
    }
    Sender.sendMessage(FFT02);
    
    ofxOscMessage FFT03;
    FFT03.setAddress("/Channel03/FFT");
    FFT03.addIntArg(Channel03_Analyzer.nAverages);
    for (int i = 0; i < Channel03_Analyzer.nAverages; i++){
        FFT03.addFloatArg(Channel03_Analyzer.averages[i]);
    }
    Sender.sendMessage(FFT03);
    
    ofxOscMessage FFT04;
    FFT04.setAddress("/Channel04/FFT");
    FFT04.addIntArg(Channel04_Analyzer.nAverages);
    for (int i = 0; i < Channel04_Analyzer.nAverages; i++){
        FFT04.addFloatArg(Channel04_Analyzer.averages[i]);
    }
    Sender.sendMessage(FFT04);
    
    
}

//--------------------------------------------------------------
//Universal function which sets normals for the triangle mesh
void setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
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
//--------------------------------------------------------------

