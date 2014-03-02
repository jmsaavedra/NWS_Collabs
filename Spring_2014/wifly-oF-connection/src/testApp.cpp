#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//we run at 60 fps!
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
    
    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);
    
	ofSetBackgroundAuto(false);
	ofBackground(255, 255, 255);
}

//--------------------------------------------------------------
void testApp::update(){
    
	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
		cout << message << endl;
		accel.clear();
        magne.clear();
		float aX,aY,aZ,mX,mY,mZ;
		vector<string> strPoints = ofSplitString(message,"[/p]");
		for(unsigned int i=0;i<strPoints.size();i++){
			vector<string> point = ofSplitString(strPoints[i],"\t");
			if( point.size() == 7 ){
				dataID=atof(point[0].c_str());
				aX=atof(point[1].c_str());
				aY=atof(point[2].c_str());
                aZ=atof(point[3].c_str());
                cout << "Accel " << aX << ", " << aY << ", " << aZ << endl;
                accel.push_back(ofPoint(aX,aY,aZ));
                mX=atof(point[4].c_str());
				mY=atof(point[5].c_str());
                mZ=atof(point[6].c_str());
                cout << "Magne " << mX << ", " << mY << ", " << mZ << endl;
                magne.push_back(ofPoint(mX,mY,mZ));
			}
		}
	}
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    
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
