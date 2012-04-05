#ifndef _TRACKER_APP
#define _TRACKER_APP

#include "ofMain.h"
#include "ofxSubtitle.h"

class SubtitleApp : public ofBaseApp{

public:
    SubtitleApp(){};
    SubtitleApp(int ac, char* av[]);

    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    ofxSubtitle     mSubtitle;

};

#endif
