#include "ofApp.h"

//Parameters

float lifespan = 10;        //Particles lifetime in seconds
ofPoint initVel(10, 0);     //Initial velocity of particles
ofPoint initAcc(0, 0);      //Initial acceleration of particles
float mass = 2;             //Mass of the particles
ofPoint initPos;            //Initial position of particle systems




//--------------------------------------------------------------
void ofApp::setup(){

    currMovieIdx = 0;
    
    ofSetFrameRate(60);
    
    //Load Image for particle
    ofLoadImage(image, "particle.png");
    
    myPlayer.loadMovie(movies[currMovieIdx]);
    myPlayer.play();
    
    //Color Palette
    colors.push_back( ofColor(0, 242, 255) );
    colors.push_back( ofColor(97, 255, 163) );
    colors.push_back( ofColor(0, 71, 127, 128) );
    colors.push_back( ofColor(255, 100) );
    
    //Set up initial position
    initPos = ofPoint(ofGetWidth() / 2, ofGetHeight() / 6);
    
    for (int i = 0; i < N; i++) {
        
        float magnitude = 10;
        float angle = 360 / float(N);
        initPos += ofPoint(magnitude, 0).rotate(i*angle, ofPoint(0, 0, 1));
        
        //set up params
        ParticleParams params ( initPos, lifespan, colors[ i % colors.size() ], initVel, initAcc, mass);
        Particle p;
        p.setup(params);
        p.setTexture(image);
        
        //Set up particle systems
        ParticleSystem system;
        system.setup(p, 10);
        
        systems.push_back(system);
        
    }
    
    //Sound
    track.loadSound("3_parade_of_beauties.mp3");
    track.setVolume(0.5);
    track.play();
    
    //initialize spectrum and previous spectrum
    for (int i = 0; i != N; i++) {
        spectrum.push_back(0.0f);
        prevSpectrum.push_back(0.0f);
    }
    
    //Initialize the threshold
    threshold = 1;
    
    time0 = ofGetElapsedTimef();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(myPlayer.isLoaded())myPlayer.update(); // get all the new frames
    
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    
    ofSoundUpdate();
    
    int K = 8;                              //Number of bands
    float *val = ofSoundGetSpectrum(K);     //Generate the spectrum
    
    //Dynamically change the threshold according to the current framerate
    if (ofGetFrameRate() < 40) {
        threshold = ofClamp(threshold + 0.01, 0.95, 0.98);
    }else{
        threshold = ofClamp(threshold - 0.001, 0.95, 0.98);
    }
    
    
    for (int i = 0; i != N; i++) {
        
        //Avoid a sudden change of spectrum
        spectrum[i] *= threshold;
        spectrum[i] = max(spectrum[i], val[i]);
        
        //Apply the gravity
        systems[i].applyForce(gravity);
        systems[i].update(dt);
        
        //Change the position of the particle systems
        //using Perlin Noise
        initPos.x = systems[i].getPosition().x;
        initPos.y = systems[i].getPosition().y;
        initPos.x += ofMap(ofNoise(0.5 * time + 100 + i * 10), 0, 1, -2, 2);
        initPos.y += ofMap(ofNoise(0.5 * time + 200 + i * 20), 0, 1, -2, 2);
        
        initPos.x = ofClamp(initPos.x, 1/5.0 * ofGetWidth(), 4/5.0 * ofGetWidth());
        initPos.y = ofClamp(initPos.y, 1/10.0 * ofGetHeight(), 1/3.0 * ofGetHeight());
        
        //Move the particle systems
        systems[i].moveEmitter(initPos.x, initPos.y);
        
    }
    
    
    for (int i = 0; i < N; i++) {
        
        if (spectrum[i] > 0.001 && spectrum[i] * (i/2.0 + 1) > 1.5 * prevSpectrum[i] && prevSpectrum[i] != 0.0f) {
            
            //Number of particles to emit
            int n = ofClamp(int(50 * spectrum[i] * (20*i+1)/2.0), 1, 10);
            
            //Initial velocity
            //Rotate vector of an angle between 0 and 180¡
            initVel = ofPoint(50, 0).rotate(ofNoise(0.2 * time)*180, ofPoint(0, 0, 1));
            //Varying velocity according to the number of particles
            initVel *= 0.05 * n;
            
            //Mass
            mass = ofClamp( 10 * (1+10*i) * spectrum[i], 2, 10);
            
            //
            Particle p;
            
            ParticleParams params (systems[i].getPosition(), 10, colors[ i % colors.size() ], initVel, initAcc, mass);
            p.setup(params);
            p.setTexture(image);
            
            
            systems[i].setup(p, n);
            
            systems[i].applyForce(gravity);
            
            systems[i].emission();
            
        }
        
    }
    
    for (int i = 0; i != N; i++) {
        prevSpectrum[i] = spectrum[i];
    }
    
    
    time0 = ofGetElapsedTimef();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    //Set a black background
    //ofBackground(0);
    cout << spectrum[0] << endl;
    float col = ofMap(spectrum[0], 0.0, 0.001, 0, 255);
    ofSetColor(col, 255-col, col);
    if(myPlayer.isLoaded())myPlayer.draw(0,0, ofGetWidth(), ofGetHeight());
    
    //Draw the particle systems
    ofSetColor(255, 255, 255);
    for (int i = 0; i < N; i++) {
        systems[i].display();
        //ofSetColor(colors[i]);
        //ofCircle(systems[i].getPosition(), 5);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
            
        case OF_KEY_RIGHT:
            nextMovie();
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::nextMovie(){
    myPlayer.closeMovie();
    currMovieIdx++;
    if(currMovieIdx > 18) currMovieIdx = 0;
    myPlayer.loadMovie(movies[currMovieIdx]);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
