#include "ofApp.h"

void ofApp::setup() 
{
    ofSetVerticalSync(false);

    cam.setVerbose(true);
    cam.setDeviceID(1);
    cam.initGrabber(320,240);
    int grabW = cam.getWidth();
    int grabH = cam.getHeight();
    printf("asked for 320 by 240 - actual size is %i by %i ", grabW, grabH);

    // Intialise tracking pipelines
    mBlobTracker.init("feet", 0);
    mHeatManager.init("heat", 0);
    mHeatManager.offset = ofPoint(0, 200);
}

void ofApp::update()
{
    cam.update();   
    if(cam.isFrameNew()) 
    {
        auto frame = cam.getPixels();
        mBlobTracker.update(frame);

        mHeatManager.map = mBlobTracker.output_and;
        mHeatManager.update(cam.getWidth(), cam.getHeight());
    }
}

void ofApp::draw()
{
    mBlobTracker.draw();  
    mHeatManager.draw();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2) + "fps");
}

void ofApp::exit()
{
    mBlobTracker.exit();
    mHeatManager.exit();
}

void ofApp::keyPressed(int key)
{
    switch (key)
    {
        case '2': mBlobTracker.toggle(); break;
        case '3': mHeatManager.toggle(); break;
    }
}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}