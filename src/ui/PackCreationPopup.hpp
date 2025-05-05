#pragma once

#include <Geode/Geode.hpp>

class PackCreationPopup : public geode::Popup<const std::vector<geode::Mod*>&> {
protected:
    static constexpr float width = 400.f;
    static constexpr float height = 300.f;

    bool setup(const std::vector<geode::Mod*>& mods);

    void onLogoSelect(geode::Task<geode::Result<std::filesystem::path>>::Event *event);

    geode::TextInput* m_nameInput;
    geode::TextInput* m_idInput;
    geode::TextInput* m_descriptionInput;
    geode::TextInput* m_authorInput;
    geode::TextInput* m_versionInput;

    cocos2d::CCSprite* m_logoPreview;
    std::string m_logoPath;

    geode::EventListener<geode::Task<geode::Result<std::filesystem::path>>> m_pickListener;

public:
    static PackCreationPopup* create(const std::vector<geode::Mod*>& mods);
};