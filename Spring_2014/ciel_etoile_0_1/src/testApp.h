#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Particle.h"
#include "Tiles.h"

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

    /*-------------------- GUI --------------------*/
	void setGUI1();
	void setGUI2();
	
	ofxUISuperCanvas *gui1;
	ofxUISuperCanvas *gui2;
    
	void guiEvent(ofxUIEventArgs &e);
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
    /*---------------------------------------------*/


    /*---------------- BACKGROUND -----------------*/
    vector <Tiles> myTiles;
    vector <string> tileModes;
    string selectedTileMode;
    /*---------------------------------------------*/

};
