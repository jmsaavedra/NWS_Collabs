//
//  Mode3.cpp
//  NWS_Assembled
//
//  Created by Matt on 11/7/13.
//
//

#include "Mode3.h"

//--------------------------------------------------------------
void Mode3::setup(){

    ofBackground(0);
    //ofSetBackgroundAuto(false);
    ofSetVerticalSync(true);
    
    xNoiseValue=0.0025;
    yNoiseValue=0.0025;
    zNoiseValue=0.001;
    xNoiseScale=10;
    yNoiseScale=10;
    xAngleOffset=0;
    yAngleOffset=0;
    speedScale = 0.2;
	
    
    setupGUI();
	
	
}

void Mode3::setupGUI(){
    gui = new ofxUICanvas();
    gui -> addLabel("Control Panel");
    gui -> addSpacer();
    gui -> addFPSSlider("FPS");
    gui -> addSpacer();
    
    gui -> addLabel("Noise Values");
    gui -> addSlider("x", 0.0001, 0.1, &xNoiseValue);
    gui -> addSlider("y", 0.0001, 0.1, &yNoiseValue);
    gui -> addSlider("z", 0.0001, 0.1, &zNoiseValue);
    gui -> addSpacer();
    
    gui -> addLabel("Noise Scaling");
    gui -> addSlider("x", 1, 20, &xNoiseScale);
    gui -> addSlider("y", 1, 20, &yNoiseScale);
    gui -> addSpacer();
    
    gui -> addLabel("Angle Offset");
    gui -> addSlider("x", 0, TWO_PI, &xAngleOffset);
    gui -> addSlider("y", 0, TWO_PI, &yAngleOffset);
    gui ->addSpacer();
    
    gui -> addSlider("Speed Scale", 0, 3, speedScale);
    
    gui ->autoSizeToFitWidgets();
    
    ofAddListener(gui->newGUIEvent,this,&Mode3::guiEvent);
    
}

//--------------------------------------------------------------
void Mode3::update(FFTOctaveAnalyzer *FFTanalyzer){
    pos.x=ofRandom(ofGetWindowWidth());
    pos.y=ofRandom(ofGetWindowHeight());
    vel=ofRandom(5,7.5);
    hue=ofRandom(255);
	
    for ( vector<ParticleJ>::iterator it = pList.begin(); it!=pList.end(); ) {
        
		it->update(speedScale * *FFTanalyzer->peaks); //adjust 1.5 based on overall volume level
		
        
        
        if ( it->isDead==TRUE ) {
            it = pList.erase(it);
        }
        else {
            it++;
        }
	}
	//    cout<< *FFTanalyzer.peaks<<endl;
	// cout<< speedScale<<endl;
	
    
}

//--------------------------------------------------------------
void Mode3::draw(){
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetColor(0, 25);
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    float avg_power = 0.0f;

	
    for ( vector<ParticleJ>::iterator it=pList.begin(); it!=pList.end(); ) {
        it->draw();
        it++;
    }
    int countSize=pList.size();
    
    if (countSize<2500) {
        
		
        for (int i=0; i<50; i++) {
			
            addParticle(pos, vel,hue);
			
        }
    }
    
	
	
}

void Mode3::guiEvent(ofxUIEventArgs &e){
    
    string name = e.widget->getName();
	int kind = e.widget->getKind();
	
    if(name == "Speed Scale")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		speedScale = slider->getScaledValue();
        cout << "value: " << slider->getScaledValue() << endl;
	}
	
}

void Mode3::addParticle(ofVec2f pos, float vel,float hue){
    ParticleJ tmp;
    tmp.setup(pos, vel,hue);
    
    tmp.passValues(&xNoiseValue, &yNoiseValue, &zNoiseValue, &xNoiseScale, &yNoiseScale, &xAngleOffset, &yAngleOffset);
    
    pList.push_back( tmp );
}


//--------------------------------------------------------------
void Mode3::keyPressed(int key){
	if (key == 'g') {
		gui->toggleVisible();
	}
}

//--------------------------------------------------------------
void Mode3::keyReleased(int key){
	
}

//--------------------------------------------------------------
void Mode3::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void Mode3::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode3::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode3::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void Mode3::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void Mode3::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void Mode3::dragEvent(ofDragInfo dragInfo){
	
}
