#include "ModpackInfoPopup.h"
#include "lists/ExportProfilesList.h"

file::FilePickOptions options = {
    std::nullopt,
    {
        {
            "Mod Profile",
            {".modprofile"}
        },
    }
};

FileTask exportToFile() {
    return file::pick(file::PickMode::SaveFile, options);
}

FileTask importLogo() {
    return file::pick(file::PickMode::OpenFile, {
        std::nullopt,
        {
            {
                "Pictures",
                {".jpg", ".png"}
            },
        }
    });
}

bool ModpackInfoPopup::setup() {
    this->setTitle("Modpack Info");
    this->setID("modpack-info-popup");

    auto menu = CCMenu::create();
    menu->setID("modpack-info-menu");
    menu->setPosition(this->m_title->getPosition() - ccp(0, 115));

    m_modpackTitle = TextInput::create(150.f, "Title");
    m_modpackTitle->setMaxCharCount(45);
    m_modpackTitle->setID("modpack-title-input");
    menu->addChildAtPosition(m_modpackTitle, Anchor::Top, ccp(0.f, -100.f));

    m_modpackDescription = TextInput::create(250.f, "Description");
    m_modpackDescription->setMaxCharCount(60);
    m_modpackDescription->setID("modpack-desc-input");
    menu->addChildAtPosition(m_modpackDescription, Anchor::Top, ccp(0.f, -140.f));

    m_modpackAuthor = TextInput::create(150.f, "Author");
    m_modpackAuthor->setID("modpack-author-input");
    menu->addChildAtPosition(m_modpackAuthor, Anchor::Center, ccp(0.f, -20.f));

    auto createPackSpr = ButtonSprite::create("Create Pack", "bigFont.fnt", "geode-button.png"_spr);
    createPackSpr->setScale(0.65f);
    m_createPackBtn = CCMenuItemSpriteExtra::create(
        createPackSpr, this, menu_selector(ModpackInfoPopup::onCreatePack)
    );
    m_createPackBtn->setID("create-pack-button");
    menu->addChildAtPosition(m_createPackBtn, Anchor::Bottom, ccp(0.f, 15.f));

    this->m_mainLayer->addChild(menu);
}

void ModpackInfoPopup::getLogo(FileTask::Event* e) {
    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();
        }
    } else if (e->isCancelled()) {
        geode::Notification::create("File Operation Cancelled", geode::NotificationIcon::Error)->show();
    }
}

void ModpackInfoPopup::onChooseLogo(CCObject*) {
    EventListener<FileTask> logoListener;
    logoListener.bind([=] (auto* e) {
        this->getLogo(e);
    });
    logoListener.setFilter(importLogo());
}

void ModpackInfoPopup::onCreatePack(CCObject* sender) {
    auto exportProfilesList = static_cast<ExportProfilesList*>(CCScene::get()->getChildByIDRecursive("ExportProfilesList"));
        m_fileTaskListener.bind([=] (auto* e) {
        if (Mod::get()->getSavedValue<bool>("include-local-mods")) {
            exportProfilesList->exportProfileWithLocalMods(e);
        } else {
            exportProfilesList->exportProfile(e);
        }
    });
    m_fileTaskListener.setFilter(exportToFile());
}

ModpackInfoPopup* ModpackInfoPopup::create() {
    auto ret = new ModpackInfoPopup();
    if (ret->init(POPUP_WIDTH, POPUP_HEIGHT, "GJ_square05.png")) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}