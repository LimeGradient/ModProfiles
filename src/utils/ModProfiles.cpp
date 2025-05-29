#include "ModProfiles.hpp"
#include "Mods.hpp"

using namespace geode::prelude;

Result<ModProfile> ModProfile::loadFromPath(std::filesystem::path file) {
    auto path = dirs::getTempDir() / GEODE_MOD_ID / "unzip" / file.stem();
    auto res = file::Unzip::intoDir(file, path);
    if (!res.isOk()) {
        log::error("Failed to pick profile: {}", res.err().value());
        return Err(res.err().value());
    }

    auto profileFile = path / "profile.json";
    if (!std::filesystem::exists(profileFile)) {
        log::error("Profile file not found: {}", profileFile.string());
        return Err("Profile file not found");
    }
    auto json = file::readJson(profileFile);
    if (!json.isOk()) {
        log::error("Failed to read profile file: {}", json.err().value());
        return Err(json.err().value());
    }
    auto profile = json.unwrap().as<ModProfile>().unwrapOrDefault();
    auto destination = dirs::getTempDir() / GEODE_MOD_ID / "profiles" / profile.id;
    try {
        std::filesystem::rename(path, destination);
    } catch (const std::filesystem::filesystem_error& e) {
        log::error("Failed to move folder: {}", e.what());
        return Err(e.what());
    }

    auto icon = destination / "icon.png";
    if (std::filesystem::exists(icon)) {
        auto imgfile = file::readBinary(icon);
        if (!imgfile.isOk()) {
            log::error("Failed to read icon file: {}", imgfile.err().value());
            return Err(imgfile.err().value());
        }
        auto data = imgfile.unwrap();

        auto image = new CCImage();
        if (!image->initWithImageData(const_cast<uint8_t *>(data.data()), data.size())) {
            delete image;
        } else {
            image->autorelease();
            profile.logo = image;
        }
    }

    return Ok(profile);
}

std::string ModProfile::Mod::typeToString() {
    switch (this->type) {
        case ModType::remote: return "remote";
        case ModType::index: return "index";
        case ModType::packed: return "packed";
    }
}

ModProfile::Mod::ModType ModProfile::Mod::stringToType(std::string type) {
    if (type == "index") return ModType::index;
    else if (type == "packed") return ModType::packed;
    else if (type == "remote") return ModType::remote;

    return ModType::packed;
}