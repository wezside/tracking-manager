#pragma once
#include "trackingmanager.hpp"
#include "imgproc.hpp"

namespace wezside
{
	class HeatManager : public wezside::TrackingManager
	{
	public:
		HeatManager()
		{
			bShow = true;
		}
		HeatManager(const HeatManager &that)
		{
			this->bEnable = that.bEnable;
			this->bSendData = that.bSendData;
			this->bShow = that.bShow;
		}
		HeatManager& operator= (const HeatManager &that)
		{
			this->bEnable = that.bEnable;
			this->bSendData = that.bSendData;
			this->bShow = that.bShow;
			return *this;
		}
		~HeatManager() {}

		void init(std::string _name = "default", int _id = 0);
		void update(int w, int h);
		void draw();

		void clear();
		void addMat(cv::Mat& in);

		ofPoint offset;
		ofTexture texture;		
		ofxIntSlider nColorMap;
		std::vector<cv::Mat> maps;
		cv::Mat map;
	};
}