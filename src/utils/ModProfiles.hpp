#pragma once

#include <Geode/Geode.hpp>

struct ModProfile {
    struct Mod {
        enum class ModType {
            index,
            packed,
            remote,
        };
        std::string id;
        ModType type;
        // For remote mods
        std::string url;
        // for local mods
        std::string path;

        Mod() = default;
        Mod(std::string id, ModType type, std::string url = "", std::string path = "")
            : id(id), type(type), url(url), path(path) {}
        
        std::string typeToString();
        static ModType stringToType(std::string type);
    };

    std::string name;
    std::string id;
    std::string description;
    std::string creator;
    std::string version;
    geode::Ref<cocos2d::CCImage> logo = nullptr;
    std::vector<ModProfile::Mod> mods;

    ModProfile() = default;
    static geode::Result<ModProfile> loadFromPath(std::filesystem::path file);
    ModProfile(std::string name, std::string id, std::string description, std::string creator, std::string version, geode::Ref<cocos2d::CCImage> logo, std::vector<ModProfile::Mod> mods)
        : name(name), id(id), description(description), creator(creator), version(version), logo(logo), mods(mods) {}
    ModProfile(std::string name, std::string id, std::string description, std::string creator, std::string version, std::vector<ModProfile::Mod> mods)
        : name(name), id(id), description(description), creator(creator), version(version), mods(mods) {}
};

template <>
struct matjson::Serialize<ModProfile>
{
    static geode::Result<ModProfile> fromJson(matjson::Value const &value)
    {
        std::vector<ModProfile::Mod> mpMods;
        auto mods = value["mods"];
        for (auto& [key, value] : mods) {
            auto type = value["type"].asString().unwrapOrDefault();

            mpMods.push_back(ModProfile::Mod(
                key,
                type == "index" ? ModProfile::Mod::ModType::index : type == "packed" ? ModProfile::Mod::ModType::packed : ModProfile::Mod::ModType::remote,
                value["url"].asString().unwrapOr(""),
                fmt::format("mods/{}.geode", key)
            ));
        }

        return geode::Ok(ModProfile{
            value["name"].asString().unwrapOrDefault(),
            value["id"].asString().unwrapOrDefault(),
            value["description"].asString().unwrapOrDefault(),
            value["creator"].asString().unwrapOrDefault(),
            value["version"].asString().unwrapOrDefault(),
            nullptr,
            mpMods
        });
    }

    static matjson::Value toJson(ModProfile const &value)
    {
        auto obj = matjson::makeObject({
            {"name", value.name},
            {"id", value.id},
            {"description", value.description},
            {"creator", value.creator},
            {"version", value.version},
            {"mods", {}},
        });
        return obj;
    }
};