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
	drawVideo = true;
	drawVideoFullscreen = true;
	drawFlowSolver = false;
	drawFlowSolverFullscreen = false;
	drawVectorField = false;
	drawImageDiff = false;
	drawParticles = false;
	particleMaxCount = 3000;
	particleColorBasedOnDirection = true;
	particleSaturation = 0.0;
	particleFade = true;
	minimumVelocity = 1.0;
	videoBlurAmount = 0.0;
	backgroundTransparency = 0.0;
	increaseBlurAndDim = false;
	imageDiffThreshold = 30.0;
	imageDiffBlur = 9.0;

	// set up scaling factors for flow solver & vector field
	flowSolverScale.set(0.5, 0.5);
	vectorFieldScale.set(0.1, 0.1);
	
	// set up cam or video player
	videoPlayerIndex = 0;
	if (useLiveVideo) {
		setupGrabber();
	} else {
		setupPlayer();
	}
	
	// set up optical flow solver & vector field based on settings from above
	setupSolverAndField();
	
	// set up gui
	setupGUI();
	
	// load image for particles
	particleImage.loadImage("butterfly.png");
	
	// set up syphon server
	syphon.setName("hands");

}

void testApp::setupGrabber() {

	// define size for input video
	videoWidth = 640;
	videoHeight = 480;
	
	// set up video camera
	if (!vidGrabber.isInitialized()) {
		vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
		vidGrabber.setGrabber(vidRecorder);
		vidGrabber.initGrabber(videoWidth, videoHeight);
		vidRecorder->setVideoDeviceID(vidRecorder->getVideoDeviceID() + 0);
	}
}

//void testApp::setupPlayer(string filename) {
void testApp::setupPlayer(int index) {
	
	// most of this code was removed after move from one vidPlayer to a vector
	// of players to simulate multiple camera angles in development
	
	// define size for video file
	//videoWidth = 480;
	//videoHeight = 360;
	
	/*
	if (vidPlayer.isLoaded()) {
		if (vidPlayer.getMoviePath() != filename) {
			vidPlayer.close();
		}
	}
	
	// set up video player
	if (!vidPlayer.isLoaded()) {
		//vidPlayer.loadMovie("topview.mp4");
		//vidPlayer.loadMovie("sideview.mov");
		vidPlayer.loadMovie(filename);
		//cout << vidPlayer.getMoviePath() << endl;
	}

	 videoWidth = vidPlayer.width;
	 videoHeight = vidPlayer.height;
	 
	 vidPlayer.setVolume(0);
	 
	 */

	if (vidPlayers.size() == 0) {
		p1.loadMovie("sideview.mov");
		p1.setVolume(0);
		vidPlayers.push_back(p1);
		
		p2.loadMovie("topview.mp4");
		p2.setVolume(0);
		vidPlayers.push_back(p2);
	}
	
	videoPlayerIndex = index;
	
	videoWidth = vidPlayers[videoPlayerIndex].width;
	videoHeight = vidPlayers[videoPlayerIndex].height;
	
	// play current video & pause others
	for (int i = 0; i < vidPlayers.size(); i++) {
		if (i == videoPlayerIndex) {
			if (vidPlayers[i].isPaused()) {
				vidPlayers[i].play();
			}
		} else {
			if (vidPlayers[i].isPlaying()) {
				vidPlayers[i].stop();
			}
		}
	}
}

void testApp::setupSolverAndField() {
	// set up optical flow solver at 1/2 of video size
	flowSolver.setup(videoWidth * flowSolverScale.x, videoHeight * flowSolverScale.y, 0.5, 3, 10, 1, 7, 1.5, false, false);
	flowSolver.setMirror(mirrorVideo, false);
	
	// set up vector field at 1/10th of video size
	field.setupField(videoWidth * vectorFieldScale.x, videoHeight * vectorFieldScale.y, ofGetWidth(), ofGetHeight());
	particles.clear();
	
	colorImage.clear();
	colorImageBlurred.clear();
	grayImage.clear();
	grayImagePrev.clear();
	grayImageDiff.clear();
	
	colorImage.allocate(videoWidth, videoHeight);
	colorImageBlurred.allocate(videoWidth, videoHeight);
	grayImage.allocate(videoWidth, videoHeight);
	grayImagePrev.allocate(videoWidth, videoHeight);
	grayImageDiff.allocate(videoWidth, videoHeight);
	grayImageDiffHistory.allocate(videoWidth, videoHeight);
}

