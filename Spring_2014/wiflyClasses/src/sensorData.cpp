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
    
    if(rawData!=""){
        //      cout << "raw msg: ";
        //		cout << message << endl;
        
		vector<string> fullMsg = ofSplitString(rawData,"/");
        //        cout << "fullMsg size "<<fullMsg.size() << endl;
        //        cout << "fullMsg 0 "<<fullMsg[0] << endl;
        //        cout << "fullMsg 1 "<<fullMsg[1] << endl;
        
        if (fullMsg.size() >= 2) { // received a full, correct message
            
            for(int k=1; k < fullMsg.size(); k++){
                vector<string> dataPoints = ofSplitString(fullMsg[k],"\t");
                //cout << "datapoints size: "<<dataPoints.size() << endl;
                if (dataPoints.size() == 7) {
                    sensorID = atof(dataPoints[0].c_str());
                    accel.push_back(ofVec3f(atof(dataPoints[1].c_str()),atof(dataPoints[2].c_str()),atof(dataPoints[3].c_str())));
                    magne.push_back(ofVec3f(atof(dataPoints[4].c_str()),atof(dataPoints[5].c_str()),atof(dataPoints[6].c_str())));
                    
                    
                    int readingNum = accel.size();
                    cout << "total readings: "<< readingNum;
                    cout << "\tid: "<< sensorID; //WHICH UNIT IS THIS ONE
                    cout << "\tAccel: " << accel[accel.size()-1].x << ", " << accel[accel.size()-1].y << ", " << accel[accel.size()-1].z;
                    cout << "\t\tMagne: " << magne[accel.size()-1].x << ", " << magne[accel.size()-1].y << ", " << magne[accel.size()-1].z << endl;
                    
                } else cout << "WRONG NUMBER OF DATAPOINTS RECEIVED: " << dataPoints.size() <<endl;
            }
        } else cout << "WRONG NUMBER OF MSG STRINGS RECEIVED: "<< fullMsg.size() << endl;
	}
    
}
