#include "blobmanager.hpp"

void wezside::BlobManager::init(std::string _name, int _id)
{
	name = _name;
	id = _id;
	gui.setup(name);
	gui.add(nScaleFactor.setup("Depthscale factor", 10, 1, 255));
	gui.add(nBlur.setup("Depth blur", 3, 1, 19));
	gui.add(fNearClip.setup("Near Clip", 70.0f, 0.0f, 255.0f));
	gui.add(fFarClip.setup("Far Clip", 120.0f, 0.0f, 255.0f));
	gui.add(nErosion.setup("Shrink", 0, 1, 21));
	gui.add(nDilution.setup("Expand", 0, 1, 150));
	gui.add(fBlobSize.setup("Min Blob Size", 0.1f, 0.0001f, 15.0f));
	gui.add(bSendData.setup("Send OSC", false));
	gui.add(bEnable.setup("Enable/Disable", true));
	gui.add(nSendFrequency.setup("Send Frequenct", 1, 1, 15));
	gui.setPosition(210, 0);
	gui.setHeaderBackgroundColor(ofColor(255.0f, 0.0f, 59.0f));
	wezside::TrackingManager::init(_name);
}

void wezside::BlobManager::update(const ofPixels& frame)
{
	if (bShow && bEnable)
	{
		int w = frame.getWidth();
		int h = frame.getHeight();
		cv::Mat depthImg(h, w, CV_8UC1, (void*)frame.getData());
		cv::Mat grayThreshNear(w, h, CV_8UC1);
		cv::Mat grayThreshFar(w, h, CV_8UC1);
		cv::Mat output(w, h, CV_8UC1);
		cv::Mat output_blur(w, h, CV_8UC1);
		
		double nearVal = fNearClip;
		double farVal = fFarClip;
		
		// Scale down the 16-bit image to an 8-bit
		// depthImg.convertTo(output, CV_8UC1, nScaleFactor / 255.0);
		
		// Blur the depth map
		if (nBlur % 2 == 0) nBlur = nBlur + 1;
		cv::GaussianBlur(output, output_blur, cv::Size(nBlur, nBlur), 10.0);

		// Perform threshold test
		cv::threshold(output_blur, grayThreshFar, farVal, 255.0, cv::THRESH_BINARY_INV);
		cv::threshold(output_blur, grayThreshNear, nearVal, 255.0, cv::THRESH_BINARY);
		cv::bitwise_and(grayThreshNear, grayThreshFar, output_and);
				
		// Erode the threshold result
		if (nErosion > 1)
		{
			cv::Mat elementErode = cv::getStructuringElement(
				cv::MORPH_RECT,
				cv::Size(2 * nErosion + 1, 2 * nErosion + 1),
				cv::Point(nErosion, nErosion));
			cv::erode(output_and, output_and, elementErode);
			elementErode.release();
		}

		if (nDilution > 1)
		{
			// Dilute the threshold result
			cv::Mat elementDilute = cv::getStructuringElement(
				cv::MORPH_RECT,
				cv::Size(2 * nErosion + 1, 2 * nErosion + 1),
				cv::Point(nErosion, nErosion));
			cv::dilate(output_and, output_and, elementDilute);
			elementDilute.release();
		}
		
		// Save the threshold result in a texture
		texture.loadData(output_and.data, w, h, GL_LUMINANCE);
		texture_depth.loadData(output_blur.data, w, h, GL_LUMINANCE);
		
		// Find contours which are between the size of 
		// 20 pixels and 1/3 the w*h pixels and ignore holes
		ofxCvGrayscaleImage depth_output;
		depth_output.allocate(DEPTH_WIDTH, DEPTH_HEIGHT);
		depth_output.setFromPixels(output_and.data, w, h);
		contourFinder.findContours(depth_output, 10, (w * h) / 2, 20, false);

		// Release CV memory usage
		depthImg.release();
		grayThreshNear.release();
		grayThreshFar.release();
		output.release();
		output_blur.release();
		output_and.release();
		depth_output.clear();
	}
}

void wezside::BlobManager::draw()
{
	if (bShow && bEnable && bDraw)
	{
		ofSetColor(255.0f);
		ofPoint offset = ofPoint(0, 200);

		// Draw pipelined processed image
		if (texture.isAllocated())
		{
			texture.draw(0, offset.y, DEPTH_WIDTH, DEPTH_HEIGHT);
		}		
		if (texture_depth.isAllocated())
		{
			texture_depth.draw(DEPTH_WIDTH, offset.y, DEPTH_WIDTH, DEPTH_HEIGHT);
		}
		
		// Draw contours
		ofFill();
		contourFinder.draw(0, offset.y, DEPTH_WIDTH, DEPTH_HEIGHT);

		// Draw centroids
		if (bDrawCentroid)
		{
			ofSetColor(255.0f, 0.0f, 59.0f);
			int size = contourFinder.blobs.size();
			//std::vector<ofxCvBlob> blobsToSend;
			ofxOscMessage m;
			m.setAddress("/kinect/" + ofToString(id));
			for (int i = 0; i < size; i++)
			{
				ofPolyline_<ofVec3f> tmp;
				tmp.addVertices(contourFinder.blobs[i].pts);
				if (abs(tmp.getArea() / 1000.0f) < fBlobSize)
				{
					continue;
				}
				//blobsToSend.push_back(contourFinder.blobs[i]);
				m.addFloatArg(contourFinder.blobs[i].centroid.x);
				m.addFloatArg(contourFinder.blobs[i].centroid.y);
				ofDrawCircle(contourFinder.blobs[i].centroid + offset, 3);

				
			}

			if (bSendData && ofGetFrameNum() % nSendFrequency == 0)
			{
				//ofBuffer buffer;
				//buffer.append(ofToString(ofGetFrameNum()));
				
				//m.addBlobArg(buffer);
				
				send(m);
			}
		}
	}
	wezside::TrackingManager::draw();
}

void wezside::BlobManager::exit()
{
	output_and.release();
}

void wezside::BlobManager::send(ofxOscMessage data)
{
	sender.sendMessage(data);
}