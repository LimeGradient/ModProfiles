#include <filesystem>

#include <Geode/Geode.hpp>
#include "ModProfilesLayer.h"

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;
namespace fs = std::filesystem;

class $modify(ModProfilesMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("mod-profiles-button.png"_spr),
			this,
			menu_selector(ModProfilesMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);
		myButton->setID("mod-profiles-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		auto modProfilesLayer = ModProfilesLayer::scene();
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, modProfilesLayer));
	}
};

