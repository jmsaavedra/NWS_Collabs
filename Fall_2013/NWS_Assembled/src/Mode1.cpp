//
//  Mode1.cpp
//  NWS_Assembled
//
//  Created by Matt on 11/7/13.
//
//

#include "Mode1.h"

//--------------------------------------------------------------
void Mode1::setup(){
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(0);
	
    ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	// set program defaults
	useLiveVideo = true;
	mirrorVideo = false;
	drawVideo = true;
	drawVideoFullscreen = true;
	drawFlowSolver = false;
	drawFlowSolverFullscreen = true;
	drawVectorField = false;
	drawImageDiff = false;
	drawParticles = false;
	growParticles = false;
	growParticleSaturation = false;
	particleMaxCount = 3000;
	particleSize = 0;
	particleColorBasedOnDirection = true;
	particleSaturation = 30.0;
	particleFade = true;
	minimumVelocity = 1.0;
	videoBlurAmount = 200.0;
	backgroundTransparency = 1.0;
	increaseBlurAndDim = false;
	decreaseBlurAndDim = false;
	increaseParticleSaturation = false;
	imageDiffThreshold = 30.0;
	imageDiffBlur = 40.0;
	
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
	
}

void Mode1::setupGrabber() {
	// define size for input video
	videoWidth = 640;
	videoHeight = 480;
	
	// set up video camera
	if (!vidGrabber.isInitialized()) {
		vidRecorder = ofPtr<ofQTKitGrabber>( new ofQTKitGrabber() );
		vidGrabber.setGrabber(vidRecorder);
		vidGrabber.initGrabber(videoWidth, videoHeight);
		vidRecorder->setVideoDeviceID(vidRecorder->getVideoDeviceID() + 14);
	}
}

