#include "Mods.hpp"

#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>

using namespace geode::utils;

namespace modutils {
    std::vector<geode::Mod*> Mod::getAllMods() {
        auto sort_by_name = [](geode::Mod* a, geode::Mod* b) {
            return a->getName() < b->getName();
        };
    
        std::list<geode::Mod*> mods;
        std::ranges::copy(geode::Loader::get()->getAllMods(), std::back_inserter(mods));
        mods.sort(sort_by_name);

        return std::vector<geode::Mod*>{std::begin(mods), std::end(mods)};
    }

    matjson::Value Mod::jsonFromMod(geode::Mod* mod) {
        return matjson::makeObject({
            {"id", mod->getID()},
            {"name", mod->getName()},
            {"version", mod->getVersion()},
            {"developers", mod->getDevelopers()},
            {"logo", mod->getPackagePath()}
        });
    }

    void Mod::isIndexMod(geode::Mod* mod, std::function<void(ModProfile::Mod::ModType)> callback) {
        web::WebRequest req;
        req.get(fmt::format("https://api.geode-sdk.org/v1/mods/{}", mod->getID())).listen([callback, mod](web::WebResponse* res) {
            if (res->ok()) {
                auto json = res->json().unwrap();
                if (json["error"].asString().unwrap().empty()) {
                    auto versions = json["payload"]["versions"].asArray().unwrap();
                    auto latestVersion = versions.front()["version"].asString().unwrap();
                    if (latestVersion == mod->getVersion().toNonVString()) {
                        callback(ModProfile::Mod::ModType::index);
                    } else {
                        callback(ModProfile::Mod::ModType::packed);
                    }
                } else {
                    callback(ModProfile::Mod::ModType::packed);
                }
            }
        }, [](auto p) {}, []{});
    }

    void Mod::createPack(ModProfile profile, std::string logoPath, std::string filePath) {
        matjson::Value profileJSON = profile;
        auto zipRes = file::Zip::create(filePath + ".modprofile");
        file::Zip* zip;
        if (!zipRes) {
            geode::log::error("Create Pack Error: {}", zipRes.unwrapErr());
        } else {
            zip = &zipRes.unwrap();
        }

        std::vector<std::string> localPaths;
        for (auto mod : profile.mods) {
            geode::log::info("mod info: {} - {}", mod.id, mod.typeToString());
            if (mod.type == ModProfile::Mod::ModType::packed) {
                geode::log::info("mod path: {}", mod.path);
                auto addLocalMods = zip->addFrom(mod.path, "mods");
                if (!addLocalMods) {
                    geode::log::error("Error adding local mods: {}", addLocalMods.unwrapErr());
                } else {
                    addLocalMods.unwrap();
                }
            }
        }
    }
}