#include "testApp.h"

int W = 200;		//Grid size
int H = 200;

//--------------------------------------------------------------
void testApp::setup(){
    //Set up vertices and colors
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            mesh.addVertex(
                           ofPoint( (x - W/2) * 12*4, (y - H/2) * 12*4, 0 ) );
            mesh.addColor( ofColor( 0, 255, 0 ) );
        }
    }
    //Set up triangles' indices
    for (int y=0; y<H-1; y++) {
        for (int x=0; x<W-1; x++) {
            int i1 = x + W * y;
            int i2 = x+1 + W * y;
            int i3 = x + W * (y+1);
            int i4 = x+1 + W * (y+1);
            mesh.addTriangle( i1, i2, i3 );
            mesh.addTriangle( i3, i1, i2 );
        }
    }
    setNormals( mesh );		//Set normals
//    light.enable();			//Enable lighting
    
    
    
    // listen on the given port
    //**********************************************
    //**********************************************
//    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    ofBackground(30, 30, 130);
    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }
    //**********************************************
    //**********************************************
    


}

//--------------------------------------------------------------
void testApp::update(){
    
    keyValue = receivedFft[50]*15;
    keyValue = ofMap(keyValue, 0.0, 1.0, 0.0, 8.0);
    keyValue *= 0.95;
//    cout << "keyValue: "<< keyValue<< endl;
    
    float time = ofGetElapsedTimef();	//Get time
    //Change vertices
    for (int y=0; y<H; y++) {
        for (int x=0; x<W; x++) {
            int i = x + W * y;			//Vertex index
            ofPoint p = mesh.getVertex( i );
            
            //Get Perlin noise value
            float value =
            ofNoise( x * 0.1, y * 0.1, time * 1 );

            //Change z-coordinate of vertex
            p.z = value *100*keyValue;
            //            p.x = x+100 * smoothedVol;
            mesh.setVertex( i, p );
            
            //Change color of vertex
            mesh.setColor( i,
                          ofColor( 255, 0, 255, 100 ) );
        }
    }
    setNormals( mesh );	//Update the normals
    
    
    
    //audio**********************************************
    //**********************************************
    //// hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/channel_01/FFT"){
            int index = m.getArgAsInt32(0);
            float val = m.getArgAsFloat(1);
            receivedFft[index] = val;
            //cout << "recvd: " << index << " :\t"<<val<<endl;
        }
        
        else {
            string msg_string;
            msg_string = m.getAddress();
            cout << "unrecognized message" << msg_string << endl;
        }
    }
    //**********************************************
    //**********************************************
    


}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableDepthTest();				//Enable z-buffering
    
    //Set a gradient background from white to gray
    ofBackgroundGradient( ofColor( 20 ), ofColor( 10 ) );
    

    easyCam.begin();

    ofPushMatrix();						//Store the coordinate system
    

    ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );

    
    //Calculate the rotation angle
    float time = ofGetElapsedTimef();   //Get time in seconds
    float angle = time * 20;			//Compute angle. We rotate at speed

    ofRotate( angle, 1, 0, 1 );
    

    
    mesh.drawWireframe();
    ofPopMatrix();      //Restore the coordinate system
    
    
    easyCam.end();
    
    

    ofSetColor(255,255,255,255);
    
    
    //I use the 50th frequency
    //**********************************************
    //**********************************************
//    float width = 3;
//    for (int i = 50;i < 51; i++){
//        //cout << i << "\t"<<fftSmoothed[i] << endl;
//        ofRect(25+i*width,250,width,-(receivedFft[i] * 750));
//        if(i%10 == 0){
//            ofDrawBitmapString(ofToString(i), 25+i*width, 265);
//        }
//    }
    //**********************************************
    //**********************************************
    

}












//--------------------------------------------------------------
//Universal function which sets normals for the triangle mesh
void setNormals( ofMesh &mesh ){
    
    //The number of the vertices
    int nV = mesh.getNumVertices();
    
    //The number of the triangles
    int nT = mesh.getNumIndices() / 3;
    
    vector<ofPoint> norm( nV ); //Array for the normals
    
    //Scan all the triangles. For each triangle add its
    //normal to norm's vectors of triangle's vertices
    for (int t=0; t<nT; t++) {
        
        //Get indices of the triangle t
        int i1 = mesh.getIndex( 3 * t );
        int i2 = mesh.getIndex( 3 * t + 1 );
        int i3 = mesh.getIndex( 3 * t + 2 );
        
        //Get vertices of the triangle
        const ofPoint &v1 = mesh.getVertex( i1 );
        const ofPoint &v2 = mesh.getVertex( i2 );
        const ofPoint &v3 = mesh.getVertex( i3 );
        
        //Compute the triangle's normal
        ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
        
        //Accumulate it to norm array for i1, i2, i3
        norm[ i1 ] += dir;
        norm[ i2 ] += dir;
        norm[ i3 ] += dir;
    }
    
    //Normalize the normal's length
    for (int i=0; i<nV; i++) {
        norm[i].normalize();
    }
    
    //Set the normals to mesh
    mesh.clearNormals();
    mesh.addNormals( norm );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
//    cout << easyCam.getTarget() <<endl;
    

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