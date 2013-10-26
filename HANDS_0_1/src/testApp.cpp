/*
 
 HANDS
 
 A project by Matt Felsen, Jia Xu, and Norihito Yuki
 Expanded Cinema & Sound
 Parsons MFA Design+Technology
 Fall 2013
 
 Developed for a performance of the New World Symphony in Miami Beach
 
 Code for particle & vector field classes taken from...
 Zach Lieberman's Algorithmic Animation class, Fall 2012
 Charlie Whitney's Algorithmic Animation class, Fall 2013
 
 Also making use of the following ofx addons:
 ofxOpenCv
 ofxOpticalFlowFarneback
 ofxUI
 ofxXmlSettings
 ofxSyphon
 
 Many thanks to the authors of all the above code for their contributions
 which made this project possible!
 
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(0);
	
    ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// set program defaults
	useLiveVideo = true;
	mirrorVideo = true;
	drawVideoDebug = true;
	drawFlowSolver = true;
	drawVectorField = false;
	drawImageDiff = false;
	drawParticles = true;
	particleMaxCount = 5000;
	particleColorBasedOnDirection = true;
	particleFade = true;
	minimumVelocity = 1;

	// set up scaling factors for flow solver & vector field
	flowSolverScale.set(0.5, 0.5);
	vectorFieldScale.set(0.1, 0.1);
	
	// set up cam or video player
	if (useLiveVideo) {
		setupGrabber();
	} else {
		setupPlayer();
	}
	
	// set up optical flow solver & vector field based on settings from above
	setupSolverAndField();
	
	// set up gui
	setupGUI();

}

void testApp::setupGrabber() {
	// define size for input video
	videoWidth = 640;
	videoHeight = 480;
	
	// set up video camera
	if (!vidGrabber.isInitialized()) {
		vidGrabber.initGrabber(videoWidth, videoHeight);
	}
}

void testApp::setupPlayer() {
	// define size for video file
	videoWidth = 480;
	videoHeight = 270;
	
	// set up video player
	if (!vidPlayer.isLoaded()) {
		vidPlayer.loadMovie("sideview2.mov");
	}
	//vidPlayer.setVolume(0);
}

void testApp::setupSolverAndField() {
	// set up optical flow solver at 1/2 of video size
	flowSolver.setup(videoWidth * flowSolverScale.x, videoHeight * flowSolverScale.y, 0.5, 3, 10, 1, 7, 1.5, false, false);
	flowSolver.setMirror(mirrorVideo, false);
	
	// set up vector field at 1/10th of video size
	field.setupField(videoWidth * vectorFieldScale.x, videoHeight * vectorFieldScale.y, ofGetWidth(), ofGetHeight());
	particles.clear();
	
	colorImage.clear();
	grayImage.clear();
	grayImagePrev.clear();
	grayImageDiff.clear();
	
	colorImage.allocate(videoWidth, videoHeight);
	grayImage.allocate(videoWidth, videoHeight);
	grayImagePrev.allocate(videoWidth, videoHeight);
	grayImageDiff.allocate(videoWidth, videoHeight);
	grayImageDiffHistory.allocate(videoWidth, videoHeight);
}

void testApp::setupGUI() {

	gui = new ofxUICanvas();
    gui->addLabel("HANDS CONTROL");
    gui->addSpacer();
    gui->addFPSSlider("FPS");
    gui->addSpacer();

	gui->addLabel("Video Input");
	gui->addToggle("Use Live Video", &useLiveVideo);
	gui->addToggle("Mirror Video", &mirrorVideo);
    gui->addSpacer();

	gui->addLabel("Debug View Drawing");
	gui->addToggle("Draw Video Debug", &drawVideoDebug);
	gui->addToggle("Draw Flow Solver", &drawFlowSolver);
	gui->addToggle("Draw Vector Field", &drawVectorField);
	gui->addToggle("Draw Image Diff", &drawImageDiff);
	gui->addToggle("Draw Particles", &drawParticles);
    gui->addSpacer();

	gui->addLabel("Particle Settings");
	gui->addSlider("Particle Max Count", 3000, 10000, &particleMaxCount);
	gui->addSlider("Particle Count", 0, 10000, &particleCount);
	gui->addToggle("Color Based on Direction", &particleColorBasedOnDirection);
	gui->addToggle("Fade Brightness", &particleFade);
	gui->addSlider("Minimum Velocity", 0, 5, &minimumVelocity);
	gui->addSpacer();

	gui->addLabel("Press 'g' to toggle GUI");
    gui->autoSizeToFitWidgets();
	gui->setPosition(ofGetWidth() - 211, 0);
	
	ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);

}


//--------------------------------------------------------------
void testApp::addParticle(float x, float y, float vx, float vy) {
    
    // offset the start position a little bit
    float xPos = x + ofRandom(-10, 10);
    float yPos = y + ofRandom(-10, 10);
    
    // now offset the initial velocity a good amount too
    ofVec2f velOffset( vx, vy );
    velOffset *= ofRandom(1,5);
	ofVec2f newVel = ofVec2f(vx, vy) * 0.3 + velOffset;
    
    // spawn a new particle with position and velocity based on movement with a bit of randomness sprinkled in
    particles.push_back( particle( ofVec2f(xPos, yPos), newVel ) );
}


//--------------------------------------------------------------
void testApp::update(){
	
	// update flow solver from new video frame
	if (useLiveVideo) {
		
		vidGrabber.update();
		
		if (vidGrabber.isFrameNew()){
			flowSolver.update(vidGrabber);
			
			colorImage.setFromPixels(vidGrabber.getPixels(), videoWidth, videoHeight);
			grayImage = colorImage;
			grayImage.mirror(false, mirrorVideo);
			
			if (drawImageDiff) {
				grayImageDiff.absDiff(grayImage, grayImagePrev);
				grayImageDiff.threshold(30);
				
				grayImageDiff.blur();
				grayImagePrev = grayImage;

				grayImageDiffHistory -= 30;
				grayImageDiffHistory += grayImageDiff;
				grayImageDiffHistory.blur(9);
			}
			
			//contourFinder.findContours(grayImageDiff, 5, 76800, 4, false);
			//cout << contourFinder.nBlobs << endl;
			

		}
	} else {
		
		vidPlayer.update();
		
		if (vidPlayer.isFrameNew()){
			flowSolver.update(vidPlayer);
			
			colorImage.setFromPixels(vidPlayer.getPixels(), videoWidth, videoHeight);
			grayImage = colorImage;
			grayImage.mirror(false, mirrorVideo);
			
			if (drawImageDiff) {
				grayImageDiff.absDiff(grayImage, grayImagePrev);
				grayImageDiff.threshold(30);
				
				grayImageDiff.blur();
				grayImagePrev = grayImage;
				
				grayImageDiffHistory -= 30;
				grayImageDiffHistory += grayImageDiff;
				grayImageDiffHistory.blur(9);
			}
			
			//contourFinder.findContours(grayImageDiff, 5, 76800, 1, false);
			//cout << contourFinder.nBlobs << endl;
		}
	}
	
	// update vector field from optical flow solver
	// there's some scaling going on between the sizes of the video, flow solver, and vector field
	ofPoint vel;
	int res = 1/vectorFieldScale.x;
	
	for(int x=0; x<videoWidth; x+=res) {
		for(int y=0; y<videoHeight; y+=res) {
			vel = flowSolver.getVelAtNorm(x / (float)videoWidth, y / (float)videoHeight);
			
			// skip if the values are too small
			if(vel.length() < minimumVelocity) {
				continue;
            }
			
			// add vel value to the field
			int pos = (y*vectorFieldScale.y) * field.fieldWidth + (x*vectorFieldScale.x);
			field.field[pos] += vel;
			
			// if we're under the particle limit, add a new particle here
			if (particles.size() <= particleMaxCount) {
				float particleX = ofMap(x, 0, videoWidth, 0, ofGetWidth());
				float particleY = ofMap(y, 0, videoHeight, 0, ofGetHeight());
				addParticle(particleX, particleY, vel.x, vel.y);
			}
		}
	}
	
	// check for particles that have reached the end of their lifetime and remove them
    for (vector<particle>::iterator p=particles.begin(); p!=particles.end(); ) {
        if (p->dead) {
            p = particles.erase(p);
        } else {
            p++;
        }
    }
	particleCount = particles.size();
	
	// update not-dead particle forces from field
	for (vector<particle>::iterator p=particles.begin(); p!=particles.end(); p++) {
		// get the force from the vector field:
		ofVec2f frc;
		frc = field.getForceFromPos(p->pos.x, p->pos.y);
		p->addForce(frc.x, frc.y);
		//p->vel.limit(5);
		p->addDampingForce();
		p->update();
	}
	
	// fade forces in field
	field.fadeField(0.9f);
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if (drawVideoDebug) {
		ofSetColor(255);
		grayImage.draw(0, 0, videoWidth * 0.5, videoHeight * 0.5);
		//grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	}

	if (drawFlowSolver) {
		flowSolver.drawColored(videoWidth * 0.5, videoHeight * 0.5, 5, 3);
		//flowSolver.drawColored(ofGetWidth(), ofGetHeight(), 10, 3);
	}
	
	if (drawVectorField) {
		field.draw();
	}
	
	if (drawImageDiff) {
		ofSetColor(100);
		//grayImageDiff.draw(0, 0, ofGetWidth(), ofGetHeight());
		grayImageDiffHistory.draw(0, 0, ofGetWidth(), ofGetHeight());
		//contourFinder.draw(0, 0, ofGetWidth(), ofGetHeight());
		
		/*
		for (int i = 0; i < contourFinder.nBlobs; i++) {
			ofSetColor(255, 255, 0);
			ofBeginShape();
			for (int j = 0; j < contourFinder.blobs.at(i).nPts; j++) {
				ofVertex(contourFinder.blobs.at(i).pts[j].x, contourFinder.blobs.at(i).pts[j].y);
			}
			ofEndShape();
			ofSetColor(255);
		}
		 */
	}
	
	if (drawParticles) {
		
		//ofNoFill();
		//ofBeginShape();
		
		for (int i = 0; i < particles.size(); i++){
			
			float hue, bri;
			
			if (particleColorBasedOnDirection) {
				hue = ofMap(atan2(particles[i].vel.y, particles[i].vel.x), -PI, PI, 0, 255);
			} else {
				hue = ofMap(particles[i].pos.x * particles[i].pos.y, 0, ofGetWidth() * ofGetHeight(), 0, 255);
			}
			
			if (particleFade) {
				bri = (1 - powf(particles[i].age / (float) particles[i].lifespan, 2)) * 255;
			} else {
				bri = 255;
			}
			
			ofSetColor(ofColor::fromHsb(hue, 225, bri));
			particles[i].draw();

			//ofVertex(particles[i].pos.x, particles[i].pos.y);
			//ofCurveVertex(particles[i].pos.x, particles[i].pos.y);
			
			/*
			 if (i % 2 == 0) {
			 ofEndShape();
			 ofBeginShape();
			 }
			 */
			
		}
		
		//ofEndShape();
	}
	
	
	// debug for optical flow solver
    /*
	 stringstream m;
	 m << "fps " << ofGetFrameRate() << endl
	 << "pyramid scale: " << flowSolver.getPyramidScale() << " p/P" << endl
	 << "pyramid levels: " << flowSolver.getPyramidLevels() << " l/L" << endl
	 << "averaging window size: " << flowSolver.getWindowSize() << " w/W" << endl
	 << "iterations per level: " << flowSolver.getIterationsPerLevel() << " i/I" << endl
	 << "expansion area: " << flowSolver.getExpansionArea() << " a/A" << endl
	 << "expansion sigma: " << flowSolver.getExpansionSigma() << " s/S" << endl
	 << "flow feedback: " << flowSolver.getFlowFeedback() << " f/F" << endl
	 << "gaussian filtering: " << flowSolver.getGaussianFiltering() << " g/G";
	 
	 ofDrawBitmapString(m.str(), 20, 20);
	 */
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	
	if (name == "Use Live Video") {
		
		// if live video is off, switch to video player
		if (!useLiveVideo) {
			
			// close the camera stream
			//vidGrabber.close();

			setupPlayer();
			if (!vidPlayer.isPlaying()) {
				vidPlayer.play();
			}
		}
		
		// if live video is on, stop player and switch to cam
		if (useLiveVideo) {
			
			if (vidPlayer.isPlaying()) {
				vidPlayer.stop();
			}
			
			setupGrabber();
		}
		
		setupSolverAndField();
	}
	
	if (name == "Mirror Video") {
		flowSolver.setMirror(mirrorVideo, false);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	// play/pause video
	if (key == ' '){
		if (vidPlayer.isPlaying()) vidPlayer.stop();
		else vidPlayer.play();
	}
	
	if (key == 'g') {
		gui->toggleVisible();
	}

	// settings for optical flow solver
	/*
	 if(key == 'p') { flowSolver.setPyramidScale(ofClamp(flowSolver.getPyramidScale() - 0.01,0.0,1.0)); }
	 else if(key == 'P') { flowSolver.setPyramidScale(ofClamp(flowSolver.getPyramidScale() + 0.01,0.0,1.0)); }
	 else if(key == 'l') { flowSolver.setPyramidLevels(MAX(flowSolver.getPyramidLevels() - 1,1)); }
	 else if(key == 'L') { flowSolver.setPyramidLevels(flowSolver.getPyramidLevels() + 1); }
	 else if(key == 'w') { flowSolver.setWindowSize(MAX(flowSolver.getWindowSize() - 1,1)); }
	 else if(key == 'W') { flowSolver.setWindowSize(flowSolver.getWindowSize() + 1); }
	 else if(key == 'i') { flowSolver.setIterationsPerLevel(MAX(flowSolver.getIterationsPerLevel() - 1,1)); }
	 else if(key == 'I') { flowSolver.setIterationsPerLevel(flowSolver.getIterationsPerLevel() + 1); }
	 else if(key == 'a') { flowSolver.setExpansionArea(MAX(flowSolver.getExpansionArea() - 2,1)); }
	 else if(key == 'A') { flowSolver.setExpansionArea(flowSolver.getExpansionArea() + 2); }
	 else if(key == 's') { flowSolver.setExpansionSigma(ofClamp(flowSolver.getExpansionSigma() - 0.01,0.0,10.0)); }
	 else if(key == 'S') { flowSolver.setExpansionSigma(ofClamp(flowSolver.getExpansionSigma() + 0.01,0.0,10.0)); }
	 else if(key == 'f') { flowSolver.setFlowFeedback(false); }
	 else if(key == 'F') { flowSolver.setFlowFeedback(true); }
	 else if(key == 'g') { flowSolver.setGaussianFiltering(false); }
	 else if(key == 'G') { flowSolver.setGaussianFiltering(true); }
	 */
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}