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
        static std::vector<std::string> exceptions = getExceptions();
        mods.remove_if([](geode::Mod* mod) {
            return std::find(exceptions.begin(), exceptions.end(), mod->getID()) != exceptions.end();
        });

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
            if (!res->ok()) callback(ModProfile::Mod::ModType::packed);
            else {
                auto json = res->json().unwrap();
                if (json["error"].asString().unwrap().empty()) {
                    for (auto version : json["payload"]["versions"]) {
                        if (version["status"].asString().unwrapOrDefault() != "accepted") continue;
                        if (version["version"].asString().unwrapOrDefault() == mod->getVersion().toNonVString()) {
                            callback(ModProfile::Mod::ModType::index);
                        } else {
                            callback(ModProfile::Mod::ModType::packed);
                        }
                        break;
                    }
                } else {
                    callback(ModProfile::Mod::ModType::packed);
                }
            }
        }, [](auto p) {}, []{});
    }

    geode::Result<void> Mod::createPack(ModProfile profile, std::string logoPath, std::string filePath) {
        matjson::Value profileJSON = profile;
        auto zipRes = file::Zip::create(filePath);
        file::Zip* zip;
        if (!zipRes) {
            geode::log::error("Create Pack Error: {}", zipRes.unwrapErr());
            return geode::Err(zipRes.unwrapErr());
        } else {
            zip = &zipRes.unwrap();
        }

        std::vector<std::string> localPaths;
        for (auto mod : profile.mods) {
            switch (mod.type) {
                case ModProfile::Mod::ModType::packed: {
                    auto addLocalMods = zip->addFrom(mod.path, "mods");
                    if (!addLocalMods) {
                        geode::log::error("Error adding local mods: {}", addLocalMods.unwrapErr());
                        return geode::Err(addLocalMods.unwrapErr());
                    } else {
                        addLocalMods.unwrap();
                        profileJSON["mods"].set(mod.id, matjson::makeObject({
                            {"type", mod.typeToString()}
                        }));
                    }
                    break;
                }
                case ModProfile::Mod::ModType::index: {
                    profileJSON["mods"].set(mod.id, matjson::makeObject({
                        {"type", mod.typeToString()}
                    }));
                }
                case ModProfile::Mod::ModType::remote: {
                    profileJSON["mods"].set(mod.id, matjson::makeObject({
                        {"type", mod.typeToString()},
                        {"url", mod.url}
                    }));
                }
            }
        }

        auto addProfileJSON = zip->add("profile.json", profileJSON.dump());
        if (!addProfileJSON) {
            geode::log::error("Error adding profile JSON: {}", addProfileJSON.unwrapErr());
            return geode::Err(addProfileJSON.unwrapErr());
        } else {
            addProfileJSON.unwrap();
        }
        if (!std::filesystem::exists(geode::dirs::getTempDir() / GEODE_MOD_ID)) {
            std::filesystem::create_directory(geode::dirs::getTempDir() / GEODE_MOD_ID);
        } else if (std::filesystem::exists(geode::dirs::getTempDir() / GEODE_MOD_ID / "logo.png" )) {
            std::filesystem::remove_all(geode::dirs::getTempDir() / GEODE_MOD_ID / "logo.png");
        }
        std::error_code error;
        std::filesystem::copy(logoPath, geode::dirs::getTempDir() / GEODE_MOD_ID / "logo.png", error);
        if (error) {
            geode::log::error("Error moving logo: {}", error.message());
            return geode::Err("Error moving logo: {}", error.message());
        }
        auto addLogo = zip->addFrom(geode::dirs::getTempDir() / GEODE_MOD_ID / "logo.png");
        if (!addLogo) {
            geode::log::error("Error adding logo: {}", addLogo.unwrapErr());
            return geode::Err("Error moving logo: {}", addLogo.unwrapErr());
        }
        addLogo.unwrap();
        std::filesystem::remove_all(geode::dirs::getTempDir() / GEODE_MOD_ID / "logo.png");

        return geode::Ok();
    }

    void Mod::downloadMod(std::string id, std::function<void()> callback) {
        web::WebRequest req;
        req.get(fmt::format("https://api.geode-sdk.org/v1/mods/{}/versions/latest/download", id)).listen([callback](web::WebResponse* res) {
            if (res->ok()) {
                auto moveMod = res->into(geode::dirs::getModsDir());
                if (moveMod) {
                    moveMod.unwrap();
                    callback();
                } else {
                    moveMod.unwrapErr();
                }
            }
        }, [](auto p){}, []{});
    }

    geode::Result<void> Mod::importPack(ModProfile profile) {
        for (auto mod : std::filesystem::directory_iterator(geode::dirs::getModsDir())) {
            static std::vector<std::string> exceptions = getExceptions();
            auto name = mod.path().stem().string();
            geode::log::info("{}", name);
            if (std::find(exceptions.begin(), exceptions.end(), name) == exceptions.end()) {
                std::filesystem::remove(mod);
            }
        }

        std::vector<ModProfile::Mod> installedMods;

        for (auto mod : profile.mods) {
            geode::log::info("mod id and type: {} - {}", mod.id, mod.typeToString());
            switch (mod.type) {
                case ModProfile::Mod::ModType::index: {
                    this->downloadMod(mod.id, [mod, installedMods]() mutable {
                        installedMods.push_back(mod);
                    });
                    break;
                }

                case ModProfile::Mod::ModType::packed: {
                    for (auto mod : std::filesystem::directory_iterator(geode::dirs::getTempDir() / GEODE_MOD_ID / "profiles" / profile.id / "mods")) {
                        std::filesystem::rename(mod, geode::dirs::getModsDir() / mod.path().filename());
                    }
                    break;
                }

                case ModProfile::Mod::ModType::remote: {
                    break;
                }
            }
        }

        return geode::Ok();
    }

    std::vector<std::string> Mod::getExceptions() {
        static std::vector<std::string> exceptions = {"geode.loader", "limegradient.modprofiles", "alphalaneous.alphas_geode_utils"};
        return exceptions;
    }
}