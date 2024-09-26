#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ModProfilesLayer : public CCLayer {
protected:
    CCSprite* m_background;
    ScrollLayer* scrollLayer = nullptr;
    CCSprite* outline;
    CCNode* m_frame;
    CCNode* m_currentList;
    
    void keyBackClicked() override;
    void switchList(CCNode* list);
public:
    std::vector<CCMenuItemSpriteExtra*> m_tabs;

    static ModProfilesLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose(CCObject*);
    void onSettings(CCObject*);
    void onTab(CCObject* sender);
};