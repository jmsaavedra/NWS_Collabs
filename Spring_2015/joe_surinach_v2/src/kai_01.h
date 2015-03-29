//
//  mvmnt_01.h
//  surinach_v1
//
//  Created by Joseph Saavedra on 3/27/15.
//
//

#pragma once
#include "ofMain.h"
#define NUM_MSG_STRINGS 4



//Particle class
class Particle {
public:
    Particle();                //Class constructor
    void setup();              //Start particle
    void update( float dt );   //Recalculate physics
    void draw(float volSize);               //Draw particle
    
    ofPoint pos;               //Position
    ofPoint vel;               //Velocity
    float time;                //Time of living
    float lifeTime;            //Allowed lifetime
    bool live;                 //Is particle live
    int colorMode;
};

//Control parameters class
class Params {
public:
    void setup();
    ofPoint eCenter;    //Emitter center
    float eRad;         //Emitter radius
    float velRad;       //Initial velocity limit
    float lifeTime;     //Lifetime in seconds
    
    float rotate;   //Direction rotation speed in angles per second
    
    float force;       //Attraction/repulsion force inside emitter
    float spinning;    //Spinning force inside emitter
    float friction;    //Friction, in the range [0, 1]
};

extern Params param; //Declaration of a global variable


//------------ GUI --------------
//Slider class
class Slider {
public:
    string title;		//Title
    ofRectangle rect;	//Rectangle for drawing
    float *value;       //Pointer to value which the slider changes
    float minV, maxV;   //Minimum and maximum values
};

//Interface class, which manages sliders
class Interface {
public:
    void setup();
    void addSlider( string title, float *value, float minV, float maxV );
    void draw();
    
    void save( int index );		//Save preset
    void load( int index );		//Load preset
    
    void mousePressed( int x, int y );
    void mouseDragged( int x, int y );
    void mouseReleased (int x, int y );
    
    vector<Slider> slider;	//Array of sliders
    int selected;			//Index of selected slider
};
//-------------------------------







class kai_01 {
    
public:
    void setup();
    void update(float amp);
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //for audio stuff
    //***********************************************
    //***********************************************
    ofTrueTypeFont font;
    //    ofxOscReceiver receiver;
    
    float * receivedFft;
    int nBandsToUse;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    int mouseX, mouseY;
    string mouseButtonState;
    
    ofImage receivedImage;
    
    float keyValue;
    //***********************************************
    //***********************************************
    
    
    
    vector<Particle> p;	  //Particles
    ofFbo fbo;            //Off-screen buffer for trails
    
    float history;        //Control parameter for trails
    float time0;          //Time value for computing dt
    
    float bornRate;       //Particles born rate per second
    float bornCount;      //Integrated number of particles to born
    
    //GUI
    Interface interf;
    bool drawInterface;
    
    
    
};



