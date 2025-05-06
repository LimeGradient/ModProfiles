#pragma once

#include <Geode/Geode.hpp>

#include <utils/ModProfiles.hpp>

class PackCreationPopup : public geode::Popup<std::vector<geode::Mod*> const&> {
protected:
    static constexpr float width = 400.f;
    static constexpr float height = 300.f;

    bool setup(std::vector<geode::Mod*> const& mods);

    geode::Result<std::vector<ModProfile::Mod>> getProfileMods();

    void onExport(geode::Task<geode::Result<std::filesystem::path>>::Event *event);
    void onLogoSelect(geode::Task<geode::Result<std::filesystem::path>>::Event *event);

    geode::TextInput* m_nameInput;
    geode::TextInput* m_idInput;
    geode::TextInput* m_descriptionInput;
    geode::TextInput* m_authorInput;
    geode::TextInput* m_versionInput;

    geode::LazySprite* m_logoPreview;
    std::string m_logoPath;

    std::vector<geode::Mod*> mods;
    std::vector<ModProfile::Mod> modProfileMods;

    geode::EventListener<geode::Task<geode::Result<std::filesystem::path>>> m_pickListener;

public:
    static PackCreationPopup* create(std::vector<geode::Mod*> const& mods);
};