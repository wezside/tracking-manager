#pragma once
#include "trackingmanager.hpp"

namespace wezside
{
	class BlobManager : public wezside::TrackingManager
	{
	public:
		BlobManager() 
		{
			bShow = true;
		}
		BlobManager(const BlobManager &that)
		{
			this->bEnable = that.bEnable;
			this->bSendData = that.bSendData;
			this->bShow = that.bShow;
			this->id = that.id;
		}
		BlobManager& operator= (const BlobManager &that)
		{
			this->bEnable = that.bEnable;
			this->bSendData = that.bSendData;
			this->bShow = that.bShow;
			this->id = that.id;
			return *this;
		}
		~BlobManager() {}

		void init(std::string _name = "default", int _id = 0);
		#ifdef _WIN32
			virtual void update(const std::shared_ptr<ofxKinectForWindows2::Source::Depth>& depth, const std::shared_ptr<ofxKinectForWindows2::Source::Body>& body);
			virtual void update(const ofPixels& frame);
		#else
			virtual void update(const ofPixels& frame);
		#endif
		void draw();
		void exit();
		void send(ofxOscMessage data);
		cv::Mat output_and;

	private:
		ofTexture texture;
		ofTexture texture_depth;
		ofxCvContourFinder contourFinder;
		ofxToggle bDrawCentroid;
		ofxIntSlider nSendFrequency;
	};
}