#pragma once

#include "ofMain.h"
#include "ofxOpticalFlowFarneback.h"
#include "ofxUI.h"

#include "particle.h"
#include "vectorField.h"

//#define _USE_LIVE_VIDEO

class testApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// particle & vector field-related
	void addParticle(float x, float y, float vx, float vy);
	vector <particle> particles;
	vectorField field;
	ofVec2f vectorFieldScale;
	
	// CV or video player-related
	ofVideoGrabber vidGrabber;
	ofVideoPlayer vidPlayer, p1, p2;
	vector <ofVideoPlayer> vidPlayers;
	int videoPlayerIndex;
	int videoWidth;
	int videoHeight;
	void setupGrabber();
	//void setupPlayer(string filename = "topview.mp4");
	void setupPlayer(int index = 0);
	void setupSolverAndField();
	
	// CV images for debug
	ofxCvColorImage colorImage, colorImageBlurred;
	ofxCvGrayscaleImage grayImage, grayImagePrev, grayImageDiff, grayImageDiffHistory;
	ofxCvContourFinder contourFinder;
	
	// optical flow solver & scale
    ofxOpticalFlowLK flowSolver;
	ofVec2f flowSolverScale;
	
	// program related
	bool useLiveVideo;
	bool mirrorVideo;
	bool drawVideo;
	bool drawVideoFullscreen;
	bool drawFlowSolver;
	bool drawFlowSolverFullscreen;
	bool drawVectorField;
	bool drawImageDiff;
	bool drawParticles;
	float particleMaxCount;
	float particleCount;
	bool particleColorBasedOnDirection;
	bool particleFade;
	float particleSaturation;
	float minimumVelocity;
	float videoBlurAmount;
	float backgroundTransparency;
	bool increaseBlurAndDim;
	float imageDiffThreshold;
	float imageDiffBlur;
	
	// gui related
	ofxUICanvas *gui;
	void setupGUI();
	void guiEvent(ofxUIEventArgs &e);
	
};
