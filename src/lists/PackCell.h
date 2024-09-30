#pragma once

#include <Geode/Geode.hpp>
#include "utils/PackInfo.h"
#include "PackSelectList.h"

using namespace geode::prelude;

class PackCell : public CCNode {
protected:
    bool init(PackInfo* packInfo);

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

    PackInfo* m_packInfo;

    static PackCell* create(PackInfo* packInfo);
};