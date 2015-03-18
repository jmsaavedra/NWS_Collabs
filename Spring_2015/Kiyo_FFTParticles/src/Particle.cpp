#include "Particle.h"

Particle::Particle(){
    radius = 3.0;
    friction = 0.01;
    bFixed = false;
    figure = 1;
    c.setHsb(1,1,1);
}

void Particle::setup(ofVec2f _position, ofVec2f _velocity){
    position = _position;
    velocity = _velocity;
}
void Particle::setup(float positionX, float positionY, float velocityX, float velocityY){
    position = ofVec2f(positionX, positionY);
    velocity = ofVec2f(velocityX, velocityY);
}
void Particle::resetForce(){
    force.set(0, 0);
}
void Particle::addForce(ofVec2f _force){
    force += _force;
}
void Particle::addForce(float forceX, float forceY){
    force += ofVec2f(forceX, forceY);
}
void Particle::updateForce(){
    force -= velocity * friction;
}
void Particle::updatePos(){
    if (!bFixed) {
        velocity += force;
        position += velocity;
    }
}
void Particle::update(){
    updateForce();
    updatePos();
}
void Particle::bounceOffWalls(){
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
void Particle::draw(){
    switch (figure) {
        case 1:
            ofSetCircleResolution(50);
            ofSetColor(c);
            ofCircle(position, radius);
            break;
        case 2:
            ofSetColor(c);
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofRect(position,radius * 1.2,radius * 1.2);
            //ofCircle(position, radius);
            break;
    }
}
void Particle::addRepulsionForce(float x, float y, float radius, float scale){
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

void Particle::addRepulsionForce(Particle &p, float radius, float scale){
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
void Particle::addAttractionForce(float x, float y, float radius, float scale){
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

void Particle::addAttractionForce(Particle &p, float radius, float scale){
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

void Particle::addCounterClockwiseForce(Particle &p, float radius, float scale){
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
void Particle::setColor(float _hue,float _sat,float _bri){
    c.setHsb(_hue, _sat, _bri);
}
ofColor Particle::getColor(){
    return c;
}
void Particle::setRadius(float _radius){
    radius = _radius;
}
void Particle::setFigure(int _figure){
    figure = _figure;
}

