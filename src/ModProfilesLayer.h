#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ModProfilesLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_background;
    ScrollLayer* scrollLayer = nullptr;

    CCSprite* outline;

public:
    static ModProfilesLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose(CCObject*);
};