

#include "soundizeMeApp.h"
#include <algorithm>



//--------------------------------------------------------------
void santiSoundize::setup(){

    ofSetCircleResolution(100);
    
    ofEnableSmoothing();

    ofSetFrameRate(30);

  
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

        
        m_balls.push_back(Ball(location,velocity,color,20,FFTids));
    }

  
}

//--------------------------------------------------------------
void santiSoundize::update(float receivedFft[] ){

    m_fftSmoothed = receivedFft;
    
    float * val = ofSoundGetSpectrum(m_nBandsToGet * 2);        // request 1024 values for fft
    for (int i = 0;i < m_nBandsToGet; i++){

        
        m_fftSmoothed[i] *= (m_isPlaying) ? 0.96f : 0.5f;

       
//        if (m_fftSmoothed[i] < val[i]) m_fftSmoothed[i] = val[i];

    }

   
    if (ofGetWidth() ==0 && ofGetHeight() == 0)
        return;
    for (int i = 0; i < m_balls.size(); i++){
        Ball *ball = & m_balls.at(i);
        // ball->separate(m_balls);
        ball->FFTupdate(m_fftSmoothed);
        ball->update();
        ball->checkEdges();
    }
    
}

//--------------------------------------------------------------
void santiSoundize::draw(){
    ofBackgroundGradient(ofColor::grey,ofColor(ofRandom(1),ofRandom(1),ofRandom(1)), OF_GRADIENT_LINEAR);
    for (int i = 0; i < m_balls.size(); i++){
        m_balls.at(i).draw();
    }

  

}

//--------------------------------------------------------------
void santiSoundize::keyPressed(int key){
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
            if (result.bSuccess){                m_audio.loadSound(result.getPath(),true);
                m_audio.play();
                m_isPlaying = true;
            
            }
        break;
           
    }
}

//--------------------------------------------------------------
void santiSoundize::keyReleased(int key){}

//--------------------------------------------------------------
void santiSoundize::mouseMoved(int x, int y){}

//--------------------------------------------------------------
void santiSoundize::mouseDragged(int x, int y, int button){
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
void santiSoundize::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void santiSoundize::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void santiSoundize::windowResized(int w, int h){}

//--------------------------------------------------------------
void santiSoundize::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void santiSoundize::dragEvent(ofDragInfo dragInfo){
    if (dragInfo.files.size()){
        m_audio.loadSound(dragInfo.files.at(0),true);
        m_audio.play();
        m_isPlaying = true;

    }
}
