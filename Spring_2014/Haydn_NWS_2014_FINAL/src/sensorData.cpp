//
//  sensorData.cpp
//  wiflyTest
//
//  Created by Namreta Kumar on 3/5/14.
//
//

#include "sensorData.h"

sensorData::sensorData() {
    
}

//-------------------------------------
void sensorData::update(string rawData){
    
    float aX,aY,aZ,mX,mY,mZ;
    
    indSensors.clear();
    
    if(rawData!=""){
        
		vector<string> fullMsg = ofSplitString(rawData,"/");;
        
        if (fullMsg.size() >= 2) { // received a full, correct message
            
            for(int k=1; k < fullMsg.size(); k++){
                vector<string> dataPoints = ofSplitString(fullMsg[k],"\t");
                if (dataPoints.size() == 7) {
                    
                    individualSensors tempSensor;
                    
                    tempSensor.individualSensorID = atof(dataPoints[0].c_str());
                    tempSensor.individualAccel = (ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                    tempSensor.individualMagne = (ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));
                    
                    indSensors.push_back(tempSensor);
                    
                } else cout << "WRONG NUMBER OF DATAPOINTS RECEIVED: " << dataPoints.size() <<endl;
            }
        } else cout << "WRONG NUMBER OF MSG STRINGS RECEIVED: "<< fullMsg.size() << endl;
	}
    
    
    
}
