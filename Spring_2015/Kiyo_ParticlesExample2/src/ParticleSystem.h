#pragma once

#include "ofMain.h"
#include "Particle.h"

class ParticleSystem {
    
public:
    
    //Default constructor
    ParticleSystem(){ }
    
    //Set up the ParticleSystem members and inizialiting the params
    //of the particles by default, except from the initial velocity (with a norm of 10)
    void setup(const ofPoint& position, int n);
    
    //Set up the ParticleSystem from a given particle
    void setup(Particle p, int n);
    
    //Get member parameters
    list<Particle> getParticleList() const{ return system; }
    ofPoint getPosition() const{ return sampleParticle.position; }
    int getNumberParticlesForEveryEmission() const { return n; }
    
    //Apply a given force
    void applyForce(const ofPoint& force);
    
    //Emit particles when called
    void emission();
    
    //Update all the particles
    void update(float dt);
    
    //Display the particle system
    void display();
    
    //Selected the Emitter and move it
    void moveEmitter(float x, float y);
    
    
private:
    
    //List which stores all the particles generated
    list<Particle> system;
    
    //Particle that describe the behaviour of a generic particle in the system
    //The position of the emitter is the position of the sampleParticle
    Particle sampleParticle;
    
    //Particles generated in a single emission
    int n;
    
    //Generate n particles based on a given particle p, with a given velocity
    void generateParticles(int n, Particle p, ofPoint velocity);
    
};