#pragma once

#include "ofMain.h"
#include "ofxSpacebrew.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"

#define BUFFER_SIZE 512

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        //audio stuff over here. Check out the function in testApp.cpp
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
		
    
        //Spacebrew stuff over here
        Spacebrew::Connection spacebrew;
        void onSpacebrewMessage( Spacebrew::Message & m );
    
        //where we'll store incoming values from Spacebrew
        ofVec3f accel, rot;

        //other things we'll use to keep track of sensors
        int width, height;
        ofVec2f sqPos, sqVel, sqAccel;
        float sqSize, sqSizeMult;
    
};
