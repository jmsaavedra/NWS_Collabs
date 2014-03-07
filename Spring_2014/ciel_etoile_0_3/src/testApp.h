#pragma once

#include "ofMain.h"

#include "ofxUI.h"

#include "ofxNetwork.h"

#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#include "Particle.h"
#include "Tiles.h"

#include "ofxSyphon.h"

#define BUFFER_SIZE 512
#define NUM_PARTICLES 256

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
	void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    
    /*-------------------- FOG --------------------*/
	ofVideoPlayer fogMovie;
    float videoAlpha;
    /*---------------------------------------------*/
    
    
    /*------------------ SYPHON ------------------*/
	ofxSyphonServer syphon;
    /*--------------------------------------------*/
    
    
    /*-------------------- GUI --------------------*/
	void setGUI1();
	void setGUI2();
	
	ofxUISuperCanvas *gui1;
	ofxUISuperCanvas *gui2;
    
	void guiEvent(ofxUIEventArgs &e);
    /*---------------------------------------------*/

    
    /*------------------- WI-FLY ------------------*/
    ofxUDPManager udpConnection;
//    vector<sensorData> sensors;    
    vector<ofVec3f> accel;
    vector<ofVec3f> magne;
    void updateConnection();
    int mySensorID;
    /*---------------------------------------------*/

    
    /*------------------- SOUND -------------------*/
    void audioIn(float * input, int bufferSize, int nChannels);
    
    FFTOctaveAnalyzer FFTanalyzer;
    
    float left[BUFFER_SIZE];
    float right[BUFFER_SIZE];
    
    fft		myfft;
    
    float magnitude[BUFFER_SIZE];
    float phase[BUFFER_SIZE];
    float power[BUFFER_SIZE];
    float freq[BUFFER_SIZE/2];
    
    ofSoundStream soundStream;
    
    bool bDrawAverages;
    /*---------------------------------------------*/
    
    
    /*----------------- PARTICLES -----------------*/
    vector<Particle> myParticles;
    
    vector<string> modes;
    string selectedMode;
    
    vector<string> shapes;
    string selectedShape;
    
    ofColor bgColor;
    float expansion;
    float rotation;
    float shapeSize;
    float particleSize;
    float nVertices;
    
    Boolean particleGUImode;    //Switch between GUI and wrist band, for controlling particles
    /*---------------------------------------------*/
    
    
    /*---------------- BACKGROUND -----------------*/
    vector <Tiles> myTiles;
    vector <string> tileModes;
    string selectedTileMode;
    float threshold;
    /*---------------------------------------------*/
    
};
