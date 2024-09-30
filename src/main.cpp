#include <filesystem>

#include <Geode/Geode.hpp>
#include "ModProfilesLayer.h"
#include "utils/ziputils.h"

#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;
namespace fs = std::filesystem;

$execute {
	std::thread([=] {
		Zip* zip = new Zip("");
		if (!fs::exists(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
			fs::create_directory(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()));
		}
		for (auto file : fs::directory_iterator(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
			if (file.is_directory() || file.path().extension().string() != ".geodepack") continue;
			auto unzippedDir = fmt::format("{}/geodepacks/{}", geode::dirs::getGeodeDir(), file.path().stem().string());
        	zip->unzipIntoFolder(file.path().string(), unzippedDir);
		}
	}).detach();
}

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

