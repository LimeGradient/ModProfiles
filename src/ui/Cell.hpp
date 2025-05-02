#pragma once

#include <Geode/Geode.hpp>
#include <utils/ModProfiles.hpp>

enum class CellType {
    MOD,
    PACK
};

class Cell : public cocos2d::CCNode {
protected:
    bool init(CellType type, geode::Mod* mod, float width);
    bool init(CellType type, ModProfile profile, float width);

    bool initMod(matjson::Value data);
    bool initProfile(matjson::Value data);

    void onModEnable(CCObject*);
    void onPackEnable(CCObject*);

    CellType m_type;
    geode::Mod* m_mod;
    ModProfile m_profile;

    cocos2d::extension::CCScale9Sprite* m_bg;
    cocos2d::CCNode* m_logo;
    cocos2d::CCNode* m_infoContainer;
    cocos2d::CCNode* m_titleContainer;
    cocos2d::CCLabelBMFont* m_titleLabel;
    cocos2d::CCLabelBMFont* m_versionLabel;
    cocos2d::CCMenu* m_developerMenu;
    cocos2d::CCMenu* m_viewMenu;
    CCMenuItemToggler* m_toggler;

public:
    static Cell* create(CellType type, geode::Mod* mod, float width);
    static Cell* create(CellType type, ModProfile profile, float width);
};