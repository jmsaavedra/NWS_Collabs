#include "soundizeMeApp.h"
#include <algorithm>

/*void soundizeMeApp::~soundizeMeApp(){}*/

//--------------------------------------------------------------
void soundizeMeApp::setup(){
    
    // Smooth edges
    ofEnableSmoothing();
    
    // Fixed framerate
    ofSetFrameRate(30);
    
    m_audio.loadSound("3_parade_of_beauties.mp3");
    m_audio.setLoop(true);
    m_audio.play();
    
    // the fft needs to be smoothed out, so we create an array of floats
    // for that purpose:
    m_nBandsToGet = 1024;
    int * arr = new int[m_nBandsToGet*4];
    for (int i = 0; i < m_nBandsToGet*4; i++){
        arr[i] = i % m_nBandsToGet;
    }
    random_shuffle(&arr[m_nBandsToGet * 0 ], &arr[m_nBandsToGet * 1]);
    random_shuffle(&arr[m_nBandsToGet * 1 ], &arr[m_nBandsToGet * 2]);
    random_shuffle(&arr[m_nBandsToGet * 2 ], &arr[m_nBandsToGet * 3]);
    random_shuffle(&arr[m_nBandsToGet * 3 ], &arr[m_nBandsToGet * 4]);
    
    m_fftSmoothed = new float[m_nBandsToGet];
    for (int i = 0; i < m_nBandsToGet; i++){
        m_fftSmoothed[i] = 0;
    }
    
    /*
     1 sichqare X [-2,2]
     2 sichqare Y [-2,2]
     3 radiusi  [5,30]
     4 rgb    setHex
     */
    
    for (int i = 0; i < m_nBandsToGet/8 ; i++)
    {
        float theta         = ofRandom(0,360);
        ofVec2f location    = ofVec2f(ofRandomWidth(),ofRandomHeight());
        ofVec2f velocity    = ofVec2f(sin(theta),cos(theta));
        ofColor color       = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
        velocity           *= ofRandom(0.5,2);
        
        std::vector<int> FFTids;
        FFTids.push_back(arr[m_nBandsToGet * 0 + i]);
        FFTids.push_back(arr[m_nBandsToGet * 1 + i]);
        FFTids.push_back(arr[m_nBandsToGet * 2 + i]);
        FFTids.push_back(arr[m_nBandsToGet * 3 + i]);
        
        // for (int i = 0; i < 4; i++)
        //     FFTids.push_back(int(ofRandom(m_nBandsToGet)));
        m_balls.push_back(Ball(location,velocity,color,20,FFTids));
        
        
    }
    
    // slider1.setup("slider1", 50, 0, 100);
    // slider2.setup("slider2", 50, 0, 100);
    // slider3.setup("slider3", 50, 0, 100);
    // slider4.setup("slider4", 50, 0, 100);
    // m_gui.setup("params", 10, 10);
    // m_gui.add(&slider1);
    // m_gui.add(&slider2);
    // m_gui.add(&slider3);
    // m_gui.add(&slider4);
    
    // ofSetBackgroundAuto(false);
    // ofBackground(0);
}

//--------------------------------------------------------------
void soundizeMeApp::update(){
    
    
    // grab the fft, and put in into a "smoothed" array,
    // by taking maximums, as peaks and then smoothing downward
    float * val = ofSoundGetSpectrum(m_nBandsToGet * 2);        // request 1024 values for fft
    for (int i = 0;i < m_nBandsToGet; i++){
        
        // let the smoothed calue sink to zero:
        m_fftSmoothed[i] *= (m_isPlaying) ? 0.96f : 0.5f;
        
        // take the max, either the smoothed or the incoming:
        if (m_fftSmoothed[i] < val[i]) m_fftSmoothed[i] = val[i];
        
    }
    
    // float * paramns = new float[4];
    // paramns[0] = slider1.getValue();
    // paramns[1] = slider2.getValue();
    // paramns[2] = slider3.getValue();
    // paramns[3] = slider4.getValue();
    //
    if (ofGetWidth() ==0 && ofGetHeight() == 0)
        return;
    for (int i = 0; i < m_balls.size(); i++){
        Ball *ball = & m_balls.at(i);
        // ball->separate(m_balls);
        ball->FFTupdate(m_fftSmoothed);
        ball->update();
        ball->checkEdges();
    }
    // delete[] paramns;
}

//--------------------------------------------------------------
void soundizeMeApp::draw(){
    // ofSetColor(ofColor(0,0,0,100));
    // ofRect(0,0,ofGetWidth(),ofGetHeight());
    ofBackgroundGradient(ofColor::gray,ofColor(30,10,30), OF_GRADIENT_CIRCULAR);
    for (int i = 0; i < m_balls.size(); i++){
        m_balls.at(i).draw();
    }
    
    //ofSetColor(ofColor(255,255,255));
    //glBegin(GL_LINE_STRIP);
    //glVertex2f(0,ofGetHeight());
    //float jumpSize = ofGetWidth()/float(m_nBandsToGet);
    //for (int i = 0;i < m_nBandsToGet; i++){
        //glVertex2f((i+1)*jumpSize,ofGetHeight() - (m_fftSmoothed[i] * 200));
    //}
    //glEnd();
    
    // m_gui.draw();
    // if(bInfoText) {
    //     stringstream ss;
    //     ss <<"Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
    //     ss <<"(f): Toggle Fullscreen"<<endl;
    //     ss <<"(s): Save Frame"<<endl;
    //     ss <<"(p): Pause Player"<<endl;
    //     ss <<"balls size"<<m_balls.size()<<endl;
    //     ss <<"(o): SystemLoadDialog"<<endl;
    //     ofDrawBitmapString(ss.str().c_str(), 20, 20);
    // }
}

//--------------------------------------------------------------
void soundizeMeApp::keyPressed(int key){
    switch(key){
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            ofSaveFrame();
            break;
        case 'p':
            m_audio.setPaused(m_isPlaying);
            m_isPlaying = !m_isPlaying;
            break;
        case 'd':
            cout << "m_balls.size()         " << m_balls.size() << endl;
            cout << "ofGetFrameRate()       " << ofGetFrameRate() << endl;
            cout << "ofGetTargetFrameRate() " << ofGetTargetFrameRate() << endl;
            break;
        case 'o':
            ofFileDialogResult result = ofSystemLoadDialog("open audio file");
            if (result.bSuccess){
                m_audio.loadSound(result.getPath(),true);
                m_audio.play();
                m_isPlaying = true;
            }
            break;
    }
}

//--------------------------------------------------------------
void soundizeMeApp::keyReleased(int key){}

//--------------------------------------------------------------
void soundizeMeApp::mouseMoved(int x, int y){}

//--------------------------------------------------------------
void soundizeMeApp::mouseDragged(int x, int y, int button){
    float theta         = ofRandom(0,360);
    ofVec2f location    = ofVec2f(x,y);
    ofVec2f velocity    = ofVec2f(sin(theta),cos(theta));
    ofColor color       = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    velocity           *= ofRandom(2,4);
    
    std::vector<int> FFTids;
    for (int i = 0; i < 4; i++)
        FFTids.push_back(int(ofRandom(m_nBandsToGet)));
    m_balls.push_back(Ball(location,velocity,color,20,FFTids));
}

//--------------------------------------------------------------
void soundizeMeApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void soundizeMeApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void soundizeMeApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void soundizeMeApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void soundizeMeApp::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size()){
        m_audio.loadSound(dragInfo.files.at(0),true);
        m_audio.play();
        m_isPlaying = true;
        
    }
}
