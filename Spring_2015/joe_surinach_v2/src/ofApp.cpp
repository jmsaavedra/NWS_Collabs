#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    oscFFT.setup();

    for(int i=0; i<nBANDS; i++){
        smoothedFft_01[i] = 0;
        smoothedFft_02[i] = 0;
    }
    for(int i=0; i<4; i++){
        smoothedAmp[i] = 0;
    }
    
    ofSetCircleResolution(180);
    ofSetCurveResolution(180);
    
    
//    Mvt 1 (Opening): Santi
//    Mvt 2 (Charlie Chaplin): Kiyo Circle
//    Mvt 3 (Beauties on Parade): Angela
//    Mvt 4 (Epitaph to Silent Movies): Kiyo Movement 1
//    Mvt 5 (Stereophonism): Yu
//    Mvt 6 (Pasadoble in Technicolor): Kai

//    angela.setup();
//    kai.setup();
//    yu.setup();
    
    santi.setup();
    movementNumber = 1;
}

//--------------------------------------------------------------
void ofApp::update(){
    oscFFT.update();
    chan1Amp = oscFFT.chan1_amp;

    for(int i=0; i<nBANDS; i++){
        smoothedFft_01[i] *= 0.97f;
        smoothedFft_02[i] *= 0.97f;
        if(oscFFT.chan1_fft[i] > smoothedFft_01[i]) smoothedFft_01[i] = oscFFT.chan1_fft[i];
        if(oscFFT.chan2_fft[i] > smoothedFft_02[i]) smoothedFft_02[i] = oscFFT.chan2_fft[i];
    }
    for (int i=0; i<4; i++){
        smoothedAmp[i] *= 0.97f;
        switch(i){
        case 1:
            if(oscFFT.chan1_amp > smoothedAmp[i]) smoothedAmp[i] = oscFFT.chan1_amp;
            break;
        case 2:
            if(oscFFT.chan2_amp > smoothedAmp[i]) smoothedAmp[i] = oscFFT.chan2_amp;
            break;
        case 3:
            if(oscFFT.chan3_amp > smoothedAmp[i]) smoothedAmp[i] = oscFFT.chan3_amp;
            break;
        case 4:
            if(oscFFT.chan4_amp > smoothedAmp[i]) smoothedAmp[i] = oscFFT.chan4_amp;
            break;
        }
    }

    
    
    //========== MOVEMENT UPDATE METHODS ===============
    switch (movementNumber) {
        case 1:
            santi.update(smoothedFft_01);
            break;
        case 2:
            kiyo1.update(smoothedFft_01, smoothedFft_02);
            break;
        case 3:
            angela.update(smoothedFft_01);
            break;
        case 4:
            //something!!
            break;
        case 5:
            yu.update(smoothedAmp[0]);
            break;
        case 6:
            //kai.update(smoothedAmp[0]);
            kai.update(oscFFT.chan1_amp);
            break;
        default:
            break;
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}


//--------------------------------------------------------------
void ofApp::draw(){
    //========== MOVEMENT DRAW METHODS ===============
    switch (movementNumber) {
        case 1:
            santi.draw();
            break;
        case 2:
            kiyo1.draw();
            break;
        case 3:
            angela.draw(oscFFT.chan1_amp);
            break;
        case 4:
            //something!!
            break;
        case 5:
            yu.draw(smoothedFft_01);
            break;
        case 6:
            kai.draw();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == OF_KEY_RIGHT){
        movementNumber++;
        if(movementNumber > 6) movementNumber = 6;
        if(movementNumber == 1) santi.setup();
        if(movementNumber == 2) kiyo1.setup();
        if(movementNumber == 3) angela.setup();
        if(movementNumber == 4) cout << "SOMETHING!"<<endl;
        if(movementNumber == 5) yu.setup();
        if(movementNumber == 6) kai.setup();

    } else if (key == OF_KEY_LEFT){
        movementNumber--;
        if(movementNumber < 1) movementNumber = 1;
    } else if (key == 'f'){
        ofToggleFullscreen();
    }
    
    else {
        switch (movementNumber){
            case 1:
                santi.keyPressed(key);
                break;
            case 2:
                kiyo1.keyPressed(key);
                break;
            case 3:
                angela.keyPressed(key);
                break;
            case 4:
                //kiyo1.keyPressed(key);
                break;
            case 5:
                yu.keyPressed(key);
                break;
            case 6:
                kai.keyPressed(key);
                break;
            default:
                break;
        }
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    switch (movementNumber){
        case 1:
            // angela.mousePressed(x,y,button);
            break;
        case 2:
            // santi.mousePressed(x,y,button);
            break;
        case 3:
            // kai.mousePressed(x,y,button);
            break;
        case 4:
            kai.mouseDragged(x,y,button);
            break;
        case 5:
            
            break;
        case 6:
            kai.mouseDragged(x,y,button);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    
    switch (movementNumber){
        case 1:
        // angela.mousePressed(x,y,button);
            break;
        case 2:
        // santi.mousePressed(x,y,button);
            break;
        case 3:
        // kai.mousePressed(x,y,button);
            break;
        case 4:
            kai.mousePressed(x,y,button);
            break;
        case 5:
            
            break;
        case 6:
            kai.mousePressed(x,y,button);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

    switch (movementNumber){
        case 1:
            // angela.mousePressed(x,y,button);
            break;
        case 2:
            // santi.mousePressed(x,y,button);
            break;
        case 3:
            // kai.mousePressed(x,y,button);
            break;
        case 4:
            kai.mouseReleased(x,y,button);
            break;
        case 5:
            
            break;
        case 6:
            kai.mouseReleased(x,y,button);
            break;
        default:
            break;
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
