#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	double m_padding = -1.0;
	double m_scale = -1.0;
	cocos2d::ccColor4B m_color = {0, 0, 0, 0};
	int m_font = -1;
	bool m_enabled = false;
	bool m_readableFont = false;
	std::string m_fontFile = "bigFont.fnt";

	static Manager* getSharedInstance() {
		if (!instance) {
			instance = new Manager();
		}
		return instance;
	}

};