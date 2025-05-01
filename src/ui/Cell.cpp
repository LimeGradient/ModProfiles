#include "Cell.hpp"

#include <Geode/loader/ModMetadata.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <utils/ModProfiles.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

bool Cell::init(CellType type, matjson::Value data, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_data = data;

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(25)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .color(ccWHITE)
        .parent(this)
        .store(this->m_bg);

    Build(geode::createModLogo(data["logo"].asString().unwrap()))
        .id("logo")
        .scale(.5f)
        .anchorPoint({.0f, .0f})
        .parent(this)
        .store(this->m_logo);

    auto infoLayout = ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setGap(0);
    infoLayout->ignoreInvisibleChildren(true);

    Build<CCNode>::create()
        .id("info-container")
        .scale(.4f)
        .anchorPoint({.0f, .0f})
        .layout(infoLayout)
        .parent(this)
        .store(this->m_infoContainer);

    Build<CCNode>::create()
        .id("title-container")
        .anchorPoint({.0f, .0f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
        )
        .parent(this->m_infoContainer)
        .store(this->m_titleContainer);
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .parent(this->m_infoContainer)
        .store(this->m_developerMenu);

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(10);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({1.f, 0.f})
        .scale(.55f)
        .layout(viewLayout)
        .store(this->m_viewMenu);
    
    this->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-10.f, 4.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

bool Cell::init(CellType type, ModProfile profile, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_profile = profile;

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(25)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .color(ccWHITE)
        .parent(this)
        .store(this->m_bg);

    auto texture = new CCTexture2D();
    texture->initWithImage(profile.logo);
    auto sprite = CCSprite::createWithTexture(texture);
    texture->release();
    
    auto logo = Build(sprite)
        .id("logo")
        .scale(.175f)
        .anchorPoint({.5f, .5f})
        .collect();
    this->addChildAtPosition(logo, Anchor::Left, ccp(20, 20));

    auto infoLayout = ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setGap(0);
    infoLayout->ignoreInvisibleChildren(true);

    Build<CCNode>::create()
        .id("info-container")
        .anchorPoint({.0f, .0f})
        .contentSize(this->m_bg->getScaledContentSize())
        .layout(infoLayout)
        .parent(this)
        .store(this->m_infoContainer);

    Build<CCNode>::create()
        .id("title-container")
        .anchorPoint({.0f, .0f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
        )
        .parent(this->m_infoContainer)
        .store(this->m_titleContainer);

    Build(CCLabelBMFont::create(profile.name.c_str(), "bigFont.fnt"))
        .id("title-label")
        .scale(0.5f)
        .store(this->m_titleLabel);
    this->m_infoContainer->addChildAtPosition(this->m_titleLabel, Anchor::Top, ccp(0.f, -15.f));
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .layout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(this->m_developerMenu);
    this->m_infoContainer->addChildAtPosition(this->m_developerMenu, Anchor::Bottom, ccp(0.f, 15.f));
    
    auto devLabel = Build(CCLabelBMFont::create(profile.creator.c_str(), "goldFont.fnt"))
        .id("developer-label")
        .scale(0.5f)
        .collect();

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(10);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({1.f, 0.f})
        .scale(.55f)
        .layout(viewLayout)
        .store(this->m_viewMenu);
    
    this->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-10.f, 4.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

Cell* Cell::create(CellType type, matjson::Value data, float width) {
    auto ret = new Cell();
    if (ret->init(type, data, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

Cell* Cell::create(CellType type, ModProfile profile, float width) {
    auto ret = new Cell();
    if (ret->init(type, profile, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}