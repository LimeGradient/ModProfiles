#pragma once

#include <Geode/Geode.hpp>

class ModProfilesLayer : public cocos2d::CCLayer {
protected:
    bool init() override;
    void keyBackClicked() override;

    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    std::string m_currentTab = "my-packs";
    std::map<std::string, geode::Ref<geode::ScrollLayer>> m_scrolls;
    geode::EventListener<geode::Task<geode::Result<std::filesystem::path>>> m_pickListener;

    CCNode *m_listFrame = nullptr;
public:
    void onBack(CCObject *sender);
    void onTab(CCObject *sender);
    void onImport(CCObject *sender);
    void onExport(CCObject *sender);

    void goToTab(std::string id);

    void onFileOpen(geode::Task<geode::Result<std::filesystem::path>>::Event *event);

    static ModProfilesLayer *create();
    static cocos2d::CCScene *scene();
};

class TabSprite : public cocos2d::CCNode {
protected:
    cocos2d::extension::CCScale9Sprite* m_deselectedBG;
    cocos2d::extension::CCScale9Sprite* m_selectedBG;
    cocos2d::CCSprite* m_icon;
    cocos2d::CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static TabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);

    void select(bool selected);
    void disable(bool disabled);
};