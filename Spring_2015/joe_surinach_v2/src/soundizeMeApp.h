#pragma once

//#include "ofMain.h"
#include "ball.h"
//include "ofgui.h"
#include <vector>

//class soundizeMeApp : public ofBaseApp{
class santiSoundize {
    public:
        void setup();
        void update(float receivedFft[]);
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
    private:
        std::vector<Ball>   m_balls;
        ofSoundPlayer       m_audio;
        float *             m_fftSmoothed;
        int                 m_nBandsToGet;
        bool                m_isPlaying;
        
};
