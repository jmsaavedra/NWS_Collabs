#include "ofApp.h"


////////////////////
const int N = 1000;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values

int BandAlpha = 50;		//Band index in spectrum, affect something
int BandSize = 10;		//Band index in spectrum, affect something else

//--------------------------------------------------------------
void ofApp::setup(){
    
    my_video.loadMovie("2_4(3).mov");
    my_video.play();
    
    
    
    //image
    ofEnableAlphaBlending();
    img.setAnchorPercent(0.8, 0.8);
    ofBackground(0);
    img.loadImage("colorful-background.jpg");
    w = img.width;
    h = img.height;
    img.allocate(w, h,OF_IMAGE_COLOR);
    
    alpha = 100;
    pixelSize = 10;
    
    //sound
    sound.loadSound( "21 Hollywood Carnival_ Parade of Beauties.mp3" );
    
    sound.setLoop( true );
    sound.play();
    
    //spectrum
    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }


}

//--------------------------------------------------------------
void ofApp::update(){

    
    my_video.update();
    
    alpha = ofMap(spectrum[BandAlpha], 0,0.2, 50, 255);
    pixelSize = ofMap(spectrum[BandSize],0, 0.8, 2, 10);
    
    
   
    ////////////////////////////////////////////////////
    
    ofSoundUpdate();
    
    
    float *val = ofSoundGetSpectrum( N );
    
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.95;	//decreasing number
        spectrum[i] = max( spectrum[i], val[i] );
    }
    
    ////////////////////////////////////////////////////

    ofLog()<<"BandAlpha = " + ofToString(spectrum[BandAlpha])
    + "BandSize =" + ofToString(spectrum[BandSize]);
    

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    my_video.draw(0, 0, w/1.2, h);
    
    // get pixel information
    unsigned char * pixels = img.getPixels();
    
    int skip = 10;

    for (int i=0; i <w; i= i+skip){
        for( int j=0; j<h; j = j+skip){
       
//    int r = pixels[j * w * 5 + i * 5];
//    int g = pixels[j * w * 5 + i * 5 + 100];
//    int b = pixels[j * w * 5 + i * 5 + 200];
//       
            
     int r = pixels[j * w * 3 + i * 3];
     int g = pixels[j * w * 3 + i * 3 + 1];
     int b = pixels[j * w * 3 + i * 3 + 2];
    
     ofColor sampleColor (r,g,b);
    
     float brightness = sampleColor.getBrightness();
     float pct = 1.0 - brightness / 255.0f;  //get dark value
            
    pct += ofMap(ofNoise(i, j, ofGetFrameNum()*0.01)*spectrum[BandAlpha]*10, 0, 1, -3, 3);
            
    ofSetColor(r, g, b,alpha);
   
    // ofSetColor(brightness,brightness,brightness, alpha);
           
    ofSetRectMode(OF_RECTMODE_CENTER); //set rectangle mode to the center
    ofRect(i, j,pixelSize*pct,pixelSize*pct);
            
    //ofCircle(i, j,pixelSize*pct);
            
        }
    }
    
    
    ////////////////////////////////////////////////////
    // Draw spectrum at the left bottom corner for visual reference
    
    ofFill();
    ofSetColor(200);
    for (int i=0; i<N; i++) {
        
        ofRect( 10 + i * 5, 700, 4, -spectrum[i] * 200 );
    }
    ////////////////////////////////////////////////////


}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
