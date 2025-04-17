#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "UI/ModProfilesLayer.hpp"

using namespace geode::prelude;

class $modify(ModProfilesMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto btnSpr = CCSprite::createWithSpriteFrameName("mod-profiles-button.png"_spr);
		auto btn = CCMenuItemSpriteExtra::create(
			btnSpr,
			this,
			menu_selector(ModProfilesMenuLayer::onModProfiles)
		);
		btn->setID("mod-profiles-btn");

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(btn);
		menu->updateLayout();

		return true;
	}

	void onModProfiles(CCObject*) {
		auto modProfilesLayer = ModProfilesLayer::scene();
		CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, modProfilesLayer));
	}
};