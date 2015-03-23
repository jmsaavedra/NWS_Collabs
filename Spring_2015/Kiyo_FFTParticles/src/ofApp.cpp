#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackgroundHex(0x000000);
    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    
    //my_stream.setup(this, 0, 2, 44100, 256, 4);
    
    soundPlayer.loadSound("3_parade_of_beauties.mp3");
    soundPlayer.setLoop(true);
    soundPlayer.play();
    
    ofToggleFullscreen();
    for(int i = 0; i < NUM; i++){
        float posX = ofRandom(ofGetWidth());
        float posY = ofRandom(ofGetHeight());
        particle[i].setup(ofVec2f(posX, posY), ofVec2f(0,0));
    }
    
    //FFT
    fftLive.setMirrorData(false);
    fftLive.setup();
    
    
    fftFile.setMirrorData(false);
    fftFile.setup();
    
    
    string guiPath = "audio.xml";
    gui.setup("audio", guiPath, 20, 20);
    gui.add(audioThreshold.setup("audioThreshold", 1.0, 0.0, 1.0));
    gui.add(audioPeakDecay.setup("audioPeakDecay", 0.915, 0.0, 1.0));
    gui.add(audioMaxDecay.setup("audioMaxDecay", 0.995, 0.0, 1.0));
    gui.add(radius.setup("radius", 10, 0.0, 100.0));
    gui.add(gravity.setup("gravity", 0.2, 0.0, 1.0));
    gui.add(audioMirror.setup("audioMirror", true));
    gui.loadFromFile(guiPath);
}

//--------------------------------------------------------------
void ofApp::update(){
    fftLive.setThreshold(audioThreshold);
    fftLive.setPeakDecay(audioPeakDecay);
    fftLive.setMaxDecay(audioMaxDecay);
    fftLive.setMirrorData(audioMirror);
    fftLive.update();
    
    fftFile.setThreshold(audioThreshold);
    fftFile.setPeakDecay(audioPeakDecay);
    fftFile.setMaxDecay(audioMaxDecay);
    fftFile.setMirrorData(audioMirror);
    fftFile.update();
    
    float * audioData = new float[NUM];
    //fftFile.getFftPeakData(audioData, NUM);
    fftLive.getFftPeakData(audioData, NUM);
    
    if(mode == 1){
        ofBackgroundHex(0x000000);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            //printf("hage: %f \n", audioValue); //-> 1891
            
            //ofPushMatrix();
            //ofPopMatrix();
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            //ofSetHexColor(c);
            //c.setHex( 0xFF8000, 128 ); // orange, 50% alpha
            
            //particle[i].setColor(1,1,1,audioValue * 256);
            particle[i].setColor(255,1,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(1);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }else if(mode == 2){
        ofBackgroundHex(0x000000);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        //printf("hage: %i \n", mode);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            particle[i].setColor(255,1,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(2);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }else if(mode == 3){
        ofBackgroundHex(0x000000);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        //printf("hage: %i \n", mode);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            particle[i].setColor(ofMap(i, 0, NUM, 0, 255, true),255,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(1);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }else if(mode == 4){
        ofBackgroundHex(0x000000);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        //printf("hage: %i \n", mode);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            particle[i].setColor(ofMap(i, 0, NUM, 0, 255, true),255,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(2);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }else if(mode == 5){
        ofBackgroundHex(0xFFFFFF);
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
        //printf("hage: %i \n", mode);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            particle[i].setColor(ofMap(i, 0, NUM, 0, 255, true),255,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(1);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }else if(mode == 6){
        ofBackgroundHex(0xFFFFFF);
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
        //printf("hage: %i \n", mode);
        for(int i=0; i<NUM; i++) {
            float audioValue = audioData[i];
            particle[i].resetForce();
            particle[i].addForce(ofVec2f(0, gravity));
            particle[i].addForce(ofVec2f(0, audioValue / -3));
            particle[i].setColor(ofMap(i, 0, NUM, 0, 255, true),255,audioValue * 255);
            particle[i].setRadius(audioValue * radius);
            particle[i].setFigure(2);
            particle[i].updateForce();
            particle[i].updatePos();
            particle[i].bounceOffWalls();
        }
    }
    delete[] audioData;
}

//--------------------------------------------------------------
void ofApp::draw(){
    for(int i = 0; i < NUM; i++){
        particle[i].draw();
    }
    
    int w = OFX_FFT_WIDTH;
    int h = OFX_FFT_HEIGHT;
    int x = 20;
    int y = ofGetHeight() - h - 20;
    //fftLive.draw(x, y, w, h);
    
    if(display){
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {//Full Screen
        ofToggleFullscreen();
        for(int i = 0; i < NUM; i++){
            float posX = ofRandom(ofGetWidth());
            float posY = ofRandom(ofGetHeight());
            particle[i].setup(ofVec2f(posX, posY), ofVec2f(0,0));
        }
    }else if(key == '1') {
        mode = 1;
    }else if(key == '2') {
        mode = 2;
    }else if(key == '3') {
        mode = 3;
    }else if(key == '4') {
        mode = 4;
    }else if(key == '5') {
        mode = 5;
    }else if(key == '6') {
        mode = 6;
    }else if(key == 'b') {
        blend = blend + 1;
        if(blend > 5) blend = 1;
        switch (blend) {
            case 1:
                blendMode = OF_BLENDMODE_ALPHA;
                break;
            case 2:
                blendMode = OF_BLENDMODE_ADD;
                break;
            case 3:
                blendMode = OF_BLENDMODE_SCREEN;
                break;
            case 4:
                blendMode = OF_BLENDMODE_MULTIPLY;
                break;
            case 5:
                blendMode = OF_BLENDMODE_SUBTRACT;
                break;
        }
    }
    
    //GUI
    if(key == 't') {
        display = !display;
    }
    
    //Radius
    if(key == OF_KEY_LEFT){
        radius = radius - 1.0;
    }else if(key == OF_KEY_RIGHT){
        radius = radius + 1.0;
    }
    
    //Audio Peak Decay
    if(key == OF_KEY_UP){
        gravity = gravity - 0.01;
    }else if(key == OF_KEY_DOWN){
        gravity = gravity + 0.01;
    }
    
    //Bang!
    if(key == 'a'){
        int posX = ofRandom(ofGetScreenWidth());
        int posY = ofRandom(ofGetScreenHeight());
        for(int i = 0; i < NUM; i++){
            ofVec2f pos = ofVec2f(posX, posY);
            float length = ofRandom(20);
            float angle = ofRandom(PI * 2);
            ofVec2f vel = ofVec2f(cos(angle) * length, sin(angle) * length);
            particle[i].setup(pos, vel);
        }
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
    for(int i = 0; i < NUM; i++){
        ofVec2f pos = ofVec2f(x, y);
        float length = ofRandom(20);
        float angle = ofRandom(PI * 2);
        ofVec2f vel = ofVec2f(cos(angle) * length, sin(angle) * length);
        particle[i].setup(pos, vel);
    }
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
