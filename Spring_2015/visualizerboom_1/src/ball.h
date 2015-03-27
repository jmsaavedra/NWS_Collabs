#ifndef ball_h

#define ball_h



#include "ofMain.h"

#include <vector>





#define _USE_MATH_DEFINES

#include <math.h>



#define FFT_MAP_RADIUS      0.33

#define RADIUS_MIN          0.5

#define RADIUS_MAX          300



#define MAX_VELOCITY        10

#define MAX_FORCE           9

#define FFT_MAP_FORCE       0.36

#define FFT_FORCE_MAX       5

#define FFT_COLOR_LERP      0.05

#define NOISE_COLOR_LERP    0.355

#define MIN_SATURATION      400

#define MIN_BRIGHTNESS      300

#define NOISE_STEP          0.02



class Ball {
    
public:
    
    Ball(ofVec2f _location,ofVec2f _velocity,ofColor _color,float _radius,std::vector<int> &_FFTids);
    
    ~Ball();
    
    void update();
    
    void FFTupdate(float* FFTSmoothed);
    
    void draw();
    
    void separate(std::vector<Ball> & balls);
    
    void checkEdges();
    
    void updateColor(float fftValue);
    
    void updateRadius(float fftValue);
    
    void applyFFTForce(ofVec2f FFTForce);
    
    
    
    std::vector<int>    FFTids;
    
    ofVec2f             location, velocity, acceleration;
    
    ofColor             color;
    
    float               radius;
    
    float               noiseR,noiseG,noiseB;
    
private:
    
    void circle(ofVec2f _location, float _radius, ofColor _color);
    
    void applyForce(ofVec2f force);
    
};

#endif

