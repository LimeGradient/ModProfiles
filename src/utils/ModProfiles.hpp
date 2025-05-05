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
        return geode::Ok(ModProfile{
            value["name"].asString().unwrapOrDefault(),
            value["id"].asString().unwrapOrDefault(),
            value["description"].asString().unwrapOrDefault(),
            value["creator"].asString().unwrapOrDefault(),
            value["version"].asString().unwrapOrDefault(),
            nullptr,
            {}
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