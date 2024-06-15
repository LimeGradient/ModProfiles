#include <filesystem>
#include "ModProfilesPopup.h"

#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>

#include <matjson.hpp>

namespace fs = std::filesystem;

using FileTask = Task<geode::Result<fs::path>>;

const file::FilePickOptions ModProfilesPopup::m_options = {
    std::nullopt,
    {
        {
            "Mod Profiles",
            {".modprofile"}
        }
    }
};

EventListener<FileTask> m_fileTaskListener;
EventListener<web::WebTask> m_webTaskListener;
EventListener<web::WebTask> m_downloadFileListener;

void ModProfilesPopup::setup() {
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

FileTask importFromFile() {
    return file::pick(file::PickMode::OpenFile, ModProfilesPopup::m_options);
}

void downloadMods(FileTask::Event* e) {
    std::vector<std::string> downloadLinks;

    m_webTaskListener.bind([=] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            if (res->ok()) {
                auto modJSON = res->json();
                log::info("holy shit dfoewsnkoad link: {}", modJSON.unwrap()["payload"]["versions"].as_array().back()["download_link"].as_string());
                auto link = modJSON.unwrap()["payload"]["versions"].as_array().back()["download_link"].as_string();
                auto modID = modJSON.unwrap()["payload"]["versions"].as_array().back()["mod_id"].as_string();

                m_downloadFileListener.bind([=] (web::WebTask::Event* ev) {
                    if (web::WebResponse* response = e->getValue()) {
                        if (response->ok()) {
                            if (!fs::exists(fmt::format("{}/{}", geode::dirs::getModsDir(), modID + ".geode"))) {
                                response->into(fmt::format("{}/{}", geode::dirs::getModsDir(), modID + ".geode"));
                                Notification::create(fmt::format("Successfully installed: {}", modID), NotificationIcon::Success);
                            }
                        }
                    } else if (e->isCancelled()) {
                        Notification::create(fmt::format("Error while downloading mod: {}", modID), NotificationIcon::Error);
                    }
                });
                auto fileReq = web::WebRequest();
                m_downloadFileListener.setFilter(fileReq.get(link));
            }
        } else if (e->isCancelled()) {
            log::info("Web requestist went fucvkyt wucjky");
        }
    });

    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();
            std::ifstream modProfileFile(path);
            std::string line;
            while (std::getline(modProfileFile, line)) {
                downloadLinks.push_back(line);
            }

            for (std::string downloadLink : downloadLinks) {
                auto req = web::WebRequest();
                m_webTaskListener.setFilter(req.get(downloadLink));
            }
        }
    }
}

void ModProfilesPopup::importMods(CCObject* obj) {
    m_fileTaskListener.bind([=](auto* e) {
        downloadMods(e);
    });
    m_fileTaskListener.setFilter(importFromFile());
}

FileTask exportToFile() {
    return file::pick(file::PickMode::SaveFile, ModProfilesPopup::m_options);
}

void exportFilePick(FileTask::Event* e) {
    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();

            std::vector<std::string> repoLinks;
            for (const auto& unzippedMod : fs::directory_iterator(geode::dirs::getModRuntimeDir().c_str())) {

                std::ifstream modJSONFile(fmt::format("{}/mod.json", unzippedMod.path()));
                std::string buffer ((std::istreambuf_iterator<char>(modJSONFile)),
                                    (std::istreambuf_iterator<char>()));

                if (buffer == "") continue;

                auto modJSON = matjson::parse(buffer);
                repoLinks.push_back(fmt::format("https://api.geode-sdk.org/v1/mods/{}", modJSON["id"].as_string()));

                std::ofstream out;

                out.open(fmt::format("{}_profile.modprofile", path), std::ios::trunc);
                
                for (std::string link : repoLinks) {
                    out << link << "\n";
                }

                out.close();
            }
            geode::Notification::create("Success! Created Profile!", geode::NotificationIcon::Success)->show();
        }
    } else if (e->isCancelled()) {
        FLAlertLayer::create("Mod Profiles", "Failed to create profile\nFile Operation Canceled", "Ok")->show();
    }
}

void ModProfilesPopup::exportMods(CCObject* obj) {
    m_fileTaskListener.bind([=](auto* e) {
        exportFilePick(e);
    });
    m_fileTaskListener.setFilter(exportToFile());
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