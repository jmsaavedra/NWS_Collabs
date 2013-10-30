//
//  Particle.cpp
//  butterflyEffect
//
//  Created by xjjulia on 29/10/13.
//
//

#include "Particle.h"

Particle::Particle(){
    
}

void Particle::setup(){
    flySize=ofRandom(30,50);
    pos.x=ofRandomWidth();
    pos.y=ofRandomHeight();
    vel.x=ofRandom(0.0, 1.0);
    vel.y=ofRandom(0.0, 1.0);
    angleX=ofRandom(360);
    angleY=ofRandom(360);
    angleZ=ofRandom(360);
    stringList[0]="0.png";
    stringList[1]="1.png";
    stringList[2]="2.png";
    stringList[3]="3.png";
    stringList[4]="4.png";
    for (int i=0; i<5; i++) {
           butterflyList[i].loadImage(stringList[i]);
    
}
}

void Particle::update(){
    pos.x += vel.x*10.0;
    pos.y += vel.y*10.0;
    
    if( pos.x < 0.0 || pos.x > ofGetWidth() ){
        vel.x = vel.x * -1.0;
    }
    
    if( pos.y < 0.0 || pos.y > ofGetHeight() ){
        vel.y = vel.y * -1.0;
    }

//    angleX+=0.1;
//    angleY+=0.1;
    //angleZ+=0.01;
//    if (angleX>360) {
//        angleX=0;
//    }
//    if (angleY>360) {
//        angleY=0;
//    }
//    if (angleZ>360) {
//        angleZ=0;
//    }
    
}

void Particle::draw(){
    
    ofRotateX(angleX);
    ofRotateY(angleY);
    ofRotateZ(angleZ);
    for (int i=0; i<butterflyList.size(); i++) {
        butterflyList[int(ofRandom(5))].draw(pos, flySize, flySize);
    }
    
  
}

    


