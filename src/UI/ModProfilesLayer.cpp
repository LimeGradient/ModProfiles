#include "ModProfilesLayer.hpp"
#include "UI/Utils/MPListType.hpp"

bool ModProfilesLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto director = CCDirector::get();

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

    CCMenuItemSpriteExtra* backBtn = Build<CCMenuItemSpriteExtra>::create(backSpr, this, menu_selector(ModProfilesLayer::onClose))
        .id("back-button")
        .collect();

    CCMenu* backMenu = Build<CCMenu>::create()
        .id("back-menu")
        .pos(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f))
        .zOrder(1)
        .parent(this)
        .child(backBtn)
        .collect();

    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    auto bg = geode::createLayerBG();
    bg->setID("content-background");
    this->addChild(bg);

    CCMenu* listSelectMenu = Build<CCMenu>::create()
        .id("list-select-menu")
        .layout(ColumnLayout::create()->setAutoScale(false)->setGap(5.f))
        .parent(backMenu)
        .collect();

    Build<MPListLayer>::create(ModBackend::get()->getMods(), "Export Mods", MPListType::ExportList)
        .id("mp-list-layer")
        .parent(this)
        .center()
        .store(m_mpListLayer);
        
    return true;
}

void ModProfilesLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
}

void ModProfilesLayer::keyBackClicked() {
    this->onClose(nullptr);
}

ModProfilesLayer* ModProfilesLayer::create() {
    auto ret = new ModProfilesLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCScene* ModProfilesLayer::scene() {
    auto layer = ModProfilesLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}