#include "SettingsPopup.h"

bool SettingsPopup::setup() {
    this->setTitle("Settings");
    m_hasAppliedSettings = false;

    auto exportSettingsMenu = CCMenu::create();
    exportSettingsMenu->setPosition(this->m_title->getPosition() - ccp(0, 115));
    exportSettingsMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(2.5f)
    );
    exportSettingsMenu->setScaledContentSize(this->getScaledContentSize());
    exportSettingsMenu->setID("export-settings-menu");
    exportSettingsMenu->setScale(.5f);
    m_mainLayer->addChild(exportSettingsMenu);

    auto exportSettingsLabel = CCLabelBMFont::create("Export:", "bigFont.fnt");
    exportSettingsLabel->setScale(.5f);
    exportSettingsMenu->addChild(exportSettingsLabel);

    auto includeLocalMods = this->createBoolSetting("Include Local Mods: ", "include-local-mods", exportSettingsMenu);
    includeLocalMods->setPositionX(includeLocalMods->getPositionX() - 50.f);
    m_settingsTogglers.push_back(includeLocalMods);

    auto m_btnMenu = CCMenu::create();
    m_btnMenu->setID("button-menu");

    auto applySpr = ButtonSprite::create("Apply", "bigFont.fnt", "geode-button.png"_spr);
    applySpr->setScale(.65f);
    m_applyBtn = CCMenuItemSpriteExtra::create(
        applySpr, this, menu_selector(SettingsPopup::onApply)
    );
    m_applyBtn->setID("apply-button");
    m_btnMenu->addChild(m_applyBtn);
    m_btnMenu->setLayout(RowLayout::create());
    m_btnMenu->getLayout()->ignoreInvisibleChildren(true);
    m_btnMenu->setPositionY(30.f);
    m_mainLayer->addChild(m_btnMenu);
    return true;
}

CCNode* SettingsPopup::createBoolSetting(std::string title, std::string id, CCMenu* parentMenu) {
    auto menu = CCMenu::create();
    menu->setLayout(
        RowLayout::create()
            ->setAxisReverse(false)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(10)
    );
    menu->setID(id);

    auto label = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
    label->setScale(.5f);
    label->setID("settings-label");
    menu->addChild(label);

    auto toggler = CCMenuItemToggler::createWithStandardSprites(parentMenu, menu_selector(SettingsPopup::onEnable), 1.f);
    toggler->setID("settings-toggler");
    menu->addChild(toggler);

    menu->updateLayout();
    parentMenu->addChild(menu);
    parentMenu->updateLayout();

    return menu;
}

void SettingsPopup::onEnable(CCObject*) {}

void SettingsPopup::onApply(CCObject*) {
    for (CCNode* node : m_settingsTogglers) {
        auto toggler = static_cast<CCMenuItemToggler*>(node->getChildByID("settings-toggler"));
        if (toggler->isToggled()) {
            Mod::get()->setSavedValue<bool>(node->getID(), true);
            log::info("set {} to true", node->getID());
        } else if (!toggler->isToggled()) {
            Mod::get()->setSavedValue<bool>(node->getID(), false);
            log::info("set {} to false", node->getID());
        }
    }
    log::info("Saved all settings to {}", Mod::get()->getSaveDir());
    Notification::create("Saved settings!", NotificationIcon::Success)->show();
    m_hasAppliedSettings = true;
}

void SettingsPopup::onClose(CCObject*) {
    if (!m_hasAppliedSettings) {
        geode::createQuickPopup("Wait!", "You have not applied your settings, do you want to continue?", "Cancel", "Ok", [=, this](auto, bool btn2) {
            if (btn2) {
                this->setKeypadEnabled(false);
                this->setTouchEnabled(false);
                this->removeFromParentAndCleanup(true);
            }
        });
    } else {
        this->setKeypadEnabled(false);
        this->setTouchEnabled(false);
        this->removeFromParentAndCleanup(true);
    }
}

SettingsPopup* SettingsPopup::create() {
    auto ret = new SettingsPopup();
    if (ret && ret->init(POPUP_WIDTH, POPUP_HEIGHT)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}