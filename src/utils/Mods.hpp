#pragma once

#include <vector>

#include <Geode/loader/Mod.hpp>

#include <utils/ModProfiles.hpp>

namespace modutils {
    class Mod {
    public:
        static Mod* get() {
            static Mod instance;
            return &instance;
        }

        std::vector<geode::Mod*> getAllMods();
        matjson::Value jsonFromMod(geode::Mod* mod);

        void isIndexMod(geode::Mod* mod, std::function<void(ModProfile::Mod::ModType)> callback);
        geode::Result<void> createPack(ModProfile profile, std::string logoPath, std::string filePath);

        void downloadMod(std::string id, std::function<void()> callback);
        geode::Result<void> importPack(ModProfile profile);
    };
}