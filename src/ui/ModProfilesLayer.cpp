#include "ModProfilesLayer.hpp"

#include <filesystem>

#include <ui/Cell.hpp>
#include <ui/SwelvyBG.hpp>
#include <utils/ModProfiles.hpp>
#include <utils/Mods.hpp>

#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <UIBuilder.hpp>

using namespace geode::prelude;

bool ModProfilesLayer::init() {
    if (!CCLayer::init()) return false;

    bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

    auto winSize = CCDirector::get()->getWinSize();
    this->setID("ModProfilesLayer"_spr);
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    if (geodeTheme) {
        auto bg = SwelvyBG::create();
        bg->setID("background");
        bg->setAnchorPoint({0.5f, 0.5f});
        this->addChildAtPosition(bg, Anchor::Center, {0, 0}, false);
    } else {
        auto bg = createLayerBG();
        bg->setID("background");
        this->addChild(bg);
    }

    auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentWidth(100.f);
    backMenu->setAnchorPoint({ .0f, .5f });
    
    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(ModProfilesLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);

    backMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(12, -25), false);

    auto actionsMenu = CCMenu::create();
    actionsMenu->setID("actions-menu");
    actionsMenu->setContentHeight(200.f);
    actionsMenu->setAnchorPoint({ .5f, .0f });

    auto rightActionsMenu = CCMenu::create();
    rightActionsMenu->setID("right-actions-menu");
    rightActionsMenu->setContentHeight(200.0f);
    rightActionsMenu->setAnchorPoint({ .5f, .0f });

    m_listFrame = CCNode::create();
    m_listFrame->setID("list-frame");
    m_listFrame->setAnchorPoint({ .5f, .5f });
    m_listFrame->setContentSize({ 380, 205 });

    auto frameBG = CCLayerColor::create(ColorProvider::get()->color("geode.loader/mod-list-bg"));
    frameBG->setID("frame-bg");
    frameBG->setContentSize(m_listFrame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    m_listFrame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName(geodeTheme ? "geode.loader/mods-list-top.png" : "geode.loader/mods-list-top-gd.png");
    tabsTop->setID("frame-top-sprite");
    tabsTop->setAnchorPoint({ .5f, .0f });
    tabsTop->setZOrder(1);
    m_listFrame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName(geodeTheme ? "geode.loader/mods-list-side.png" : "geode.loader/mods-list-side-gd.png");
    tabsLeft->setID("frame-left-sprite");
    tabsLeft->setScaleY(m_listFrame->getContentHeight() / tabsLeft->getContentHeight());
    m_listFrame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6.5f, 1));

    auto tabsRight = CCSprite::createWithSpriteFrameName(geodeTheme ? "geode.loader/mods-list-side.png" : "geode.loader/mods-list-side-gd.png");
    tabsRight->setID("frame-right-sprite");
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(m_listFrame->getContentHeight() / tabsRight->getContentHeight());
    m_listFrame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6.5f, 1));

    auto tabsBottom = CCSprite::createWithSpriteFrameName(geodeTheme ? "geode.loader/mods-list-bottom.png" : "geode.loader/mods-list-bottom-gd.png");
    tabsBottom->setID("frame-bottom-sprite");
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    tabsBottom->setZOrder(1);
    m_listFrame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 3));

    this->addChildAtPosition(m_listFrame, Anchor::Center, ccp(0, -10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setID("tabs-menu");
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(m_listFrame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 8)));
    // Increment touch priority so the mods in the list don't override
    mainTabs->setTouchPriority(-150);

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, const char*, bool>> {
        { "geode.loader/download.png", "My Packs", "my-packs", false },
        { "geode.loader/globe.png", "Download", "download", false },
        { "import.png"_spr, "Import", "import", false },
        { "export.png"_spr, "Export", "export", false },
    }) {
        auto btn = CCMenuItemSpriteExtra::create(
            TabSprite::create(std::get<0>(item), std::get<1>(item), 100, std::get<3>(item)),
            this, menu_selector(ModProfilesLayer::onTab)
        );
        btn->setID(std::get<2>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    goToTab("my-packs");

    return true;
}

ModProfilesLayer *ModProfilesLayer::create() {
    auto ret = new ModProfilesLayer;
    if (!ret->init()) {
        delete ret;
        return nullptr;
    }
    ret->autorelease();
    return ret;
}

CCScene *ModProfilesLayer::scene() {
    auto scene = CCScene::create();
    scene->addChild(ModProfilesLayer::create());
    return scene;
}

void ModProfilesLayer::onBack(CCObject *sender) {
    openModsList();
}

void ModProfilesLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModProfilesLayer::onTab(CCObject *sender) {
    if (auto node = static_cast<CCNode *>(sender)) {
        goToTab(node->getID());
    }
}

void ModProfilesLayer::goToTab(std::string tab) {
    // Update selected tab
    for (auto tabBtn : m_tabs) {
        auto selected = tabBtn->getID() == tab;
        static_cast<TabSprite*>(tabBtn->getNormalImage())->select(selected);
        tabBtn->setEnabled(!selected);
    }

    // Remove current list from UI (it's Ref'd so it stays in memory)
    if (m_scrolls.contains(m_currentTab)) {
        m_scrolls.at(m_currentTab)->removeFromParent();
    }

    // Lazily create new list and add it to UI
    if (!m_scrolls.contains(tab)) {
        auto list = ScrollLayer::create(m_listFrame->getContentSize() - ccp(30, 0));
        list->setPosition(m_listFrame->getContentSize() / 2);
        list->setID(tab);
        list->ignoreAnchorPointForPosition(false);
        list->m_contentLayer->setLayout(ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(this->getScaledContentHeight())
            ->setGrowCrossAxis(false)
            ->setCrossAxisOverflow(true)
            ->setGap(5.f)
        );
        list->m_scrollLimitTop = 5.f;
        m_listFrame->addChild(list);
        if (tab == "import") {
            if (std::filesystem::exists(fmt::format("{}/packs", Mod::get()->getSaveDir()))) {
                for (auto pack : std::filesystem::directory_iterator(fmt::format("{}/packs", Mod::get()->getSaveDir()))) {
                    auto profile = ModProfile::loadFromPath(pack).unwrapOrDefault();
                    auto cell = Cell::create(CellType::PACK, profile, list->getScaledContentWidth());
                    list->m_contentLayer->addChild(cell);
                }
                list->m_contentLayer->updateLayout();
                list->scrollToTop();
            }

            auto btn = Build<ButtonSprite>::create("Import", "bigFont.fnt", "geode.loader/GE_button_05.png", 0.8f)
                .scale(0.75f)
                .intoMenuItem([this]() {
                    file::FilePickOptions options = {
                        std::nullopt,
                        {{.description = "Mod Profiles",
                          .files = {"*.modprofile"}}}};
                
                    this->m_pickListener.bind(this, &ModProfilesLayer::onFileOpen);
                    this->m_pickListener.setFilter(file::pick(file::PickMode::OpenFile, options));
                })
                .intoNewParent(CCMenu::create())
                .collect();

            this->getChildByIDRecursive("frame-bottom-sprite")->addChildAtPosition(btn, Anchor::Center);
        } else if (tab == "export") {
            auto modUtils = modutils::Mod::get();
            for (auto mod : modUtils->getAllMods()) {
                auto cell = Cell::create(CellType::MOD, mod, list->getScaledContentWidth());
                list->m_contentLayer->addChild(cell);
            }
            list->m_contentLayer->updateLayout();
            list->scrollToTop();
        }
        m_scrolls.emplace(tab, list);
    }
    // Add list to UI
    else {
        m_listFrame->addChild(m_scrolls.at(tab));
    }

    // Update current source
    m_currentTab = tab;

    // Update the state of the current list
    // m_lists.at(m_currentSource)->updateDisplay(m_modListDisplay);
    // m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    // m_lists.at(m_currentSource)->updateState();
}

void ModProfilesLayer::onFileOpen(Task<Result<std::filesystem::path>>::Event *event)
{
    if (event->isCancelled())
    {
        return;
    }
    if (auto result = event->getValue())
    {
        if (!result->isOk())
        {
            FLAlertLayer::create(
                "Error",
                fmt::format("Failed to open file. Error: {}", result->err().value()),
                "Ok")
                ->show();
            return;
        }

        if (!std::filesystem::exists(fmt::format("{}/packs", Mod::get()->getSaveDir())))
            std::filesystem::create_directory(fmt::format("{}/packs", Mod::get()->getSaveDir()));
        if (!std::filesystem::exists(fmt::format("{}/packs/{}", Mod::get()->getSaveDir(), result->unwrap().filename()))) {
            std::filesystem::copy_file(result->unwrap(), fmt::format("{}/packs/{}", Mod::get()->getSaveDir(), result->unwrap().filename()));
        }

        auto profile = ModProfile::loadFromPath(result->unwrap());
        if (!profile.isOk())
        {
            FLAlertLayer::create(
                "Error",
                fmt::format("Failed to load profile. Error: {}", profile.err().value()),
                "Ok")
                ->show();
            return;
        }

        auto modProfile = profile.unwrapOrDefault();
        auto list = m_scrolls.at("import");
        if (list->m_contentLayer->getChildByID(modProfile.id)) {
            return;
        } else {
            auto cell = Cell::create(CellType::PACK, modProfile, list->getScaledContentWidth());

            if (auto menu = list->m_contentLayer->getChildByID("select-menu")) {
                list->m_contentLayer->removeChild(menu);
            }
            list->m_contentLayer->addChild(cell);
            list->m_contentLayer->updateLayout();
            list->scrollToTop();
        }
    }
}

void ModProfilesLayer::onExport(CCObject *sender) {
    
}


bool TabSprite::init(const char* iconFrame, const char* text, float width, bool altColor) {
    if (!CCNode::init())
        return false;

    const CCSize itemSize { width, 35 };
    const CCSize iconSize { 18, 18 };

    this->setContentSize(itemSize);
    this->setAnchorPoint({ .5f, .5f });

    m_deselectedBG = CCScale9Sprite::createWithSpriteFrameName("geode.loader/tab-bg.png");
    m_deselectedBG->setScale(.8f);
    m_deselectedBG->setContentSize(itemSize / .8f);
    m_deselectedBG->setColor(to3B(ColorProvider::get()->color("geode.loader/mod-list-tab-deselected-bg")));
    this->addChildAtPosition(m_deselectedBG, Anchor::Center);

    m_selectedBG = CCScale9Sprite::createWithSpriteFrameName("geode.loader/tab-bg.png");
    m_selectedBG->setScale(.8f);
    m_selectedBG->setContentSize(itemSize / .8f);
    m_selectedBG->setColor(to3B(ColorProvider::get()->color(
        altColor ?
            "geode.loader/mod-list-tab-selected-bg-alt" :
            "geode.loader/mod-list-tab-selected-bg"
    )));
    this->addChildAtPosition(m_selectedBG, Anchor::Center);

    m_icon = CCSprite::createWithSpriteFrameName(iconFrame);
    limitNodeSize(m_icon, iconSize, 3.f, .1f);
    this->addChildAtPosition(m_icon, Anchor::Left, ccp(16, 0), false);

    m_label = CCLabelBMFont::create(text, "bigFont.fnt");
    m_label->limitLabelWidth(this->getContentWidth() - 45, clamp(width * .0045f, .35f, .55f), .1f);
    m_label->setAnchorPoint({ .5f, .5f });
    this->addChildAtPosition(m_label, Anchor::Left, ccp((itemSize.width - iconSize.width) / 2 + iconSize.width, 0), false);

    return true;
}

TabSprite* TabSprite::create(const char* iconFrame, const char* text, float width, bool altColor) {
    auto ret = new TabSprite();
    if (ret->init(iconFrame, text, width, altColor)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void TabSprite::select(bool selected) {
    m_deselectedBG->setVisible(!selected);
    m_selectedBG->setVisible(selected);
}

void TabSprite::disable(bool disabled) {
    auto color = disabled ? ccc3(95, 95, 95) : ccc3(255, 255, 255);
    m_deselectedBG->setColor(color);
    m_selectedBG->setColor(color);
    m_icon->setColor(color);
    m_label->setColor(color);
}