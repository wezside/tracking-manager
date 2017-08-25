#include "trackingmanager.hpp"

void wezside::TrackingManager::init(std::string _name, int _id)
{
	name = _name;
	id = _id;
	if (name.compare("default") == 0)
	{
		ofLogWarning() << "Settings won't be saved - give your manager a name";
	}
	else gui.loadFromFile(ofToDataPath("settings.xml"));

	sender.setup(HOST, PORT);
	ofxOscMessage m;
	m.setAddress("/init/" + ofToString(id));
	m.addStringArg("Hello World");
	ofLogNotice() << "/init/" + ofToString(id);
	sender.sendMessage(m);
}

void wezside::TrackingManager::toggle()
{
	bShow = !bShow;
	ofLogNotice() << "[" << name << "] Toggle Visisbility " << bShow;
}

#ifdef _WIN32
void wezside::TrackingManager::update(const std::shared_ptr<ofxKinectForWindows2::Source::Depth>& depth, const std::shared_ptr<ofxKinectForWindows2::Source::Body>& body)
{
}

void wezside::TrackingManager::update(const ofPixels& frame)
{
}
#else
void wezside::TrackingManager::update(const ofPixels& frame)
{
}
#endif

void wezside::TrackingManager::draw()
{
	if (bShow)
	{
		gui.draw();
	}
}

void wezside::TrackingManager::send(ofxOscMessage data)
{
	if (bShow && bSendData)
	{

	}
}

void wezside::TrackingManager::setPosition(float x, float y)
{
	gui.setPosition(x, y);
}

void wezside::TrackingManager::exit()
{
	if (name.compare("default") == 0)
	{
		ofLogWarning() << "Couldn't save settings file - name was set to 'default'";
	}
	else gui.saveToFile(ofToDataPath("settings.xml")); 
}
