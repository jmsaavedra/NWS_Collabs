
/**********************************************************************
 
 Visuals for New World Symphony Presentation in March 09, 2014.
 Joseph Haydn - String Quartet No. 34 in B-Flat Major, Op. 33, No. 4.
 
 A project by Bernardo Schorr
 Made for Independent Study
 Parsons MFA Design+Technology
 Spring 2014
 
 Leading teachers: Edward Keller, Joseph Saavedra
 Participants: Bernardo Schorr, Brendan Byrne, Gabriel Gianordoli,
 Julie Huynh, Madhava Tankha, Namreta Kumar.
 
 Made with openframeworks: http://www.openframeworks.cc
 
 Making use of the following ofx addons:
 ofxNetwork
 ofxOpenCv
 ofxOpticalFlowFarneback
 ofxSyphon
 
 Thanks to the authors of openframeworks
 and ofxaddons that made this project possible.
 
 This project also relies on Soundflower for routing audio through
 applications in OSX. More info at: http://cycling74.com/soundflower-landing-page/
 
 **********************************************************************/

#pragma once

//including OF and addons
#include "ofMain.h"
#include "fft.h"
#include "FFTOctaveAnalyzer.h"
#include "ofxSyphon.h"
#include "ofxNetwork.h"

//project runs in four modes, specified in these separate headers.
#include "ModeOne.h"
#include "ModeTwo.h"
#include "ModeThree.h"
#include "ModeFour.h"

//For inputting and parsing accelerometer and magnetometer sensor data
#include "sensorData.h"
#include "movingBackground.h"

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
    
    /**********************************************************************
    
    This is where all our audio is processed using FFt (Fast Fourier transform).
    We're transforming audio input in frequency graphs and using this data
    to drive transformations in the screen.
     
    More information at: http://en.wikipedia.org/wiki/Fast_Fourier_transform
     
    **********************************************************************/
    
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
    
    /**********************************************************************
     
     Simple variables to hold width and height, rendering unnecessary to call
     ofGetWindowWidth() and ofGetWindowHeight() once per loop.
     
     **********************************************************************/
    
    int width, height;
    
    /**********************************************************************
     
     The project outputs visuals for audio in 4 different modes accessible 
     through pressing keys 1 to 4. The current mode is controlled by the 
     integer 'modeSelector'.
     
     **********************************************************************/
    
    int modeSelector;
    ModeOne modeOne;
    ModeTwo modeTwo;
    ModeThree modeThree;
    ModeFour modeFour;
    
    /**********************************************************************
     
     For each mode, there are four overlayed color backgrounds on the visuals. 
     These backgrounds move according to accelerometer data being input from 
     sensors in the musicians wrists.
     
     **********************************************************************/
    
    //hold 4 background for each mode
    vector <MovingBackground> bkgListOne;
    vector <MovingBackground> bkgListTwo;
    vector <MovingBackground> bkgListThree;
    vector <MovingBackground> bkgListFour;
    
    //hold a value to multiply the incoming sensor data
    float *  multiplier;
    float multValue;
    
    /**********************************************************************
     
     This drives the video input. On stage we used a Blackmagic 
     UltraStudio Express to input video data from an SDI connection. 
     Running without blackmagic it will connect to the first available video 
     input in your computer, most likely you built-in video camera.
     
     This code is extracted from the osxVideoRecorder example that ships with OF.
     
     **********************************************************************/
    
    void setupGrabber();
    ofVideoGrabber 			vidGrabber;
    ofPtr<ofQTKitGrabber>	vidRecorder;
    int videoWidth, videoHeight;
    
    /**********************************************************************
     
     This drives the input from accelerometer and magnetometer data via UDP. 
     We're not actually using Magnetometer data, it's there in case one needs 
     to adapt the code to use it.
     
     **********************************************************************/
    
    ofxUDPManager udpConnection;
    ofVec3f accel, magne;
    sensorData sensors;
    
    /**********************************************************************
     
     All visuals are written to an FBO and then sent out to MadMapper via Syphon.
     MadMapper was actually doing the output to the main screens.
     
     **********************************************************************/
    ofFbo fbo;
    ofxSyphonServer individualTextureSyphonServer;
	ofxSyphonClient mClient;
};
