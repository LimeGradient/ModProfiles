#include "Cell.hpp"

#include <Geode/loader/ModMetadata.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <utils/ModProfiles.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

bool Cell::init(CellType type, Mod* mod, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_mod = mod;
    this->setID(mod->getID());

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(25)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .height(35.f)
        .color(ccWHITE)
        .parent(this)
        .store(this->m_bg);

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

    auto logo = Build(geode::createModLogo(mod))
        .id("logo")
        .scale(.5f)
        .anchorPoint({.0f, .5f})
        .collect();
    this->m_infoContainer->addChildAtPosition(logo, Anchor::Left, ccp(5.f, 0.f));

    Build<CCNode>::create()
        .id("title-container")
        .anchorPoint({.0f, .5f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(false)
        )
        .width(width)
        .store(this->m_titleContainer);
    this->m_infoContainer->addChildAtPosition(this->m_titleContainer, Anchor::TopLeft, ccp(35.f, -10.f));

    Build(CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt"))
        .id("title-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.5f})
        .parent(this->m_titleContainer)
        .store(this->m_titleLabel);

    auto versionLabel = Build(CCLabelBMFont::create(fmt::format("v{}", mod->getVersion().toNonVString()).c_str(), "bigFont.fnt"))
        .id("version-label")
        .scale(0.2)
        .anchorPoint({0.f, 0.575f})
        .color(ccc3(0, 255, 0))
        .parent(this->m_titleContainer)
        .collect();
    m_titleContainer->updateLayout();
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .layout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(this->m_developerMenu);
    
    auto devLabel = Build(CCLabelBMFont::create(ModMetadata::formatDeveloperDisplayString(mod->getDevelopers()).c_str(), "goldFont.fnt"))
        .id("developer-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.35f})
        .collect();
    this->m_infoContainer->addChildAtPosition(devLabel, Anchor::BottomLeft, ccp(35.f, 10.f));

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(10);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({0.f, 0.f})
        .contentSize({50.f, 50.f})
        .scale(.55f)
        .layout(viewLayout)
        .store(this->m_viewMenu);
    
    Build(CCMenuItemToggler::createWithStandardSprites(this, menu_selector(Cell::onPackEnable), 1.f))
        .id("toggler")
        .parent(m_viewMenu)
        .store(m_toggler);
    
    this->m_infoContainer->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-15.f, 0.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

bool Cell::init(CellType type, ModProfile profile, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_profile = profile;
    this->setID(profile.id);

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(25)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .height(35.f)
        .color(ccWHITE)
        .parent(this)
        .store(this->m_bg);

    auto texture = new CCTexture2D();
    texture->initWithImage(profile.logo);
    auto sprite = CCSprite::createWithTexture(texture);
    texture->release();
    
    auto logo = Build(sprite)
        .id("logo")
        .scale(.15f)
        .anchorPoint({.5f, .5f})
        .collect();
    this->addChildAtPosition(logo, Anchor::Left, ccp(17.5, 17.5));

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
        .anchorPoint({.0f, .5f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(false)
        )
        .width(width)
        .store(this->m_titleContainer);
    this->m_infoContainer->addChildAtPosition(this->m_titleContainer, Anchor::TopLeft, ccp(35.f, -10.f));

    Build(CCLabelBMFont::create(profile.name.c_str(), "bigFont.fnt"))
        .id("title-label")
        .scale(0.35f)
        .anchorPoint(0.f, 0.5f)
        .parent(this->m_titleContainer)
        .store(this->m_titleLabel);
    
    Build(CCLabelBMFont::create(fmt::format("v{}", profile.version).c_str(), "bigFont.fnt"))
        .id("version-label")
        .scale(0.2)
        .anchorPoint({0.f, 0.575f})
        .color(ccc3(0, 255, 0))
        .parent(this->m_titleContainer)
        .collect();
    this->m_titleContainer->updateLayout();
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .layout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(this->m_developerMenu);
    
    auto devLabel = Build(CCLabelBMFont::create(profile.creator.c_str(), "goldFont.fnt"))
        .id("developer-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.5f})
        .collect();
    this->m_infoContainer->addChildAtPosition(devLabel, Anchor::BottomLeft, ccp(35.f, 10.f));

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(10);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({0.f, 0.f})
        .contentSize({50.f, 50.f})
        .scale(.55f)
        .layout(viewLayout)
        .store(this->m_viewMenu);
    
    Build(CCMenuItemToggler::createWithStandardSprites(this, menu_selector(Cell::onModEnable), 1.f))
        .id("toggler")
        .parent(m_viewMenu)
        .store(m_toggler);
    
    this->m_infoContainer->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-15.f, 0.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

void Cell::onModEnable(CCObject*) {

}

void Cell::onPackEnable(CCObject*) {

}

Cell* Cell::create(CellType type, Mod* mod, float width) {
    auto ret = new Cell();
    if (ret->init(type, mod, width)) {
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