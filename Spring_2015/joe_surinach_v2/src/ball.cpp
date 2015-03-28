#include "ball.h"
Ball::~Ball(){
}
Ball::Ball(ofVec2f _location,ofVec2f _velocity,ofColor _color,float _radius,std::vector<int> &_FFTids){
    FFTids      =   _FFTids;
    location    =   _location;
    velocity    =   _velocity;
    color       =   _color;
    radius      =   _radius;
    noiseR      =   ofRandom(5000);
    noiseG      =   ofRandom(5000);
    noiseB      =   ofRandom(5000);
}
void Ball::updateColor(float fftValue){
    if (fftValue != 0){
        color.lerp(ofColor::fromHex(ofMap(fftValue, 0, 0.35, 0, 255*255*255)),FFT_COLOR_LERP);
    }

    color.lerp(ofColor(
        ofMap(ofNoise(noiseR), 0, 1, 0, 1),
        ofMap(ofNoise(noiseG), 1, 0, 1, 0),
        ofMap(ofNoise(noiseB), 0, 1, 0, 1)
    ),NOISE_COLOR_LERP);

    if(color.getBrightness() < MIN_BRIGHTNESS)
        color.setBrightness(MIN_BRIGHTNESS);
    if(color.getSaturation() < MIN_SATURATION)
        color.setSaturation(MIN_SATURATION);


    noiseR += NOISE_STEP;
    noiseG += NOISE_STEP;
    noiseB += NOISE_STEP;
}

void Ball::checkEdges(){
    float diameter = radius*2;
    if (location.x > ofGetWidth() + diameter)
        location.x  =   -diameter;
    else if (location.x < -diameter)
        location.x  =  ofGetWidth() + diameter;

    if (location.y > ofGetHeight() + diameter)
        location.y  =  -diameter;
    else if (location.y < -diameter)
        location.y  =  ofGetHeight() + diameter;
}
void Ball::updateRadius(float fftValue){
    if(fftValue > FFT_MAP_RADIUS)
        fftValue = FFT_MAP_RADIUS;
    radius = ofMap(fftValue, 0, FFT_MAP_RADIUS, RADIUS_MIN, RADIUS_MAX);
}

void Ball::applyFFTForce(ofVec2f FFTForce){
    float angle = ofMap(FFTForce.x, 0, FFT_MAP_FORCE, 0,TWO_PI);
    float length = ofMap(FFTForce.y, 0, FFT_MAP_FORCE, 0, FFT_FORCE_MAX);
    FFTForce.set(sin(angle),cos(angle));
    FFTForce = FFTForce * length;
    applyForce(FFTForce);
}

void Ball::separate(std::vector<Ball> & balls){
    float desiredseparation = radius;
    ofVec2f sum;
    int count = 0;
    for (int i = 0; i < balls.size(); ++i){

        ofVec2f delta = location - balls.at(i).location;
        float distance  = delta.length();
        if (distance != 0 &&  distance < desiredseparation){
            sum  += delta / (distance * distance);
            count++;
        }
    }

    if (count > 0){
      sum.normalize();
      sum *= MAX_VELOCITY;
      ofVec2f steer = sum - velocity;
      steer.limit(MAX_FORCE);
      applyForce(steer);
    }

}
void Ball::applyForce(ofVec2f force){
    // force = force / mass //radius
    acceleration = acceleration + force;
}

void Ball::FFTupdate(float* FFTSmoothed){
    float   r = sqrt(FFTSmoothed[FFTids.at(0)]),
            x = sqrt(FFTSmoothed[FFTids.at(1)]),
            y = sqrt(FFTSmoothed[FFTids.at(2)]),
            c = sqrt(FFTSmoothed[FFTids.at(3)]);
    updateRadius(r);
    applyFFTForce(ofVec2f(x,y));
    updateColor(c);
}
void Ball::update(){
    velocity = velocity + acceleration;
    velocity.limit(MAX_VELOCITY);
    location = location + velocity;
    acceleration = acceleration * 0;
}

 void Ball::circle(ofVec2f _location, float _radius, ofColor _color){
    //http://slabode.exofire.net/circle_draw.shtml
    int num_segments = 10 * sqrtf(_radius);
    float theta = 2 * M_PI / float(num_segments);
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = _radius;//we start at angle = 0
    float y = 0;

    glBegin(GL_TRIANGLE_FAN);
    ofSetColor(_color);
    glVertex2f(_location.x, _location.y);
    ofSetColor(ofColor(_color.r,_color.g,_color.b,0));
    for(int ii = 0; ii < num_segments; ii++) {
        glVertex2f(x + _location.x, y + _location.y);//output vertex

        
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glVertex2f(radius + _location.x,  _location.y);
    glEnd();
}


void Ball::draw(){
    ofSetColor(color);
    ofFill();
    // ofElipse(location.x, location.y, radius * 0.8);
    ofCircle(location.x, location.y, radius);
    // circle(location,radius,color);

}
