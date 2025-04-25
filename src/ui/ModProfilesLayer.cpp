#include "ModProfilesLayer.hpp"

#include <Geode/ui/GeodeUI.hpp>

bool ModProfilesLayer::init() {
    if (!CCLayer::init()) return false;

    auto winSize = cocos2d::CCDirector::get()->getWinSize();
    this->setID("ServerListLayer"_spr);
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    auto bg = geode::createLayerBG();
    bg->setID("background");
    this->addChild(bg);

    auto m_backMenu = cocos2d::CCMenu::create();
    m_backMenu->setID("back-menu");
    m_backMenu->setContentHeight(100.f);
    m_backMenu->setAnchorPoint({ .5f, 1.f });
    
    auto backSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(ModProfilesLayer::onBack)
    );
    backBtn->setID("back-button");
    m_backMenu->addChild(backBtn);

    m_backMenu->setLayout(
        geode::ColumnLayout::create()
            ->setAxisAlignment(geode::AxisAlignment::End)
            ->setAxisReverse(true)
    );
    this->addChildAtPosition(m_backMenu, geode::Anchor::TopLeft, ccp(25.f, -25.f / 4.f), false);

    return true;
}

ModProfilesLayer *ModProfilesLayer::create() {
    auto ret = new ModProfilesLayer;
    if (!ret->init()) {
        delete ret;
        return nullptr;
    }
    ret->autorelease();
    return ret;
}

CCScene *ModProfilesLayer::scene() {
    auto scene = CCScene::create();
    scene->addChild(ModProfilesLayer::create());
    return scene;
}

void ModProfilesLayer::onBack(CCObject *sender) {
    openModsList();
}

void ModProfilesLayer::keyBackClicked() {
    this->onBack(nullptr);
}