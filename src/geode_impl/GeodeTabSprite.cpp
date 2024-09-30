/*
Implementation of GeodeTabSprite
https://github.com/geode-sdk/geode/blob/main/loader/src/ui/mods/GeodeStyle.hpp
*/

#include "GeodeTabSprite.h"

#include <Geode/Geode.hpp>
#include <Geode/utils/ColorProvider.hpp>

$on_mod(Loaded) {
    ColorProvider::get()->define("mod-list-tab-deselected-bg"_spr, { 26, 24, 29, 255 });
    ColorProvider::get()->define("mod-list-tab-selected-bg"_spr, { 168, 147, 185, 255 });
}

bool GeodeTabSprite::init(const char* iconFrame, const char* text, float width, bool altColor) {
    if (!CCNode::init())
        return false;

    const CCSize itemSize { width, 35 };
    const CCSize iconSize { 18, 18 };

    this->setContentSize(itemSize);
    this->setAnchorPoint({ .5f, .5f });

    m_deselectedBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
    m_deselectedBG->setScale(.8f);
    m_deselectedBG->setContentSize(itemSize / .8f);
    m_deselectedBG->setColor("mod-list-tab-deselected-bg"_cc3b);
    this->addChildAtPosition(m_deselectedBG, Anchor::Center);

    m_selectedBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
    m_selectedBG->setScale(.8f);
    m_selectedBG->setContentSize(itemSize / .8f);
    m_selectedBG->setColor(to3B(ColorProvider::get()->color(
        altColor ?
            "mod-list-tab-selected-bg-alt"_spr :
            "mod-list-tab-selected-bg"_spr
    )));
    m_selectedBG->setVisible(false);
    this->addChildAtPosition(m_selectedBG, Anchor::Center);

    m_icon = CCSprite::createWithSpriteFrameName(iconFrame);
    limitNodeSize(m_icon, iconSize, 3.f, .1f);
    this->addChildAtPosition(m_icon, Anchor::Left, ccp(16, 0), false);

    m_label = CCLabelBMFont::create(text, "bigFont.fnt");
    m_label->limitLabelWidth(this->getContentWidth() - 45, clamp(width * .0045f, .35f, .55f), .1f);
    m_label->setAnchorPoint({ .5f, .5f });
    this->addChildAtPosition(m_label, Anchor::Left, ccp((itemSize.width - iconSize.width) / 2 + iconSize.width, 0), false);

    return true;
}

GeodeTabSprite* GeodeTabSprite::create(const char* iconFrame, const char* text, float width, bool altColor) {
    auto ret = new GeodeTabSprite();
    if (ret && ret->init(iconFrame, text, width, altColor)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void GeodeTabSprite::select(bool selected) {
    m_deselectedBG->setVisible(!selected);
    m_selectedBG->setVisible(selected);
}

void GeodeTabSprite::disable(bool disabled) {
    auto color = disabled ? ccc3(95, 95, 95) : ccc3(255, 255, 255);
    m_deselectedBG->setColor(color);
    m_selectedBG->setColor(color);
    m_icon->setColor(color);
    m_label->setColor(color);
}