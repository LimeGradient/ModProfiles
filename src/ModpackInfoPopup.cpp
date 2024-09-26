#include "ModpackInfoPopup.h"
#include "lists/ExportProfilesList.h"

EventListener<FileTask> logoListener;

file::FilePickOptions options = {
    std::nullopt,
    {
        {
            "Mod Profile",
            {".geodepack"}
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
                {"*.jpg", "*.png"}
            },
        }
    });
}

bool ModpackInfoPopup::setup() {
    this->setTitle("Modpack Info");
    this->setID("modpack-info-popup");

    m_packInfo = new PackInfo();

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

    auto selectLogoBtnSpr = ButtonSprite::create("Choose Logo", "bigFont.fnt", "geode-button.png"_spr);
    selectLogoBtnSpr->setScale(0.65f);
    m_chooseLogoBtn = CCMenuItemSpriteExtra::create(
        selectLogoBtnSpr, this, menu_selector(ModpackInfoPopup::onChooseLogo)
    );
    m_chooseLogoBtn->setID("choose-logo-button");
    menu->addChildAtPosition(m_chooseLogoBtn, Anchor::Center, ccp(0.f, -60.f));

    m_logoPathText = CCLabelBMFont::create("Logo Path:", "bigFont.fnt");
    m_logoPathText->setScale(0.25f);
    m_logoPathText->setID("logo-path-text");
    menu->addChildAtPosition(m_logoPathText, Anchor::Center, ccp(0.f, -100.f));

    auto createPackSpr = ButtonSprite::create("Create Pack", "bigFont.fnt", "geode-button.png"_spr);
    createPackSpr->setScale(0.65f);
    m_createPackBtn = CCMenuItemSpriteExtra::create(
        createPackSpr, this, menu_selector(ModpackInfoPopup::onCreatePack)
    );
    m_createPackBtn->setID("create-pack-button");
    menu->addChildAtPosition(m_createPackBtn, Anchor::Bottom, ccp(0.f, 15.f));

    this->m_mainLayer->addChild(menu);

    return true;
}

void ModpackInfoPopup::getLogo(FileTask::Event* e) {
    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();
            m_packInfo->logo = path.string();
            m_logoPathText->setString(fmt::format("Logo Path: {}", path.string()).c_str());
        }
    } else if (e->isCancelled()) {
        geode::Notification::create("File Operation Cancelled", geode::NotificationIcon::Error)->show();
    }
}

void ModpackInfoPopup::onChooseLogo(CCObject*) {
    logoListener.bind([=] (auto* e) {
        this->getLogo(e);
    });
    logoListener.setFilter(importLogo());
}

void ModpackInfoPopup::onCreatePack(CCObject* sender) {
    auto exportProfilesList = static_cast<ExportProfilesList*>(CCScene::get()->getChildByIDRecursive("export-pack-list"));
    m_fileTaskListener.bind([=] (auto* e) {
        m_packInfo->setPackInfo(
            m_modpackTitle->getString(), 
            m_modpackDescription->getString(), 
            m_modpackAuthor->getString(), 
            Mod::get()->getSavedValue<bool>("include-local-mods")
        );
        exportProfilesList->exportProfile(e, m_packInfo);
    });
    m_fileTaskListener.setFilter(exportToFile());
    this->onClose(new CCObject());
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