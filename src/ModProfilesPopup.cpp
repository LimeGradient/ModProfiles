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
            link.erase(link.begin(), std::next(link.begin(), 19));
            std::string apiLink = fmt::format("https://api.github.com/repos/{}/releases/latest", link);
            web::AsyncWebRequest()
                .userAgent("ModProfiles")
                .fetch(apiLink)
                .text()
                .then([](std::string const& value) {
                    auto json = matjson::parse(value);
                    log::info("Latest Release Download URL: {}", json["assets"][0]["browser_download_url"].as_string());

                    auto modFilePath = fmt::format("{}/{}", geode::dirs::getModsDir(), json["assets"][0]["name"].as_string());
                    
                    if (modFilePath.find(".geode") == std::string::npos) {
                        return;
                    }

                    if (!fs::exists(modFilePath)) {
                        web::fetchFile(json["assets"][0]["browser_download_url"].as_string(), modFilePath);
                        Notification::create(fmt::format("Successfully downloaded: {}", json["assets"][0]["name"].as_string()), NotificationIcon::Success, 1.f)->show();
                    } else {
                        return;
                    }
                })
                .expect([](std::string const& error) {
                    auto errorJSON = matjson::parse(error);
                    if (errorJSON["message"].as_string() == "Not Found") return;

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
        if (modJSON.contains("repository")) {
            log::info("{}", modJSON["repository"].as_string());
            repoLinks.push_back(modJSON["repository"].as_string());
        } else {
            continue;
        }
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