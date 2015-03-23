#pragma once

#include "ofMain.h"

class ParticleParams;

class Particle {
    
    friend class ParticleSystem;
    
public:
    
    //Constructor
    Particle() : mass(1.0), color( ofColor::white ), lifespan(10.0), timeStep(1.0), textureSet(false) { }
    
    //Get the parameters
    ofPoint getPosition() const { return position; }
    ofPoint getVelocity() const { return velocity; }
    ofPoint getAcceleration() const { return acceleration; }
    float getMass() const { return mass; }
    ofColor getColor() const { return color; }
    bool isAlive() const { return (lifespan > 0) ? true : false; }
    
    //Set up the parameters
    void setup(const ParticleParams& params);
    
    //Set the velocity to a given value
    void setVelocity(const ofPoint& velocity){ this->velocity = velocity; }
    
    //Set texture
    void setTexture(const ofTexture& texture);
    
    //Apply a force to the particle
    void applyForce(const ofPoint& force);
    
    //Update the parameters
    void update(float dt);
    
    //Display the particle
    void display();
    
    
private:
    
    ofPoint position;
    ofPoint velocity;
    ofPoint acceleration;
    //Get a linear dependence between mass (>0) and circle radius of the particle
    float mass;
    ofColor color;
    //The particle lives until lifespan > 0
    //Set lifespan at the beginning to be more than 4
    float lifespan;
    //maxLifespan represents the beginning (and maximum) value of lifespan
    float maxLifespan;
    //Timestep (> 0) indicates the speed with which the particle dies
    //TimeStep = 1 by default. Set timeStep = 0, to avoid the death of the particle
    float timeStep;
    //If the particle has no texture associated, then it'll be drawn as a ofCircle
    ofTexture texture;
    bool textureSet;
    
};




//Class to generate objects which contain the initial
//parameters of a Particle object
class ParticleParams {
    
    friend Particle;
    
public:
    
    ParticleParams() { }
    
    //Constructor
    ParticleParams(const ofPoint& position, float lifespan = 10, const ofColor& color = ofColor::white, const ofPoint& velocity = ofPoint(0, 0), const ofPoint& acceleration = ofPoint(0, 0), float mass = 1.0);
    
private:
    
    ofPoint position;
    ofPoint velocity;
    ofPoint acceleration;
    float mass;
    ofColor color;
    float lifespan;
    float timeStep;
    
};



