#include <filesystem>
#include <Geode/Geode.hpp>
#include "utils/ModProfiles.hpp"
#include "utils/Mods.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {

    if (!std::filesystem::exists(geode::dirs::getTempDir() / GEODE_MOD_ID)) {
        std::filesystem::create_directory(geode::dirs::getTempDir() / GEODE_MOD_ID);
    } else {
        if (std::filesystem::exists(dirs::getTempDir() / GEODE_MOD_ID / "profiles")) {
            for (auto file : std::filesystem::directory_iterator(dirs::getTempDir() / GEODE_MOD_ID / "profiles")) {
                std::filesystem::remove_all(file.path());
            }
        } else {
            std::filesystem::create_directory(geode::dirs::getTempDir() / GEODE_MOD_ID / "profiles");
        }
        if (std::filesystem::exists(dirs::getTempDir() / GEODE_MOD_ID / "unzip")) {
            for (auto file : std::filesystem::directory_iterator(dirs::getTempDir() / GEODE_MOD_ID / "unzip")) {
                std::filesystem::remove_all(file.path());
            }
        } else {
            std::filesystem::create_directory(geode::dirs::getTempDir() / GEODE_MOD_ID / "unzip");
        }
    }

    if (std::filesystem::exists(Mod::get()->getSaveDir() / "packs")) {
        for (auto pack : std::filesystem::directory_iterator(fmt::format("{}/packs", Mod::get()->getSaveDir()))) {
            auto res = ModProfile::loadFromPath(pack);
            if (res.isOk()) {
                modutils::Mod::get()->profiles.push_back(res.unwrap());
            } else {
                log::error("Failed to load a profile: ", res.unwrapErr());
            }
        }
    }
}