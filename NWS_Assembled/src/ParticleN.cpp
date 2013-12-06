#include "ParticleN.h"

ParticleN::ParticleN(){
    radius = 5.0;
    friction = 0.01;
    bFixed = false;
}

void ParticleN::setup(ofVec2f _position, ofVec2f _velocity){
    position = _position;
    velocity = _velocity;
}
void ParticleN::setup(float positionX, float positionY, float velocityX, float velocityY){
    position = ofVec2f(positionX, positionY);
    velocity = ofVec2f(velocityX, velocityY);
}

void ParticleN::resetForce(){
    force.set(0, 0);
}

void ParticleN::addForce(ofVec2f _force){
    force += _force;
}
void ParticleN::addForce(float forceX, float forceY){
    force += ofVec2f(forceX, forceY);
}

void ParticleN::updateForce(){
    force -= velocity * friction;
}

void ParticleN::updatePos(){
    if (!bFixed) {
        velocity += force;
        position += velocity;
    }
}

void ParticleN::update(){
    updateForce();
    updatePos();
}


void ParticleN::bounceOffWalls(){
	bool bDampedOnCollision = false;
	bool bDidICollide = false;
	
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (position.x > maxx){
		position.x = maxx;
		velocity.x *= -1;
		bDidICollide = true;
	} else if (position.x < minx){
		position.x = minx;
		velocity.x *= -1;
		bDidICollide = true;
	}
	
	if (position.y > maxy){
		position.y = maxy;
		velocity.y *= -1;
		bDidICollide = true;
	} else if (position.y < miny){
		position.y = miny;
		velocity.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		velocity *= 0.3;
	}
	
}

void ParticleN::draw(){
    ofCircle(position, radius * 2);
}

void ParticleN::addRepulsionForce(float x, float y, float radius, float scale){
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	ofVec2f diff	= position - posOfForce;
	float length	= diff.length();
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);
        diff.normalize();
		force.x = force.x + diff.x * scale * pct;
        force.y = force.y + diff.y * scale * pct;
    }
}

void ParticleN::addRepulsionForce(ParticleN &p, float radius, float scale){
	ofVec2f posOfForce;
	posOfForce.set(p.position.x,p.position.y);
	ofVec2f diff	= position - posOfForce;
	float length	= diff.length();
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = force.x + diff.x * scale * pct;
        force.y = force.y + diff.y * scale * pct;
		p.force.x = p.force.x - diff.x * scale * pct;
        p.force.y = p.force.y - diff.y * scale * pct;
    }
}

void ParticleN::addAttractionForce(float x, float y, float radius, float scale){
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	ofVec2f diff	= position - posOfForce;
	float length	= diff.length();
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = force.x - diff.x * scale * pct;
        force.y = force.y - diff.y * scale * pct;
    }
}

void ParticleN::addAttractionForce(ParticleN &p, float radius, float scale){
	ofVec2f posOfForce;
	posOfForce.set(p.position.x, p.position.y);
	ofVec2f diff	= position - posOfForce;
	float length	= diff.length();
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = force.x - diff.x * scale * pct;
        force.y = force.y - diff.y * scale * pct;
		p.force.x = p.force.x + diff.x * scale * pct;
        p.force.y = p.force.y + diff.y * scale * pct;
    }
}

void ParticleN::addCounterClockwiseForce(ParticleN &p, float radius, float scale){
	ofVec2f posOfForce;
	posOfForce.set(p.position.x,p.position.y);

	ofVec2f diff = position - posOfForce;
	float length = diff.length();
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x = force.x + diff.y * scale * pct;
        force.y = force.y - diff.x * scale * pct;
		p.force.x = p.force.x - diff.y * scale * pct;
        p.force.y = p.force.y + diff.x * scale * pct;
		
    }
}

