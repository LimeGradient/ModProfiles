/*
A lot (if not all) of the UI code is straight from Geode github
i wanted it to look as close as possible to the actual geode menu
https://github.com/geode-sdk/geode
*/

#include <Geode/Geode.hpp>
#include <Geode/utils/ColorProvider.hpp>

using namespace geode::prelude;

#include "ModProfilesLayer.h"
#include "lists/ExportProfilesList.h"
#include "geode_impl/GeodeTabSprite.h"
#include "geode_impl/SwelvyBG.h"

bool ModProfilesLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    this->setID("ModProfilesLayer");

    ColorProvider::get()->define("geode-bg"_spr, { 25, 17, 37, 255 });

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    this->addChild(SwelvyBG::create());

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(ModProfilesLayer::onClose));
    CCMenu* backMenu = CCMenu::create();
    backMenu->addChild(backBtn);
    backMenu->setContentWidth(100.f);
    backMenu->setAnchorPoint({.0f, .5f});
    backMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    backMenu->setZOrder(1);
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(12, -25), false);

    m_frame = CCNode::create();
    m_frame->setID("mod-profiles-frame");
    m_frame->setAnchorPoint({.5f, .5f});
    m_frame->setContentSize({380, 205});

    auto frameBG = CCLayerColor::create(ColorProvider::get()->color("geode-bg"_spr));
    frameBG->setID("frame-bg");
    frameBG->setContentSize(m_frame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    m_frame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName("geode-top.png"_spr);
    tabsTop->setID("frame-top-sprite");
    tabsTop->setAnchorPoint({ .5f, .0f });
    m_frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName("geode-side.png"_spr);
    tabsLeft->setID("frame-left-sprite");
    tabsLeft->setScaleY(m_frame->getContentHeight() / tabsLeft->getContentHeight());
    m_frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6, 0));

    auto tabsRight = CCSprite::createWithSpriteFrameName("geode-side.png"_spr);
    tabsRight->setID("frame-right-sprite");
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(m_frame->getContentHeight() / tabsRight->getContentHeight());
    m_frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6, 0));

    auto tabsBottom = CCSprite::createWithSpriteFrameName("geode-bottom.png"_spr);
    tabsBottom->setID("frame-bottom-sprite");
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    m_frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 2));

    this->addChildAtPosition(m_frame, Anchor::Center, ccp(0, -10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setID("tabs-menu");
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(m_frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 8)));
    // Increment touch priority so the mods in the list don't override
    mainTabs->setTouchPriority(-150);

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, const char*>> {
        { "GJ_shareBtn_001.png", "Export", "export-button" },
        { "GJ_downloadBtn_001.png", "Import", "import-button" },
        { "GJ_duplicateBtn_001.png", "Packs", "packs-button" },
        { "GJ_statsBtn_001.png", "Pack Index", "pack-index-button" }
    }) {
        auto btn = CCMenuItemSpriteExtra::create(
            GeodeTabSprite::create(std::get<0>(item), std::get<1>(item), 120),
            this, menu_selector(ModProfilesLayer::onClose)
        );
        btn->setID(std::get<2>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    auto list = ExportProfilesList::create(m_frame->getContentSize() - ccp(30, 0));
    list->setPosition(m_frame->getContentSize() / 2);
    m_frame->addChild(list);

    return true;
}

void ModProfilesLayer::switchList(CCNode* list) {

}

void ModProfilesLayer::keyBackClicked() {
    this->onClose(nullptr);
}

void ModProfilesLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
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