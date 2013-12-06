#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    video.initGrabber(ofGetWindowWidth(), ofGetWindowHeight());
    video.setDesiredFrameRate(30);
    width = video.width;
    height = video.height;
    videoColorCvImage.allocate(width, height);
    ofBackground(0);
    
    for (int i=0; i<20000; i++) {
        addParticle();
    }
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    video.update();
    if (video.isFrameNew()){
        ofTexture texture;
        texture = video.getTextureReference();
        texture.readToPixels(pixels);
//        videoColorCvImage.setFromPixels(video.getPixels(),width,height);
        
    }
    
    for(int i = 0; i < pList.size(); i++){
		pList[i].update();
		
		pList[i].color.set(pixels.getColor(pList[i].pos.x,pList[i].pos.y));
	}

}

//--------------------------------------------------------------
void testApp::draw(){
  // videoColorCvImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    
    for (int i=0; i<pList.size(); i++) {
        pList[i].draw();
    }
}

void testApp::addParticle(){
    Particle tmp;
    tmp.setup();
    pList.push_back(tmp);
    
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
