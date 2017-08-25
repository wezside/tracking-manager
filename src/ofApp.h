#pragma once

#include "ofMain.h"
#ifdef _WIN32
#include "ofxKinectForWindows2.h"
#endif
#include "blobmanager.hpp"
#include "heatmanager.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		#ifdef _WIN32
		ofxKinectForWindows2::Device kinect;
		#endif

		ofVideoGrabber cam;	
		wezside::BlobManager mBlobTracker;
		wezside::HeatManager mHeatManager;
};
