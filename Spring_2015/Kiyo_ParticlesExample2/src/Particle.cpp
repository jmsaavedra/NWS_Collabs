#include "Particle.h"


//Particle

void Particle::setup(const ParticleParams &params){
    
    position = params.position;
    velocity = params.velocity;
    acceleration = params.acceleration;
    mass = (params.mass > 0) ? params.mass : 1.0;
    color = params.color;
    lifespan = params.lifespan;
    maxLifespan = params.lifespan;
    timeStep = (params.timeStep >= 0) ? params.timeStep : 1.0;
    
}

void Particle::setTexture(const ofTexture &texture){
    this->texture = texture;
    textureSet = true;
}

void Particle::update(float dt){
    
    if ( isAlive() ) {   //Check if the particle is alive
        
        //If dt is too large, then make it smaller
        //to avoid problems with the Euler method
        float deltaT = ofClamp(dt, 0, 0.1);
        
        velocity += acceleration * deltaT;
        position += velocity * deltaT;
        
        //Set acceleration to 0, to avoid the accumulation
        //with the previous value
        acceleration *= 0;
        
        //Decrease lifespan
        lifespan -= timeStep * deltaT;
        
    }
    
}

void Particle::applyForce(const ofPoint& force){
    
    if(mass > 0){
        acceleration = force / mass;
    }
    
}

void Particle::display() {
    
    if( isAlive() )
    {
        //Set up the color
        ofSetColor(color);
        
        //Create a variable to store the age of the particle
        float age = maxLifespan-lifespan;
        
        //radius will store the radius of the particle
        float radius = 0;
        //k represent the time necessary to be adult and to die
        int k = 2;
        
        if (timeStep == 0) {    //If the particle will never die
            
            radius = 5 * mass;
            
        }else if (age < k) {    //If the particle is still a child
            
            radius = ofMap(age, 0, k, 0, 5 * mass);
            
        }else if( age > maxLifespan - k ){ //If the particle isn't an adult anymore
            
            radius = ofMap(age, maxLifespan - k, maxLifespan, 5 * mass, 0);
            
        }else{  //If the particle is an adult
            
            radius = 5 * mass;
            
        }
        
        //Draw the particle
        if (textureSet) {
            texture.draw(position.x, position.y, 2*radius, 2*radius);
        }else{
            ofSetCircleResolution(20);
            ofCircle(position, radius);
        }
    }
}



//ParticleParams

ParticleParams::ParticleParams(const ofPoint& position, float lifespan, const ofColor& color, const ofPoint& velocity, const ofPoint& acceleration, float mass){
    
    this->position = position;
    this->lifespan = lifespan;
    this->timeStep = 1;
    this->color = color;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->mass = mass;
    
}