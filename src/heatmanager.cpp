#include "heatmanager.hpp"

void wezside::HeatManager::init(std::string _name, int _id)
{
	name = _name;
	id = _id;
	gui.setDefaultFillColor(ofColor(255.0f / 255.0f, 0.0f, 255.0f / 59.0f));
	gui.setDefaultHeaderBackgroundColor(ofColor(255.0f / 255.0f, 0.0f, 255.0f / 59.0f));
	gui.setup(name);
	gui.add(nColorMap.setup("Color Map", 4, 0, 11));
	gui.add(bEnable.setup("Enable/Disable", true));
	gui.setPosition(0, 120);
	gui.setHeaderBackgroundColor(ofColor(255.0f, 116.0f, 0.0f));
	wezside::TrackingManager::init(_name);
}

void wezside::HeatManager::update(int w, int h)
{
	if (bShow && bEnable)
	{
		//Finding Distance Transform
		cv::Mat mDist, mBlobDist, mBlobHeatMap;
		cv::distanceTransform(map, mDist, CV_DIST_L2, 5);
		cv::normalize(mDist, mDist, 0, 1., cv::NORM_MINMAX);
		mDist.convertTo(mDist, CV_8UC1, 255, 0);

		double min;
		double max;
		cv::minMaxIdx(mDist, &min, &max);
		cv::Mat adjMap;

		// expand your range to 0..255. Similar to histEq();
		mDist.convertTo(adjMap, CV_8UC1, 255 / (max - min), -min);

		// Convert the grayscale image to a tone-mapped one, 
		// function is found in contrib module, so include contrib.hpp 
		// and link accordingly
		cv::Mat falseColorsMap;
		cv::applyColorMap(adjMap, falseColorsMap, nColorMap);

		// Save the threshold result in a texture
		texture.loadData(falseColorsMap.data, w, h, GL_RGB);
	} 
}

void wezside::HeatManager::draw()
{
	if (bShow && bEnable)
	{
		ofSetColor(255.0f);
		// Draw pipelined processed image
		if (texture.isAllocated())
		{
			texture.draw(0, offset.y, DEPTH_WIDTH, DEPTH_HEIGHT);
		}
	}
	wezside::TrackingManager::draw();
}

void wezside::HeatManager::clear()
{
	vector<cv::Mat>::iterator it = maps.begin();
	for (; it != maps.end(); ++it)
	{
		(*it).release();
	}
	maps.clear();
}

void wezside::HeatManager::addMat(cv::Mat& in)
{
	maps.push_back(in);
}