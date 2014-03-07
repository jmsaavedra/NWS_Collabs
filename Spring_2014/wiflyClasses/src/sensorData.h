//
//  sensorData.h
//  wiflyTest
//
//  Created by Namreta Kumar on 3/5/14.
//
//

#pragma once
#include "ofMain.h"

class sensorData {
    
public:
    
    sensorData();
    
    vector<ofVec3f> accel;
    vector<ofVec3f> magne;
    float sensorID;
    
    void update(string rawData);
    
};
