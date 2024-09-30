#include "PackSelectList.h"

using namespace geode::prelude;

bool PackSelectList::init(CCSize const& size) {
    if (!CCNode::init()) return false;

    this->setContentSize(size);
    this->setAnchorPoint({.5f, .5f});
    this->setID("pack-select-list");

    m_list = ScrollLayer::create(size);
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(2.5f)
    );
    m_list->scrollToTop();
    m_list->m_scrollLimitTop = 5.f;
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0));

    m_topContainer = CCNode::create();
    m_topContainer->setID("top-container");
    m_topContainer->ignoreAnchorPointForPosition(false);
    m_topContainer->setContentWidth(size.width);
    m_topContainer->setAnchorPoint({ .5f, 1.f });

    m_btnMenu = CCMenu::create();
    m_btnMenu->setID("button-menu");

    m_importPackBtnSpr = ButtonSprite::create("Import Pack", "bigFont.fnt", "geode-button.png"_spr);
    m_importPackBtnSpr->setScale(0.65f);
    m_importPackBtn = CCMenuItemSpriteExtra::create(
        m_importPackBtnSpr, this, menu_selector(PackSelectList::onImportPack)
    );
    m_btnMenu->addChild(m_importPackBtn);
    m_btnMenu->setLayout(RowLayout::create());
    m_btnMenu->getLayout()->ignoreInvisibleChildren(true);

    auto restartBtnSpr = ButtonSprite::create("Restart Game", "bigFont.fnt", "geode-button.png"_spr);
    restartBtnSpr->setScale(0.65f);
    m_restartBtn = CCMenuItemSpriteExtra::create(
        restartBtnSpr, this, menu_selector(PackSelectList::onRestartGame)
    );
    m_restartBtn->setVisible(false);
    m_restartBtn->setPosition(m_importPackBtn->getPosition());
    m_btnMenu->addChild(m_restartBtn);
    m_btnMenu->updateLayout();
    
    this->addChildAtPosition(m_btnMenu, Anchor::Bottom, ccp(0, -13.f));

    this->getAllPacks();

    return true;
}

void PackSelectList::getAllPacks() {
    float totalHeight = .0f;
    Zip* zip = new Zip("");
    if (!fs::exists(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
        fs::create_directory(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()));
    }
    for (auto file : fs::directory_iterator(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
        if (file.is_directory() || file.path().extension().string() != ".geodepack") continue;

        PackInfo* packInfo = new PackInfo();
        auto unzippedDir = fmt::format("{}/geodepacks/{}", geode::dirs::getGeodeDir(), file.path().stem().string());

        std::ifstream packJsonFile(fmt::format("{}/pack.json", unzippedDir));
        std::stringstream buffer;
        buffer << packJsonFile.rdbuf();

        auto packJson = matjson::parse(buffer.str());
        packInfo->setPackInfo(
            packJson["title"].as_string(),
            packJson["description"].as_string(),
            packJson["author"].as_string(),
            packJson["hasLocalMods"].as_bool()
        );
        if (fs::exists(fmt::format("{}/logo.png", unzippedDir).c_str())) {
            packInfo->logo = fmt::format("{}/logo.png", unzippedDir);
        } else {
            packInfo->logo = "";
        }

        auto packCell = PackCell::create(packInfo);
        packCell->setID(file.path().stem().string());
        packCell->setContentWidth(this->getScaledContentWidth());
        packCell->m_bg->setContentWidth(this->getScaledContentWidth() + 150.f);
        packCell->m_infoContainer->setPositionX(packCell->m_bg->getScaledContentWidth() / 2);
        packCell->m_viewMenu->setPositionX(packCell->m_bg->getScaledContentWidth() - 50.f);
        packCell->m_enableToggle->setPositionX(630.f);
        totalHeight += packCell->getScaledContentHeight();
        packCell->setPosition(115.f, totalHeight);
        m_list->m_contentLayer->addChild(packCell);
        m_list->m_contentLayer->updateLayout();
        m_list->updateLayout();
    }
}

void PackSelectList::packSelect(PackInfo* packInfo) {
    for (auto file : fs::directory_iterator(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
        if (!file.is_directory()) continue;

        auto unzippedDir = fmt::format("{}/geodepacks/{}", geode::dirs::getGeodeDir(), file.path().stem().string());
        std::ifstream packJsonFile(fmt::format("{}/pack.json", unzippedDir));
        std::stringstream buffer;
        buffer << packJsonFile.rdbuf();
        auto packJsonStr = buffer.str();

        auto packJson = matjson::parse(packJsonStr);
        if (packJson["title"].as_string() == packInfo->title) {
            if (packJson["hasLocalMods"].as_bool() == true) {
                FLAlertLayer::create(
                    "Mod Profiles",
                    "<cr>Warning!</c> This pack may contain mods in development! Beware!",
                    "Ok"
                )->show();
            }
            this->m_selectedPackPath = unzippedDir;
        }
    }

    m_importPackBtn->setVisible(false);
    m_restartBtn->setVisible(true);
}

void PackSelectList::packDeselect() {
    m_importPackBtn->setVisible(true);
    m_restartBtn->setVisible(false);
}

void PackSelectList::onImportPack(CCObject*) {

}

void PackSelectList::onRestartGame(CCObject*) {
    createQuickPopup("Restart", "Are you sure you want to restart?", "Cancel", "Ok", [=, this](auto, bool btn2) {
        if (btn2) {
            std::ifstream packJsonFile(fmt::format("{}/pack.json", this->m_selectedPackPath));
            std::stringstream buffer;
            buffer << packJsonFile.rdbuf();
            auto packJsonStr = buffer.str();

            auto packJson = matjson::parse(packJsonStr);
            if (packJson["hasLocalMods"].as_bool()) {
                auto modsDir = fmt::format("{}/mods", this->m_selectedPackPath);
                for (auto file : fs::directory_iterator(geode::dirs::getModsDir())) {
                    fs::remove_all(file.path());
                }
                
                for (auto file : fs::directory_iterator(modsDir)) {
                    if (ModUtils::isModCompatible(file.path())) {
                        fs::copy(file.path(), geode::dirs::getModsDir());
                    } else {
                        log::info("Mod not compatible: {}", file.path().stem().string());
                    }
                }
            }
            geode::utils::game::restart();
        }
    });
}

PackSelectList* PackSelectList::create(CCSize const& size) {
    auto ret = new PackSelectList();
    if (ret && ret->init(size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}