#include <filesystem>
#include "ModProfilesPopup.h"

#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>

#include <matjson.hpp>

namespace fs = std::filesystem;

void ModProfilesPopup::setup() {
    this->m_filter = {
			"Mod Profile",
			{".modprofile"},
		};
	this->m_options = {
			.filters = {m_filter},
	};

    CCLabelBMFont* title = CCLabelBMFont::create("Mod Profiles", "bigFont.fnt");
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    title->setPosition({winSize.width/2, winSize.height/2+90});

    m_mainLayer->addChild(title);
    
    ButtonSprite* importSprite = ButtonSprite::create("Import Mods", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    ButtonSprite* exportSprite = ButtonSprite::create("Export Mods", 200, true, "goldFont.fnt", "GJ_button_01.png", 30, 1);
    ButtonSprite* restartSprite = ButtonSprite::create("Restart Game", 200, true, "bigFont.fnt", "GJ_button_03.png", 30, 1);

    CCMenuItemSpriteExtra* importButton = CCMenuItemSpriteExtra::create(importSprite, this, menu_selector(ModProfilesPopup::importMods));
    importButton->setPosition({winSize.width/2, winSize.height/2 - 5});

    CCMenuItemSpriteExtra* exportButton = CCMenuItemSpriteExtra::create(exportSprite, this, menu_selector(ModProfilesPopup::exportMods));
    exportButton->setPosition({winSize.width/2, winSize.height/2 - 50});

    CCMenuItemSpriteExtra* restartButton = CCMenuItemSpriteExtra::create(restartSprite, this, menu_selector(ModProfilesPopup::onRestart));
    restartButton->setPosition({winSize.width/2, winSize.height/2 - 95});

    CCMenuItemSpriteExtra* discordButton = CCMenuItemSpriteExtra::create(CCSprite::create("discord-icon.png"_spr), this, menu_selector(ModProfilesPopup::onDiscord));
    discordButton->setPosition({winSize.width / 2, winSize.height/2 + 40});


    m_buttonMenu->addChild(importButton);
    m_buttonMenu->addChild(exportButton);
    m_buttonMenu->addChild(discordButton);
    m_buttonMenu->addChild(restartButton);
    setTouchEnabled(true);
}

void ModProfilesPopup::importMods(CCObject* obj) {
    file::pickFile(file::PickMode::OpenFile, this->m_options, [this](ghc::filesystem::path result) {
        std::vector<std::string> repoLinks;

        auto path = fs::path(result.c_str());
        #ifdef GEODE_IS_WINDOWS
        auto strPath = geode::utils::string::wideToUtf8(result.c_str());
        #else
        std::string strPath = result.c_str();
        #endif

        if (!strPath.ends_with(".modprofile")) {
            FLAlertLayer::create("Mod Profiles", "Failed to import profile\nWrong file type. File should end with .modprofile", "Ok")->show();
            return;
        }

        try {
            fs::copy(strPath, fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()), fs::copy_options::overwrite_existing);
        } catch (fs::filesystem_error& e) {
            FLAlertLayer::create("Mod Profiles", fmt::format("Failed to import profile\nError: {}", e.what()), "Ok")->show();
            return;
        }

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
                        auto downloadLink = json["mod"]["download"].as_string();

                        size_t modName = downloadLink.find_last_of("/");

                        if (!fs::exists(fmt::format("{}/{}", geode::dirs::getModsDir(), downloadLink.substr(modName+1)))) {
                            // web::fetchFile(downloadLink, fmt::format("{}/{}", geode::dirs::getModsDir(), downloadLink.substr(modName+1)));
                            
                            log::info("Mod Name: {} - Mod Link: {}", downloadLink.substr(modName+1), downloadLink);

                            web::AsyncWebRequest()
                            .fetch(downloadLink)
                            .into(fmt::format("{}/{}", geode::dirs::getModsDir(), downloadLink.substr(modName+1)))
                            .then([=](auto value) {
                                
                            })
                            .expect([&](std::string const& error) {
                                Notification::create(fmt::format("Failed to download: {}", downloadLink.substr(modName+1)), NotificationIcon::Error)->show();
                            });

                            Notification::create(fmt::format("Successfully downloaded: {}", downloadLink.substr(modName+1)), NotificationIcon::Success, 1.f)->show();
                        } else {
                            return;
                        }
                    })
                    .expect([=](std::string const& error) {
                        log::error("Link: {}: Error while downloading mods: {}", link, error);
                    });
        }
        fs::remove(fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()));
    }
    }, [=]() {
        FLAlertLayer::create("Mod Profiles", "Failed to import profile\nFile Selection Canceled", "Ok")->show();
        return;
    });
}

void ModProfilesPopup::exportMods(CCObject* obj) {
    file::pickFile(file::PickMode::SaveFile, this->m_options, [](ghc::filesystem::path result) { // I need to find a fix for this before 2.206
        std::vector<std::string> repoLinks;
        std::string profileFilePath;

        auto path = fs::path(result.c_str());
        #ifdef GEODE_IS_WINDOWS
        auto strPath = geode::utils::string::wideToUtf8(result.c_str());
        #else
        std::string strPath = result.c_str();
        #endif

        for (const auto& unzippedMod : fs::directory_iterator(geode::dirs::getModRuntimeDir().c_str())) {
            std::ifstream modJSONFile(fmt::format("{}/mod.json", unzippedMod.path()));
            std::string buffer ((std::istreambuf_iterator<char>(modJSONFile)),
                                (std::istreambuf_iterator<char>()));

            if (buffer == "") continue;

            auto modJSON = matjson::parse(buffer);
            auto modVersion = modJSON["version"].as_string().erase(0, 1); // remove the v from version
            repoLinks.push_back(fmt::format("https://raw.githubusercontent.com/geode-sdk/mods/main/mods-v2/{}/{}/entry.json", modJSON["id"].as_string(), modVersion));
        }

        std::ofstream out;

        out.open(fmt::format("{}_profile.modprofile", strPath), std::ios::trunc);
        
        for (std::string link : repoLinks) {
            out << link << "\n";
        }

        out.close();

        geode::Notification::create("Success! Created Profile!", geode::NotificationIcon::Success)->show();
    }, []() {
        FLAlertLayer::create("Mod Profiles", "Failed to create profile\nFile Operation Canceled", "Ok")->show();
    });
}

void ModProfilesPopup::onDiscord(CCObject* obj) {
    geode::createQuickPopup("Discord", "For support, sharing modpacks, and more join the Discord!", "Cancel", "Ok", [](auto, bool btn2) {
        if (btn2) {
            web::openLinkInBrowser("https://discord.gg/44ANAhXz7r");
        }
    });
}

void ModProfilesPopup::onRestart(CCObject* obj) {
    geode::createQuickPopup("Restart", "Are you sure you want to restart?", "Cancel", "Ok", [](auto, bool btn2) {
        if (btn2) geode::utils::game::restart();
    });
}

ModProfilesPopup* ModProfilesPopup::create() {
    auto ret = new ModProfilesPopup();

    if (ret && ret->init(250, 275, "GJ_square02.png", "")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}