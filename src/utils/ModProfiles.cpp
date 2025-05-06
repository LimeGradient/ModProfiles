#include "ModProfiles.hpp"

using namespace geode::prelude;

Result<ModProfile> ModProfile::loadFromPath(std::filesystem::path file) {
    auto path = dirs::getTempDir() / "profiles" / file.stem();
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

    auto icon = path / "icon.png";
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