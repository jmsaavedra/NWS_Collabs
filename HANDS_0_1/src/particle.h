#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class particle
{
public:
	ofVec2f pos;
	ofVec2f vel;
	ofVec2f frc;   // frc is also know as acceleration (newton says "f=ma")
	
	particle();
	particle(ofVec2f pos, ofVec2f vel);
	virtual ~particle(){};
	
	void resetForce();
	void addForce(float x, float y);
	void addDampingForce();
	void setInitialCondition(float px, float py, float vx, float vy);
	void update();
	void draw();
	void draw(ofImage *image);
	
	void bounceOffWalls();
	
	
	float damping;
    int     age, lifespan;
    bool    dead;
	
protected:
private:
};

#endif // PARTICLE_H
