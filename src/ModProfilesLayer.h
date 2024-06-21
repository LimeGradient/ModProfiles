#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ModProfilesLayer : public CCLayer {
protected:
    CCSprite* m_background;
    ScrollLayer* scrollLayer = nullptr;
    CCSprite* outline;

    void keyBackClicked() override;

public:
    static ModProfilesLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose(CCObject*);
};