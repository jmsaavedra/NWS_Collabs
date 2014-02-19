#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    Sender.setup(HOST, PORT);
    
	// 0 output channels,
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
    
    ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);
	
    
	ofSoundStreamListDevices();
    
	
	AudioIn.setDeviceID(2); // !!! watch print for list of devices !!!
    AudioIn.setup(this, 2, 4, 44100, BUFFER_SIZE, 4);
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
//--------------------------------------------------------------
void testApp::draw(){
    //debug view
    ofPushStyle();
        ofPushMatrix();
            ofTranslate(0, ofGetWindowHeight()-130);
    
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