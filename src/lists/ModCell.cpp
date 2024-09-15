#include "ModCell.h"

#include <Geode/Geode.hpp>
#include <Geode/loader/ModMetadata.hpp>
#include <Geode/ui/GeodeUI.hpp>
using namespace geode::prelude;

bool ModCell::init(Mod* mod) {
    if (!CCNode::init()) return false;

    m_mod = mod;

    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .0f, .0f });
    m_bg->setScale(.7f);
    m_bg->setColor(ccWHITE);
    m_bg->setOpacity(25);
    this->addChild(m_bg);

    m_logo = geode::createModLogo(mod);
    m_logo->setID("logo-sprite");
    m_logo->setScale(0.5f);
    m_logo->setAnchorPoint({.0f, .0f});
    m_logo->setPositionY(m_logo->getPositionY() + 2.f);
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setID("info-container");
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({ .0f, .0f });
    m_infoContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0)
    );
    m_infoContainer->getLayout()->ignoreInvisibleChildren(true);
    m_infoContainer->setPositionY(m_infoContainer->getPositionY() + 20.f);

    m_titleContainer = CCNode::create();
    m_titleContainer->setID("title-container");
    m_titleContainer->setAnchorPoint({ .0f, .5f });
    m_titleContainer->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setAxisAlignment(AxisAlignment::Start)
    );

    m_titleLabel = CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt");
    m_titleLabel->setID("title-label");
    m_titleContainer->addChild(m_titleLabel);

    m_versionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_versionLabel->setID("version-label");
    m_versionLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(std::nullopt, .7f));
    m_titleContainer->addChild(m_versionLabel);
    
    m_infoContainer->addChild(m_titleContainer);
    
    m_developers = CCMenu::create();
    m_developers->setID("developers-menu");
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({ .0f, .5f });

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(mod->getDevelopers());
    m_developerLabel = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    m_developerLabel->setID("developers-label");
    m_developers->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    m_developers->addChild(m_developerLabel);
    m_developers->setPosition({0.f, 130.f});
    m_infoContainer->addChild(m_developers);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setAnchorPoint({ 1.f, .0f });
    m_viewMenu->setScale(.55f);
     m_viewMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(10)
    );
    m_viewMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));
    m_viewMenu->setPositionY(m_viewMenu->getPositionY() + 4.f);

    m_enableToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(ModCell::onEnable), 1.f);
    m_enableToggle->setID("enable-toggler");
    m_viewMenu->addChild(m_enableToggle);
    m_viewMenu->updateLayout();

    this->addChild(m_infoContainer);
    this->setContentSize(CCSize{m_bg->getScaledContentWidth(), m_bg->getScaledContentHeight()});
    m_infoContainer->setContentSize(this->getScaledContentSize());
    this->setID(mod->getID());

    return true;
}

void ModCell::onEnable(CCObject* sender) {
    
}

ModCell* ModCell::create(Mod* mod) {
    auto ret = new ModCell();
    if (ret && ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}