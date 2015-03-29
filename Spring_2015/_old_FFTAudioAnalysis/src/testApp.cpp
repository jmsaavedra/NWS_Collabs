#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    Sender.setup(HOST, PORT);
    
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    setupFailed = false;
    
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
    ofSoundStreamListDevices();
    
//	cout<< "devices: " <<  ofSoundStreamListDevices() << endl;
    
	
	AudioIn.setDeviceID(2); // !!! watch print for list of devices !!!
//    if (NCHAN > 3){
//        if ( ! AudioIn.setup(this, 2, 4, 44100, BUFFER_SIZE, 4)){
//            setupFailed = true;
//        }
//    }
//    else {
    AudioIn.setup(this, 0, 8, 44100, BUFFER_SIZE, 1);
//    }
    AudioIn.start();
    
    
	peakTime =15; // hold longer
    decayRate = 0.95f; // decay slower
    HiGain = 0.9f; // reduced gain at lowest frequency
    LowGain = 0.01f; // increasing gain at higher frequencies
    
    avg_power  = 0.0f;
    
	Channel01_Analyzer.setup(44100, BUFFER_SIZE/2, 11);
    Channel02_Analyzer.setup(44100, BUFFER_SIZE/2, 11);
    
//    Channel01_Analyzer.setup(44100, BUFFER_SIZE, 2);
//    Channel02_Analyzer.setup(44100, BUFFER_SIZE, 2);
	
	Channel01_Analyzer.peakHoldTime = peakTime;
	Channel01_Analyzer.peakDecayRate = decayRate;
	Channel01_Analyzer.linearEQIntercept = HiGain;
	Channel01_Analyzer.linearEQSlope = LowGain;
    
    Channel02_Analyzer.peakHoldTime = peakTime;
	Channel02_Analyzer.peakDecayRate = decayRate;
	Channel02_Analyzer.linearEQIntercept = HiGain;
	Channel02_Analyzer.linearEQSlope = LowGain;
    
    Channel01_Aubio.setup();
    Channel02_Aubio.setup();
    
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
}


//--------------------------------------------------------------
void testApp::update(){
    if(!setupFailed){
        audioAnalisys();
        SentMessages();
        
        Channel01_FFT_size = Channel01_Analyzer.nAverages;
        Channel02_FFT_size = Channel02_Analyzer.nAverages;
        
        Channel01_Pitch = Channel01_UsefulPitch;
        Channel01_Attack = Channel01_att;
        Channel01_Amplitude = Channel01_Aubio.amplitude;
        
        Channel02_Pitch= Channel02_UsefulPitch;
        Channel02_Attack= Channel02_att;
        Channel02_Amplitude= Channel02_Aubio.amplitude;

        Channel01_LinearPitch = 69 + 12*log2f(Channel01_Pitch/440);
        Channel02_LinearPitch = 69 + 12*log2f(Channel02_Pitch/440);
    }
    
}
//--------------------------------------------------------------
void testApp::draw(){
    if(!setupFailed){
        //debug view
        ofPushStyle();
            ofPushMatrix();
                ofTranslate(0, 100);
        
                //-----  channel 01 -----
                ofPushMatrix();
                    ofTranslate(100, 0);
                    ofFill();
                    ofDrawBitmapString("Channel01", -90,0);
                    ofSetColor(255,40);
                    ofDrawBitmapString("FFT: "+ ofToString(Channel01_FFT_size) + " freq", 0,15);
                    
                    ofDrawBitmapString("Frequency: " + ofToString(Channel01_Pitch,0), 0,30);
                    ofDrawBitmapString("Linear Pitch: " + ofToString(Channel01_LinearPitch,0), 0,45);
                    ofDrawBitmapString("Amplitude: " + ofToString(Channel01_Amplitude,4), 0,60);
                    ofDrawBitmapString("Attack: " + ofToString(Channel01_Attack,4), 0,75);
                    for (int i = 0; i < Channel01_FFT_size; i++){
                        ofSetColor(255,20);
                        ofRect((i*6),0,5,-Channel01_Analyzer.averages[i] * 3);
                        ofSetColor(255);
                        ofLine((i*6), -Channel01_Analyzer.averages[i] * 3, ((i+1)*6), -Channel01_Analyzer.averages[i+1] * 3);
                    }
                    ofSetColor(255, 255*Channel01_Amplitude);
                    ofCircle(-50,50, Channel01_Attack*3);
                    ofSetColor(255);
                    ofNoFill();
                    ofCircle(-50, 50, Channel01_Attack*3);
                
                ofPopMatrix();
        
                //-----  channel 02 -----
                ofPushMatrix();
                    ofTranslate(100, 180);
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
	}
    
    Channel01_Aubio.processAudio(Channel01, BUFFER_SIZE);
    Channel02_Aubio.processAudio(Channel02, BUFFER_SIZE);
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
}

//-------------------------------------------------------
void testApp::SentMessages(){
    
    
//    ofxOscMessage Channel01;
//    Channel01.setAddress("/Channel01/AudioAnalysis");
//    Channel01.addFloatArg(Channel01_Aubio.amplitude);
//    Channel01.addFloatArg(Channel01_UsefulPitch);
//    Channel01.addFloatArg(Channel01_att);
//    Sender.sendMessage(Channel01);
//    
//    ofxOscMessage Channel02;
//    Channel02.setAddress("/Channel02/AudioAnalysis");
//    Channel02.addFloatArg(Channel02_Aubio.amplitude);
//    Channel02.addFloatArg(Channel02_UsefulPitch);
//    Channel02.addFloatArg(Channel02_att);
//    Sender.sendMessage(Channel02);

    
   // cout<<    sizeof(Channel01_Analyzer.nAverages) <<endl;
//    cout << Channel02_FFT_size << endl;
////    cout << sizeof(Channel01_Analyzer.spe2avg)<<endl;
//    
//    ofxOscMessage FFT01;
//    FFT01.setAddress("/channel_01/FFT");
////    FFT01.addIntArg(Channel01_Analyzer.nAverages);
//    for (int i = 0; i < Channel01_Analyzer.nAverages; i++){
//        FFT01.addIntArg(i);
//        FFT01.addFloatArg(Channel01_Analyzer.averages[i]);
//
////        FFT01.addInt64Arg(Channel01_Analyzer.averages[i] * 10000);
////      cout<< "ch01 analyzer: "<< i << " "<< Channel01_Analyzer.averages[i]<<endl;
//    }
//    Sender.sendMessage(FFT01);
    
    for (int i = 0; i < Channel01_Analyzer.nAverages; i++){
        ofxOscMessage FFT01;
        FFT01.setAddress("/channel_02/FFT");
        FFT01.addIntArg(i);
        FFT01.addFloatArg(Channel01_Analyzer.averages[i]);
        Sender.sendMessage(FFT01);
    }


    for (int i = 0; i < Channel02_Analyzer.nAverages; i++){
        ofxOscMessage FFT02;
        FFT02.setAddress("/channel_02/FFT");
        FFT02.addIntArg(i);
        FFT02.addFloatArg(Channel02_Analyzer.averages[i]);
        Sender.sendMessage(FFT02);
    }

//    cout << "sent: "<<Channel02_Analyzer.nAverages << endl;
}