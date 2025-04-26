#include <Geode/Geode.hpp>
#include <alphalaneous.alphas_geode_utils/include/NodeModding.h>
#include "ui/ModProfilesLayer.hpp"

using namespace geode::prelude;

class $nodeModify(MyModsLayer, ModsLayer) {

    struct Fields {};

	void modify() {
		// Borrowed this check from Unlog, thanks alpha
		bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

		if (CCMenu* btnMenu = typeinfo_cast<CCMenu*>(getChildByID("right-actions-menu"))) {
			CCSprite* spr = CircleButtonSprite::createWithSpriteFrameName("profiles.png"_spr, 1.f, geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green);
			spr->setScale(0.8f);
		    CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyModsLayer::onModProfiles));
		    btn->setID("mod-profiles"_spr);

		    btnMenu->addChild(btn);
			btnMenu->updateLayout();
		}
	}

	void onModProfiles(CCObject* obj) {
		CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, ModProfilesLayer::scene()));
	}
};