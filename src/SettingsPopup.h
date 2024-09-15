#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SettingsPopup : public geode::Popup<> {
protected:
    std::vector<CCNode*> m_settingsTogglers;
    bool m_hasAppliedSettings;

    CCMenuItemSpriteExtra* m_applyBtn;

    bool setup() override;

    void onEnable(CCObject*);
    void onApply(CCObject*);
    void onClose(CCObject*) override;

    CCNode* createBoolSetting(std::string title, std::string id, CCMenu* parentMenu);
public:
    static constexpr float POPUP_WIDTH = 300.f;
    static constexpr float POPUP_HEIGHT = 300.f;

    static SettingsPopup* create();
};