#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include "ExportModsLayer.h"

bool ExportModsLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    m_background = createLayerBG();
    m_background->setID("mod-profiles-background");
    this->addChild(m_background);

    outline = CCSprite::createWithSpriteFrameName("mod-profiles-profile-selection.png"_spr);
    this->addChild(outline);
    outline->setPosition(winSize / 2);
    outline->setZOrder(1);

    return true;
}

ExportModsLayer* ExportModsLayer::create() {
    auto ret = new ExportModsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCScene* ExportModsLayer::scene() {
    auto layer = ExportModsLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}