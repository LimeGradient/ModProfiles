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
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0));

    m_topContainer = CCNode::create();
    m_topContainer->setID("top-container");
    m_topContainer->ignoreAnchorPointForPosition(false);
    m_topContainer->setContentWidth(size.width);
    m_topContainer->setAnchorPoint({ .5f, 1.f });

    float totalHeight = .0f;

    m_btnMenu = CCMenu::create();
    m_btnMenu->setID("button-menu");

    auto importPackBtnSpr = ButtonSprite::create("Import Pack", "bigFont.fnt", "geode-button.png"_spr);
    importPackBtnSpr->setScale(0.65f);
    m_importPackBtn = CCMenuItemSpriteExtra::create(
        importPackBtnSpr, this, menu_selector(PackSelectList::onImportPack)
    );
    m_btnMenu->addChild(m_importPackBtn);
    m_btnMenu->setLayout(RowLayout::create());
    m_btnMenu->getLayout()->ignoreInvisibleChildren(true);
    
    this->addChildAtPosition(m_btnMenu, Anchor::Bottom, ccp(0, -13.f));

    this->getAllPacks();

    return true;
}

void PackSelectList::getAllPacks() {
    Zip* zip = new Zip("");
    if (!fs::exists(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
        fs::create_directory(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()));
    }
    for (auto file : fs::directory_iterator(fmt::format("{}/geodepacks", geode::dirs::getGeodeDir()))) {
        zip->unzipIntoFolder(file.path().string(), fmt::format("{}/geodepacks/{}", geode::dirs::getGeodeDir(), file.path().stem().string()));
    }
}

void PackSelectList::onImportPack(CCObject*) {

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