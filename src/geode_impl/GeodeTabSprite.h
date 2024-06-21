/*
Implementation of GeodeTabSprite
https://github.com/geode-sdk/geode/blob/main/loader/src/ui/mods/GeodeStyle.hpp
*/

#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GeodeTabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static GeodeTabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);
    
    void select(bool selected);
    void disable(bool disabled);
};