void testApp::setupGUI() {

	// set up file names for video toggle
	vector <string> files;
	files.push_back("sideview.mov");
	files.push_back("topview.mp4");
	
	// build the gui
	gui = new ofxUICanvas();
    gui->addLabel("HANDS CONTROL");
    gui->addSpacer();
    gui->addFPSSlider("FPS");
    gui->addSpacer();

	gui->addLabel("Video Input");
	gui->addToggle("Use Live Video", &useLiveVideo);
	gui->addRadio("Video File", files);
	gui->addToggle("Mirror Video", &mirrorVideo);
    gui->addSpacer();

	gui->addLabel("Debug View Drawing");
	gui->addToggle("Draw Video", &drawVideo);
	gui->addToggle("Draw Video Fullscreen", &drawVideoFullscreen);
	gui->addToggle("Draw Flow Solver", &drawFlowSolver);
	gui->addToggle("Draw Flow Fullscreen", &drawFlowSolverFullscreen);
	gui->addToggle("Draw Vector Field", &drawVectorField);
	gui->addToggle("Draw Image Diff", &drawImageDiff);
	gui->addToggle("Draw Particles", &drawParticles);
    gui->addSpacer();

	gui->addLabel("Particle Settings");
	gui->addSlider("Particle Max Count", 1000, 10000, &particleMaxCount);
	gui->addSlider("Particle Count", 0, 10000, &particleCount);
	gui->addToggle("Color Based on Direction", &particleColorBasedOnDirection);
	gui->addSlider("Color Saturation", 0, 255, &particleSaturation);
	gui->addToggle("Fade Brightness", &particleFade);
	gui->addSlider("Minimum Velocity", 0, 5, &minimumVelocity);
	gui->addSlider("Video Blur Amount", 0, 200, &videoBlurAmount);
	gui->addSlider("Background Transparency", 0, 1, &backgroundTransparency);
	gui->addToggle("Auto Increase Blur/Dim", &increaseBlurAndDim);
	gui->addSlider("Image Diff Threshold", 0, 100, &imageDiffThreshold);
	gui->addSlider("Image Diff Blur", 0, 100, &imageDiffBlur);
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
	
	bool isFrameNew = false;
	
	// slowly increase blur amount & set background brightness/dimming to
	// half of that value
	if (increaseBlurAndDim) {
		videoBlurAmount += 0.025;
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	
	// update flow solver & cv color image from new video frame, whether from
	// video camera or video player
	if (useLiveVideo) {
		
		vidGrabber.update();
		
		if (vidGrabber.isFrameNew()){
			isFrameNew = true;
			
			flowSolver.update(vidGrabber);
			colorImage.setFromPixels(vidGrabber.getPixels(), videoWidth, videoHeight);

			//contourFinder.findContours(grayImageDiff, 5, 76800, 4, false);
		}
	} else {
		
		//vidPlayer.update();
		vidPlayers[videoPlayerIndex].update();
		
		//if (vidPlayer.isFrameNew()){
		if (vidPlayers[videoPlayerIndex].isFrameNew()){
			isFrameNew = true;
			
			//flowSolver.update(vidPlayer);
			flowSolver.update(vidPlayers[videoPlayerIndex]);
			
			//colorImage.setFromPixels(vidPlayer.getPixels(), videoWidth, videoHeight);
			colorImage.setFromPixels(vidPlayers[videoPlayerIndex].getPixels(), videoWidth, videoHeight);
			
			//contourFinder.findContours(grayImageDiff, 5, 76800, 1, false);
		}
	}
	
	// do some processing if we've got a new frame
	if (isFrameNew) {
		// flip image horizontally if the mirroVideo checkbox is ticked
		colorImage.mirror(false, mirrorVideo);
		grayImage = colorImage;
		
		// blur the color image
		if (videoBlurAmount) colorImage.blur(videoBlurAmount);
		
		// dim the image based on the background transparency setting
		if (backgroundTransparency) {
			colorImage -= 255 * backgroundTransparency;
		}
		
		// take difference of current & previous frames in order to get
		// white contour lines. blur it so it's nice & soft and add it
		// to the history image so we get some nice ghosting/motion blur
		if (drawImageDiff) {
			grayImageDiff.absDiff(grayImage, grayImagePrev);
			grayImageDiff.threshold(imageDiffThreshold);
			
			grayImageDiff.blur();
			grayImagePrev = grayImage;
			
			grayImageDiffHistory -= 30;
			grayImageDiffHistory += grayImageDiff;
			grayImageDiffHistory.blur(imageDiffBlur);
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
	
	particleCount = particles.size();
	
	// check for particles that have reached the end of their lifetime and remove them
    for (vector<particle>::iterator p=particles.begin(); p!=particles.end(); ) {
        if (p->dead) {
            p = particles.erase(p);
        } else {
            p++;
        }
    }
	
	
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
	
	// draw the video image
	if (drawVideo) {
		ofSetColor(255);
		if (!drawVideoFullscreen) {
			grayImage.draw(0, 0, videoWidth * 0.5, videoHeight * 0.5);
		} else {
			//grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
			colorImage.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
	}
	
	// draw the optical flow solver
	if (drawFlowSolver) {
		if (!drawFlowSolverFullscreen) {
			flowSolver.drawColored(videoWidth * 0.5, videoHeight * 0.5, 5, 3);
		} else {
			flowSolver.drawColored(ofGetWidth(), ofGetHeight(), 20, 5);
		}
	}
	
	// draw the vector field
	if (drawVectorField) {
		field.draw();
	}
	
	// draw the image diff (white blurred contour lines)
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
	
	// draw the particles
	if (drawParticles) {
		
		//ofNoFill();
		//ofBeginShape();
		
		for (int i = 0; i < particles.size(); i++){
			
			float hue, bri;
			
			// calculate the particle's color either based on its direction or
			// based on its position on the screen
			if (particleColorBasedOnDirection) {
				hue = ofMap(atan2(particles[i].vel.y, particles[i].vel.x), -PI, PI, 0, 255);
			} else {
				hue = ofMap(particles[i].pos.x * particles[i].pos.y, 0, ofGetWidth() * ofGetHeight(), 0, 255);
			}
			
			// calculate the particle's brightness as either fully bright or fading based on
			// its age. for fading, a percentage of its age vs. its lifespan is calculated and
			// then squared so it stays brighter for longer, and then fades quicker instead of
			// gradually across its entire life
			if (particleFade) {
				bri = (1 - powf(particles[i].age / (float) particles[i].lifespan, 2)) * 255;
			} else {
				bri = 255;
			}
			
			ofSetColor(ofColor::fromHsb(hue, particleSaturation, bri));
			particles[i].draw();
			//particles[i].draw(&particleImage);

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
	
	syphon.publishScreen();
	
	
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
	int kind = e.widget->getKind();
	
	if (name == "Use Live Video") {
		
		// if live video is off, switch to video player
		if (!useLiveVideo) {
						
//			if (!vidPlayer.isPlaying()) {
//				vidPlayer.play();
//			}
			setupPlayer(videoPlayerIndex);
//			if (!vidPlayers[videoPlayerIndex].isPlaying()) {
//				vidPlayers[videoPlayerIndex].play();
//			}
		}
		
		// if live video is on, stop player and switch to cam
		if (useLiveVideo) {
			
//			if (vidPlayer.isPlaying()) {
//				vidPlayer.stop();
//			}
			if (vidPlayers[videoPlayerIndex].isPlaying()) {
				vidPlayers[videoPlayerIndex].stop();
			}
			
			setupGrabber();
		}
		
		setupSolverAndField();
	}
	
	if (name == "Mirror Video") {
		flowSolver.setMirror(mirrorVideo, false);
	}
	
	if (name == "sideview.mov") {
		useLiveVideo = false;
		setupPlayer(0);
		setupSolverAndField();
	}
	if (name == "topview.mp4") {
		useLiveVideo = false;
		setupPlayer(1);
		setupSolverAndField();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	// play/pause video
	if (key == ' '){
		if (vidPlayers[videoPlayerIndex].isPlaying()) vidPlayers[videoPlayerIndex].stop();
		else vidPlayers[videoPlayerIndex].play();
	}
	
	if (key == 'g') {
		gui->toggleVisible();
	}
	
	if (key == '=') {
		setupPlayer(0);
		setupSolverAndField();
	}
	if (key == '-') {
		setupPlayer(1);
		setupSolverAndField();
	}
	
	if (key == '1') {
		minimumVelocity = 0.5;
		videoBlurAmount = 30;
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	if (key == '2') {
		minimumVelocity = 1.0;
		videoBlurAmount = 70;
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	if (key == '3') {
		minimumVelocity = 2.0;
		videoBlurAmount = 100;
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	
	if (key == 's') {
		imageDiffThreshold = 4;
		imageDiffBlur = 4;
	}
	if (key == 'n') {
		imageDiffThreshold = 30;
		imageDiffBlur = 9;
	}
	
	if (key == 'd') {
		drawImageDiff = !drawImageDiff;
	}
	if (key == 'p') {
		drawParticles = !drawParticles;
	}
	if (key == 'a') {
		increaseBlurAndDim = !increaseBlurAndDim;
	}
	
//	if (key == '3') {
//		vidGrabber.setDeviceID(0);
//		vidGrabber.initGrabber(videoWidth, videoHeight);
//	}
//	if (key == '4') {
//		vidGrabber.setDeviceID(1);
//		vidGrabber.initGrabber(videoWidth, videoHeight);
//	}
//	if (key == '5') {
//		vidGrabber.setDeviceID(2);
//		vidGrabber.initGrabber(videoWidth, videoHeight);
//	}
	
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