//void Mode1::setupPlayer(string filename) {
void Mode1::setupPlayer(int index) {
	
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

void Mode1::setupSolverAndField() {
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

void Mode1::setupGUI() {
	
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
	
	gui->addLabel("View & Fields");
	gui->addToggle("Draw Video", &drawVideo);
	gui->addToggle("Draw Video Fullscreen", &drawVideoFullscreen);
	gui->addToggle("Draw Flow Solver", &drawFlowSolver);
	gui->addToggle("Draw Flow Fullscreen", &drawFlowSolverFullscreen);
	gui->addToggle("Draw Vector Field", &drawVectorField);
    gui->addSpacer();
	
	gui->addLabel("Particle Settings");
	gui->addSlider("Particle Max Count", 3000, 10000, &particleMaxCount);
	gui->addSlider("Particle Count", 0, 10000, &particleCount);
	gui->addToggle("Color Based on Direction", &particleColorBasedOnDirection);
	//gui->addToggle("Fade Brightness", &particleFade);
	gui->addSlider("Minimum Velocity", 0, 5, &minimumVelocity);
	gui->addSpacer();
	
	gui->addLabel("Effects");
	gui->addToggle("Draw Image Diff", &drawImageDiff);
	gui->addToggle("Draw Particles", &drawParticles);
	gui->addLabelToggle("Grow Particles", &growParticles);
	gui->addSlider("Particle Size", 0, 5, &particleSize);
	gui->addLabelToggle("Grow Saturation", &growParticleSaturation);
	gui->addSlider("Color Saturation", 0, 255, &particleSaturation);
	gui->addSlider("Video Blur Amount", 0, 200, &videoBlurAmount);
	gui->addSlider("Background Transparency", 0, 1, &backgroundTransparency);
	gui->addSlider("Image Diff Threshold", 0, 100, &imageDiffThreshold);
	gui->addSlider("Image Diff Blur", 0, 100, &imageDiffBlur);
	gui->addToggle("Increase Blur/Dim", &increaseBlurAndDim);
	gui->addToggle("Decrease Blur/Dim", &decreaseBlurAndDim);
	gui->addToggle("Increase Particle Saturation", &increaseParticleSaturation);
	//gui->addSpacer();
	
	//gui->addLabel("Press 'g' to toggle GUI");
    gui->autoSizeToFitWidgets();
	gui->setPosition(ofGetWidth() - 211, 0);
	
	ofAddListener(gui->newGUIEvent,this,&Mode1::guiEvent);
	
}


//--------------------------------------------------------------
void Mode1::addParticle(float x, float y, float vx, float vy) {
    
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
void Mode1::update(FFTOctaveAnalyzer *FFTanalyzer){
	
	bool isFrameNew = false;
	
	// set xeno/interpolation speed for changing between presets
	float lirpSpeed = 0.03;

	// update all animatable preset properties if the current vals are different
	// from the specified target. also check whether target has been set/set it to false
	// so that the sliders are draggable (otherwise they'd animate back to their preset
	// target values once you let go of the slider)
	if (videoBlurAmount != videoBlurAmountTarget && videoBlurAmountTarget) {
		videoBlurAmount = videoBlurAmount * (1-lirpSpeed) + videoBlurAmountTarget * lirpSpeed;
	} else {
		videoBlurAmountTarget = false;
	}
	
	if (backgroundTransparency != backgroundTransparencyTarget && backgroundTransparencyTarget) {
		backgroundTransparency = backgroundTransparency * (1-lirpSpeed) + backgroundTransparencyTarget * lirpSpeed;
	} else {
		backgroundTransparencyTarget = false;
	}
	
	if (imageDiffThreshold != imageDiffThresholdTarget && imageDiffThresholdTarget) {
		imageDiffThreshold = imageDiffThreshold * (1-lirpSpeed) + imageDiffThresholdTarget * lirpSpeed;
	} else {
		imageDiffThresholdTarget = false;
	}
	
	if (imageDiffBlur != imageDiffBlurTarget && imageDiffBlurTarget) {
		imageDiffBlur = imageDiffBlur * (1-lirpSpeed) + imageDiffBlurTarget * lirpSpeed;
	} else {
		imageDiffBlurTarget = false;
	}
	
//	if (particleSize != particleSizeTarget && particleSizeTarget) {
//		particleSize = particleSize * (1-lirpSpeed) + particleSizeTarget * lirpSpeed;
//	} else {
//		particleSizeTarget = false;
//	}
	
	if (growParticles) {
		float particleSizeStepSize = 0.003;
		
		if (particleSizeTarget) {
			if (abs(particleSize - particleSizeTarget) >= particleSizeStepSize) {
				if (particleSize < particleSizeTarget) {
					particleSize += particleSizeStepSize;
				} else {
					particleSize -= particleSizeStepSize;
				}
				if (!drawParticles) drawParticles = true;
			} else {
				growParticles = false;
				particleSizeTarget = false;
			}
		}
	}


	if (growParticleSaturation) {
		float particleSaturationStepSize = 0.07;
		
		if (particleSaturationTarget) {
			if (abs(particleSaturation - particleSaturationTarget) >= particleSaturationStepSize) {
				if (particleSaturation < particleSaturationTarget) {
					particleSaturation += particleSaturationStepSize;
				} else {
					particleSaturation -= particleSaturationStepSize;
				}
				if (!drawParticles) drawParticles = true;
			} else {
				growParticleSaturation = false;
				particleSaturationTarget = false;
			}
		}
	}
	
	// slowly increase blur amount & set background brightness/dimming to
	// half of that value
	if (increaseBlurAndDim) {
		videoBlurAmount += 0.05;
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	if (decreaseBlurAndDim) {
		videoBlurAmount -= 0.05;
		if (videoBlurAmount <= 0) {
			videoBlurAmount = 0;
			decreaseBlurAndDim = false;
		}
		backgroundTransparency = ofMap(videoBlurAmount, 0, 200, 0, 0.5);
	}
	
	// slowly increase particle saturation from white -> color
	if (increaseParticleSaturation) {
		particleSaturation += 0.02;
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
	
	blue = int(ofMap(FFTanalyzer->averages[3], 0, 20, 0, 255));
    red = int(ofMap(FFTanalyzer->averages[8], 0, 100, 0, 250));
}

//--------------------------------------------------------------
void Mode1::draw(){

	ofDisableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	
	ofBackground(0);
	
	ofSetColor(0);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
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
				//hue = ofMap(particles[i].pos.x * particles[i].pos.y, 0, ofGetWidth() * ofGetHeight(), 0, 255);
				ofColor c = ofColor(red, 29 ,blue, 30);
				hue = c.getHue();
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
			particles[i].draw(particleSize);
			
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
void Mode1::guiEvent(ofxUIEventArgs &e)
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
	
	if (name == "Increase Blur/Dim") {
		decreaseBlurAndDim = false;
	}
	if (name == "Decrease Blur/Dim") {
		increaseBlurAndDim = false;
	}
	
	if (name == "Grow Particles") {
		if (growParticles) {
			drawParticles = true;
		}
		particleSizeTarget = 1.6;
	}
	if (name == "Grow Saturation") {
		if (growParticleSaturation) {
			drawParticles = true;
		}
		particleSaturationTarget = 235;
	}
	
	if (name == "Video Blur Amount") {
		videoBlurAmountTarget = videoBlurAmount;
	}
	if (name == "Background Transparency") {
		backgroundTransparencyTarget = backgroundTransparency;
	}
	if (name == "Image Diff Threshold") {
		imageDiffThresholdTarget = imageDiffThreshold;
	}
	if (name == "Image Diff Blur") {
		imageDiffBlurTarget = imageDiffBlur;
	}
}

//--------------------------------------------------------------
void Mode1::keyPressed(int key){
	
	// play/pause video
//	if (key == ' '){
//		if (vidPlayers[videoPlayerIndex].isPlaying()) vidPlayers[videoPlayerIndex].stop();
//		else vidPlayers[videoPlayerIndex].play();
//	}
	
	if (key == 'g') {
		gui->toggleVisible();
	}
	
//	if (key == '-') {
//		setupPlayer(0);
//		setupSolverAndField();
//	}
//	if (key == '=') {
//		setupPlayer(1);
//		setupSolverAndField();
//	}

	if (key == '0') {
		videoBlurAmountTarget = 0.01;
		backgroundTransparencyTarget = 0.01;
		//imageDiffThresholdTarget = 5;
		//imageDiffBlurTarget = 41;
	}
	
	// presets for background brightness & dimming
	if (key == '1') {
		videoBlurAmountTarget = 5;
		backgroundTransparencyTarget = 0.2;
		//imageDiffThresholdTarget = 15;
		//imageDiffBlurTarget = 5;
	}
	if (key == '2') {
		videoBlurAmountTarget = 18;
		backgroundTransparencyTarget = 0.3;
		//imageDiffThresholdTarget = 15;
		//imageDiffBlurTarget = 5;
	}
	if (key == '3') {
		videoBlurAmountTarget = 10;
		backgroundTransparencyTarget = 0.5;
		//imageDiffThresholdTarget = 15;
		//imageDiffBlurTarget = 5;

	}
	if (key == '4') {
		videoBlurAmountTarget = 200;
		backgroundTransparencyTarget = 1.0;
		//imageDiffThresholdTarget = 15;
		//imageDiffBlurTarget = 5;
	}
	
	// presets for image blur as crisp or really blurry & blobby
	if (key == '7') {
		//imageDiffThresholdTarget = 15;
		imageDiffBlurTarget = 5;
	}
	if (key == '8') {
		//imageDiffThresholdTarget = 30;
		imageDiffBlurTarget = 9;
	}
	if (key == '9') {
		//imageDiffThresholdTarget = 5;
		imageDiffBlurTarget = 41;
	}

	if (key == '-') {
		imageDiffThresholdTarget = 30;
	}
	if (key == '=') {
		imageDiffThresholdTarget = 10;
	}
	
	// toggle certain features
	if (key == 'd') {
		drawImageDiff = !drawImageDiff;
		if (drawImageDiff) {
			grayImagePrev = grayImage;
		}
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
}

//--------------------------------------------------------------
void Mode1::keyReleased(int key){
	
}

//--------------------------------------------------------------
void Mode1::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void Mode1::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode1::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode1::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode1::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void Mode1::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void Mode1::dragEvent(ofDragInfo dragInfo){
	
}