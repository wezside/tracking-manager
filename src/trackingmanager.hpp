#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#ifdef _WIN32
#include "ofxKinectForWindows2.h"
#endif
#include "ofxOpenCv.h"
#include "ofxCvContourFinder.h"
#include "imgproc.hpp"

#define HOST "169.254.116.191"
#define PORT 12345
#define DEPTH_WIDTH 512
#define DEPTH_HEIGHT 424

namespace wezside
{
	class TrackingManager
	{
	public:
		TrackingManager() {}
		TrackingManager(const TrackingManager &that) {}
		TrackingManager& operator= (const TrackingManager &rhs) {return *this;}

		virtual ~TrackingManager() { ofLogNotice() << "[TrackingManager] Destruct"; }

		virtual void init(std::string _name = "default", int _id = 0);
		virtual void toggle();

		#ifdef _WIN32
			virtual void update(const std::shared_ptr<ofxKinectForWindows2::Source::Depth>& depth, const std::shared_ptr<ofxKinectForWindows2::Source::Body>& body);
			virtual void update(const ofPixels& frame);
		#else
			virtual void update(const ofPixels& frame);
		#endif

		virtual void draw();
		virtual void send(ofxOscMessage data);
		virtual void setPosition(float x, float y);
		virtual void exit();

	protected:

		int nGX;
		int nGY;
		int nGWidth;
		int nGHeight;
		int id;

		bool bShow;
		std::string name;

		ofxPanel gui;
		ofParameterGroup parameters;
		ofxFloatSlider fNearClip;
		ofxFloatSlider fFarClip;
		ofxFloatSlider fBlobSize;
		ofxIntSlider nScaleFactor;
		ofxIntSlider nBlur;
		ofxIntSlider nErosion;
		ofxIntSlider nDilution;
		ofxToggle bEnable;
		ofxToggle bSendData;
		ofxToggle bDraw;

		ofxOscSender sender;
	};
}