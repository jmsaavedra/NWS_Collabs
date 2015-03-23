#include "ParticleSystem.h"

//Private

void ParticleSystem::generateParticles(int n, Particle p, ofPoint velocity){
    
    float angle = 360 / float(n);
    
    for(int i = 0; i != n; i++){
        
        //Rotate the velocity of angle in respect with the z-axis
        velocity.rotate(angle, ofPoint(0, 0, 1));
        p.setVelocity(velocity);
        system.push_back(p);
        
    }
    
}



//Public

void ParticleSystem::setup(const ofPoint& position, int n){
    
    ParticleParams params(position, n, ofColor::white, ofPoint(0, 0), ofPoint(0, 0), 1.0);
    sampleParticle.setup(params);
    
    this->n = n;
    
}


void ParticleSystem::setup(Particle p, int n){
    
    sampleParticle = p;
    this->n = n;
    
}


void ParticleSystem::applyForce(const ofPoint& force){
    
    for (list<Particle>::iterator it = system.begin(); it != system.end(); it++) {
        it->applyForce(force);
    }
    
}

void ParticleSystem::emission(){
    
    generateParticles(n, sampleParticle, sampleParticle.getVelocity());
    
}

void ParticleSystem::update(float dt){
    
    //If dt is too large, then make it smaller
    //to avoid problems with the Euler method
    float deltaT = ofClamp(dt, 0, 0.1);
    
    list<Particle>::iterator it = system.begin();
    while (it != system.end()) {
        
        if (it->isAlive()) {
            
            it->update(deltaT);
            
            it++;
        }else{
            it = system.erase(it);
        }
        
    }
    
}


void ParticleSystem::display(){
    
    for (list<Particle>::iterator it = system.begin(); it != system.end(); it++) {
        it->display();
    }
    
}


void ParticleSystem::moveEmitter(float x, float y){
    
    sampleParticle.position = ofPoint(x, y);
    
}