#include <filesystem>
#include "ModProfilesPopup.h"

#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>

#include <matjson.hpp>

namespace fs = std::filesystem;

void ModProfilesPopup::setup() {
    this->m_filter = {
			"Mod Profile",
			{".profile"},
		};
	this->m_options = {
			.filters = {m_filter},
	};

    CCLabelBMFont* title = CCLabelBMFont::create("Mod Profiles", "bigFont.fnt");
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    title->setPosition({winSize.width/2, winSize.height/2+70});

    m_mainLayer->addChild(title);
    
    ButtonSprite* importSprite = ButtonSprite::create("Import Mods", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    ButtonSprite* exportSprite = ButtonSprite::create("Export Mods", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);

    CCMenuItemSpriteExtra* importButton = CCMenuItemSpriteExtra::create(importSprite, this, menu_selector(ModProfilesPopup::importMods));
    importButton->setPosition({winSize.width/2, winSize.height/2 - 25});

    CCMenuItemSpriteExtra* exportButton = CCMenuItemSpriteExtra::create(exportSprite, this, menu_selector(ModProfilesPopup::exportMods));
    exportButton->setPosition({winSize.width/2, winSize.height/2 - 70});

    m_buttonMenu->addChild(importButton);
    m_buttonMenu->addChild(exportButton);
    setTouchEnabled(true);
}

void ModProfilesPopup::importMods(CCObject* obj) {
    std::vector<std::string> repoLinks;

    file::pickFile(file::PickMode::OpenFile, this->m_options, [&](ghc::filesystem::path result) {
        auto path = fs::path(result.c_str());
        #ifdef GEODE_IS_WINDOWS
        auto strPath = geode::utils::string::wideToUtf8(result.c_str());
        #else
        std::string strPath = result.c_str();
        #endif

        if (!strPath.ends_with(".profile")) {
            FLAlertLayer::create("Mod Profiles", "Failed to import profile\nWrong file type. File should end with .profile", "Ok")->show();
            return;
        }

        try {
            fs::rename(strPath, fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()));
        } catch (fs::filesystem_error& e) {
            FLAlertLayer::create("Mod Profiles", fmt::format("Failed to import profile\nError: {}", e.what()), "Ok")->show();
            return;
        }
    }, [=]() {
        FLAlertLayer::create("Mod Profiles", "Failed to import profile\nFile Selection Canceled", "Ok")->show();
        return;
    });

    if (fs::exists(fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()))) {
        std::ifstream profile(fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()));
        std::string line;
        if (profile.is_open()) {
            while (std::getline(profile, line)) {
                repoLinks.push_back(line);
            }
            profile.close();
        } else {
            log::info("Unable to open Profile");
            return;
        }
        
        for (std::string link : repoLinks) {
            web::AsyncWebRequest()
                .userAgent("ModProfiles")
                .fetch(link)
                .text()
                .then([](std::string const& value) {
                    auto json = matjson::parse(value);
                    size_t modName = json["mod"]["download"].as_string().find_last_of("/");

                    if (!fs::exists(fmt::format("{}/{}", geode::dirs::getModsDir(), json["mod"]["download"].as_string().substr(modName+1)))) {
                        web::fetchFile(json["mod"]["download"].as_string(), fmt::format("{}/{}", geode::dirs::getModsDir(), json["mod"]["download"].as_string().substr(modName+1)));
                        Notification::create(fmt::format("Successfully downloaded: {}", json["mod"]["download"].as_string().substr(modName+1)), NotificationIcon::Success, 1.f)->show();
                    } else {
                        return;
                    }
                })
                .expect([](std::string const& error) {
                    log::error("Error while downloading mods: {}", error);
                });
        }
    }
}

void ModProfilesPopup::exportMods(CCObject* obj) {
    std::vector<std::string> repoLinks;
    std::string profileFilePath;

    for (const auto& unzippedMod : fs::directory_iterator(geode::dirs::getModRuntimeDir().c_str())) {
        std::ifstream modJSONFile(fmt::format("{}/mod.json", unzippedMod.path()));
        std::string buffer ((std::istreambuf_iterator<char>(modJSONFile)),
                            (std::istreambuf_iterator<char>()));

        if (buffer == "") continue;

        auto modJSON = matjson::parse(buffer);
        auto modVersion = modJSON["version"].as_string().erase(0, 1); // remove the v from version
        repoLinks.push_back(fmt::format("https://raw.githubusercontent.com/geode-sdk/mods/main/mods-v2/{}/{}/entry.json", modJSON["id"].as_string(), modVersion));
    }

    std::ofstream out;  // Clear the file first
    out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ofstream::out | std::ofstream::trunc);
    out.close();

    out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ios::app);
    
    for (std::string link : repoLinks) {
        out << link << "\n";
    }

    out.close();

    file::pickFile(file::PickMode::SaveFile, this->m_options, [](ghc::filesystem::path result) {
        auto path = fs::path(result.c_str());
        #ifdef GEODE_IS_WINDOWS
        auto strPath = geode::utils::string::wideToUtf8(result.c_str());
        #else
        std::string strPath = result.c_str();
        #endif

        try {
            fs::rename(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), fmt::format("{}.profile", strPath));
        } catch (fs::filesystem_error& e) {
            FLAlertLayer::create("Mod Profiles", fmt::format("Failed to create profile\nError: {}", e.what()), "Ok")->show();
        }

        geode::Notification::create("Success! Created Profile!", geode::NotificationIcon::Success);
    }, []() {
        FLAlertLayer::create("Mod Profiles", "Failed to create profile\nError while moving file.", "Ok")->show();
    });
}

ModProfilesPopup* ModProfilesPopup::create() {
    auto ret = new ModProfilesPopup();

    if (ret && ret->init(250, 200, "GJ_square02.png", "")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}