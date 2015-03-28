//
//  surinachOsc.cpp
//  surinach_v2
//
//  Created by Joseph Saavedra on 3/28/15.
//
//

#include "surinachOsc.h"

//-----------------------------------------------------------
void surinachOsc::setup(){
    
    cout << "hit surinach OSC setup" << endl;
    cout << "listening for osc messages on port " << PORT << "\n";
    receiver.setup(PORT);
    
    
    nBandsToUse = 1024;
    receivedFft = new float[nBandsToUse];
    for (int i = 0; i < nBandsToUse; i++){
        receivedFft[i] = 0;
    }
}

//-----------------------------------------------------------
void surinachOsc::update(){
    
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
}