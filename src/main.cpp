#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "Manager.hpp"

#define getBool getSettingValue<bool>
#define getDouble getSettingValue<double>
#define getInt getSettingValue<int64_t>
#define getColor getSettingValue<cocos2d::ccColor4B>

using namespace geode::prelude;

void updateSettings(Manager* fields = Manager::getSharedInstance(), const Mod* mod = Mod::get()) {
	fields->m_padding = mod->getDouble("padding");
	fields->m_scale = mod->getDouble("scale");
	fields->m_color = mod->getColor("color");
	fields->m_font = mod->getInt("stackSizeFont");
	fields->m_enabled = mod->getBool("enabled");
	fields->m_readableFont = mod->getBool("readableMode");
	fields->m_fontFile = "bigFont.fnt";
	if (fields->m_font == 0) return;
	if (fields->m_font == -1) {
		fields->m_fontFile = "goldFont.fnt";
	} else if (fields->m_font == -2) {
		fields->m_fontFile = "chatFont.fnt";
	} else if (fields->m_font > 0) {
		fields->m_fontFile = fmt::format("gjFont{:02d}.fnt", fields->m_font);
	}
}

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		updateSettings();
		return true;
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	void setupHasCompleted() {
		PlayLayer::setupHasCompleted();
		updateSettings();
	}
};

class $modify(MyEditLevelLayer, EditLevelLayer) {
	bool init(GJGameLevel* p0) {
		if (!EditLevelLayer::init(p0)) return false;
		updateSettings();
		return true;
	}
};

class $modify(MyEditorUI, EditorUI) {
	static void onModify(auto& self) {
		(void) self.setHookPriority("EditorUI::getCreateBtn", -2123456789);
	}
	CCLabelBMFont* makeLabel(int id) {
		auto fields = Manager::getSharedInstance();
		const auto label = CCLabelBMFont::create(fmt::format("{}", id).c_str(), fields->m_fontFile.c_str());
		const auto nodeID = fmt::format("{}/objectID-{}-stacksize", Mod::get()->getID(), id);
		const auto [r, g, b, a] = fields->m_color;
		if (fields->m_readableFont && fields->m_font == -2)
			label->setBlendFunc({GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA});
		label->setAlignment(kCCTextAlignmentRight);
		label->setAnchorPoint({1.f, 0.f});
		label->setColor({r, g, b});
		label->setOpacity(a);
		label->setScale(fields->m_scale);
		label->setID(nodeID);
		return label;
	}
	CreateMenuItem* getCreateBtn(int id, int bg) {
		auto fields = Manager::getSharedInstance();
		const auto result = EditorUI::getCreateBtn(id, bg);
		if (!fields->m_enabled) return result;
		CCLabelBMFont* label = makeLabel(id);
		float padding = fields->m_padding;
		padding /= 2.f;
		label->setPosition({33 - padding, 6 + padding});
		label->setZOrder(1);
		result->addChild(label);
		result->updateLayout();
		return result;
	}
};