#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ModCell : public CCNode {
protected:
    bool init(Mod* mod);
    void updateState();

    void onEnable(CCObject*);

public:
    CCScale9Sprite* m_bg;
    CCNode* m_logo;
    CCNode* m_infoContainer;
    CCNode* m_titleContainer;
    CCLabelBMFont* m_titleLabel;
    CCLabelBMFont* m_versionLabel;
    CCNode* m_developers;
    CCLabelBMFont* m_developerLabel;
    CCMenu* m_viewMenu;
    CCMenuItemToggler* m_enableToggle = nullptr;

    Mod* m_mod;

    static ModCell* create(Mod* mod);
    void updateSize(float width, bool big);
};