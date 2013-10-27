#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(0);
    ofSetBackgroundAuto(false);
    
    xNoiseValue=0.0025;
    yNoiseValue=0.0025;
    zNoiseValue=0.001;
    xNoiseScale=10;
    yNoiseScale=10;
    xAngleOffset=0;
    yAngleOffset=0;

//    values[0] = &xNoiseValue;
//    values[1] = &yNoiseValue;
//    values[2] = &zNoiseValue;
//    
//    values[3] = &xNoiseScale;
//    values[4] = &yNoiseScale;
//    
//    values[5] = &xAngleOffset;
//    values[6] = &yAngleOffset;
    
    setupGUI();


}

void testApp::setupGUI(){
    gui = new ofxUICanvas();
    gui -> addLabel("Control Panel");
    gui -> addSpacer();
    gui -> addFPSSlider("FPS");
    gui -> addSpacer();
    
    gui -> addLabel("Noise Values");
    gui -> addSlider("x", 0.0001, 0.01, &xNoiseValue);
    gui -> addSlider("y", 0.0001, 0.01, &yNoiseValue);
    gui -> addSlider("z", 0.0001, 0.01, &zNoiseValue);
    gui -> addSpacer();
    
    gui -> addLabel("Noise Scaling");
    gui -> addSlider("x", 1, 20, &xNoiseScale);
    gui -> addSlider("y", 1, 20, &yNoiseScale);
    gui -> addSpacer();
    
    gui -> addLabel("Angle Offset");
    gui -> addSlider("x", 0, TWO_PI, &xAngleOffset);
    gui -> addSlider("y", 0, TWO_PI, &yAngleOffset);
    gui ->addSpacer();
	
	gui->addSlider("Particle Count", 0, 100000, &particleCount);
    
    gui ->autoSizeToFitWidgets();
	gui->toggleVisible();
    
}

//--------------------------------------------------------------
void testApp::update(){
    pos.x=ofRandom(ofGetWindowWidth());
    pos.y=ofRandom(ofGetWindowHeight());
    vel=ofRandom(5,7.5);
    hue=ofRandom(255);

    for ( vector<Particle>::iterator it = pList.begin(); it!=pList.end(); ) {
        it->update();
        
        if ( it->isDead==TRUE ) {
            it = pList.erase(it);
        }
        else {
            it++;
        }
    }

    xNoiseValue = ofMap(cos(ofGetElapsedTimef()/15), -1, 1, 0.005, 0.001);
	yNoiseValue = ofMap(sin(ofGetElapsedTimef()/15 + PI/4), -1, 1, 0.005, 0.001);

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 25);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

    for ( vector<Particle>::iterator it=pList.begin(); it!=pList.end(); ) {
        it->draw();
        it++;
    }
	if (particleCount <= 15000) {
		for (int i=0; i<80; i++) {
			
			addParticle(pos, vel,hue);
			
		}
	}

	particleCount = pList.size();
}

void testApp::addParticle(ofVec2f pos, float vel,float hue){
    Particle tmp;
    tmp.setup(pos, vel,hue);
    //tmp.passValues(values)
    tmp.passValues(&xNoiseValue, &yNoiseValue, &zNoiseValue, &xNoiseScale, &yNoiseScale, &xAngleOffset, &yAngleOffset);
    
    pList.push_back( tmp );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
