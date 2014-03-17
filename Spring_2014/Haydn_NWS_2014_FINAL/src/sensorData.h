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
    
    struct individualSensors {
        
        int individualSensorID;
        ofVec3f individualAccel;
        ofVec3f individualMagne;
        
    };
    
    vector <individualSensors> indSensors;
    
    void update(string rawData);
    
};
