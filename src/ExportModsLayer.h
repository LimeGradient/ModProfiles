#pragma once

#include <Geode/Geode.hpp>

class ExportModsLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_background;
    ScrollLayer* scrollLayer = nullptr;

    CCSprite* outline;

public:
    static ExportModsLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose();
};