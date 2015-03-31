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
    ofBackground(50);
}

//--------------------------------------------------------------
void testApp::update(){
    float aX,aY,aZ,mX,mY,mZ,dataID;
    
	char udpMessage[100000];
	udpConnection.Receive(udpMessage,100000);
	string message=udpMessage;
	if(message!=""){
//      cout << "raw msg: ";
//		cout << message << endl;

		vector<string> fullMsg = ofSplitString(message,"/");
//        cout << "fullMsg size "<<fullMsg.size() << endl;
//        cout << "fullMsg 0 "<<fullMsg[0] << endl;
//        cout << "fullMsg 1 "<<fullMsg[1] << endl;
        
        if (fullMsg.size() >= 2) { // received a full, correct message
            
            for(int k=1; k < fullMsg.size(); k++){
            vector<string> dataPoints = ofSplitString(fullMsg[k],"\t");
            //cout << "datapoints size: "<<dataPoints.size() << endl;
                if (dataPoints.size() == 7) {
                    dataID = atof(dataPoints[0].c_str());
                    accel.push_back(ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                    magne.push_back(ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));
                    
                    
                    int readingNum = accel.size();
                    cout << "total readings: "<< readingNum;
                    cout << "\tid: "<< dataID; //WHICH UNIT IS THIS ONE
                    cout << "\tAccel: " << accel[accel.size()-1].x << ", " << accel[accel.size()-1].y << ", " << accel[accel.size()-1].z;
                    cout << "\t\tMagne: " << magne[accel.size()-1].x << ", " << magne[accel.size()-1].y << ", " << magne[accel.size()-1].z << endl;
                    
                    recvdDataString = "received from WiFly id# "+ofToString(dataID)
                    + "\n\nAccel: "+ ofToString(accel[accel.size()-1].x) +", "+ ofToString(accel[accel.size()-1].y) +", "+ ofToString(accel[accel.size()-1].z)
                    + "\tMagne: " + ofToString(magne[accel.size()-1].x) +", "+ ofToString(magne[accel.size()-1].y) +", "+ ofToString(magne[accel.size()-1].z);

                } else{
                    cout << "WRONG NUMBER OF DATAPOINTS RECEIVED: " << dataPoints.size() <<endl;
                    recvdDataString = "ERROR: Wrong Number of Datapoints Received !";
                }
            }
        } else {
             cout << "WRONG NUMBER OF MSG STRINGS RECEIVED: "<< fullMsg.size() << endl;
            recvdDataString = "ERROR: Wrong number of Msg Strings Received !";
        }
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50);
    ofSetColor(255);
    ofDrawBitmapString("total readings received: "+ofToString(accel.size()), 25, 50);
    ofSetColor(50,255,50);
    ofDrawBitmapString(recvdDataString, 25, 100);
    
    ofSetColor(200, 200, 255);
    ofDrawBitmapString("last command sent: "+sentDataString, 25, 250);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    char * toSend = new char;
    
    switch (key) {
        case OF_KEY_LEFT:
            *toSend = 'L';
            break;
        case OF_KEY_RIGHT:
            *toSend = 'R';
            break;
        case ' ': //space bar
            *toSend = 'S';
            break;
        default:
            cout << "ERROR: only LEFT, RIGHT, and SPACE are commands"<<endl;
            break;
    }
    sentDataString = toSend;
    udpConnection.Send(toSend, 1);
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
