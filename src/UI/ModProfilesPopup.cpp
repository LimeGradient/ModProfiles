#include "ModProfilesPopup.hpp"

bool ModProfilesPopup::setup() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("Mod Profiles");

    auto* btnMenu = Build<CCMenu>::create()
        .pos({this->popupWidth / 1.5f, this->popupHeight + 10.f})
        .layout(RowLayout::create()->setGap(20.f)->setAutoScale(false))
        .parent(this)
        .collect();

    auto* exportBtn = Build<ButtonSprite>::create("Export Pack", "bigFont.fnt", "geode.loader/GE_button_05.png", 0.8f)
        .intoMenuItem([this](auto) {
            
        })
        .scale(0.75f)
        .parent(btnMenu)
        .collect();

    auto* importPackBtn = Build<ButtonSprite>::create("Import Pack", "bigFont.fnt", "geode.loader/GE_button_05.png", 0.8f)
        .intoMenuItem([this](auto) {
            
        })
        .scale(0.75f)
        .parent(btnMenu)
        .collect();

    
    btnMenu->updateLayout();

    return true;
}