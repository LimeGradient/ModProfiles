#include "Cell.hpp"

#include <Geode/loader/ModMetadata.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <utils/ModProfiles.hpp>
#include <utils/Mods.hpp>
#include <UIBuilder.hpp>

// #include "PackCreationPopup.hpp"

using namespace geode::prelude;

bool Cell::init(CellType type, Mod* mod, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_mod = mod;
    this->setID(mod->getID());

    bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(geodeTheme ? 25 : 90)
        .color(geodeTheme ? ccWHITE : ccBLACK)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .height(35.f)
        .parent(this)
        .store(this->m_bg);

    auto infoLayout = ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setGap(0);
    infoLayout->ignoreInvisibleChildren(true);

    Build<CCNode>::create()
        .id("info-container")
        .anchorPoint({.0f, .0f})
        .contentSize(this->m_bg->getScaledContentSize())
        .layout(infoLayout)
        .parent(this)
        .store(this->m_infoContainer);

    auto logo = Build(geode::createModLogo(mod))
        .id("logo")
        .scale(.5f)
        .anchorPoint({.0f, .5f})
        .collect();
    this->m_infoContainer->addChildAtPosition(logo, Anchor::Left, ccp(5.f, 0.f));

    Build<CCNode>::create()
        .id("title-container")
        .anchorPoint({.0f, .5f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(false)
        )
        .width(width)
        .store(this->m_titleContainer);
    this->m_infoContainer->addChildAtPosition(this->m_titleContainer, Anchor::TopLeft, ccp(35.f, -10.f));

    Build(CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt"))
        .id("title-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.5f})
        .parent(this->m_titleContainer)
        .store(this->m_titleLabel);

    auto versionLabel = Build(CCLabelBMFont::create(fmt::format("v{}", mod->getVersion().toNonVString()).c_str(), "bigFont.fnt"))
        .id("version-label")
        .scale(0.2)
        .anchorPoint({0.f, 0.575f})
        .color(ccc3(0, 255, 0))
        .parent(this->m_titleContainer)
        .collect();
    m_titleContainer->updateLayout();
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .layout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(this->m_developerMenu);
    
    auto devLabel = Build(CCLabelBMFont::create(ModMetadata::formatDeveloperDisplayString(mod->getDevelopers()).c_str(), "goldFont.fnt"))
        .id("developer-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.35f})
        .collect();
    this->m_infoContainer->addChildAtPosition(devLabel, Anchor::BottomLeft, ccp(35.f, 10.f));

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(10);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({1.f, .5f})
        .contentSize({50.f, 25.f})
        .layout(viewLayout)
        .scale(.55f)
        .store(this->m_viewMenu);
    
    Build(CCMenuItemToggler::createWithStandardSprites(this, menu_selector(Cell::onModEnable), 1.f))
        .id("toggler")
        .parent(m_viewMenu)
        .store(m_toggler);

    m_viewMenu->updateLayout();
    this->m_infoContainer->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-7.5f, 0.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

bool Cell::init(CellType type, ModProfile profile, float width) {
    if (!CCNode::init()) return false;

    this->m_type = type;
    this->m_profile = profile;
    this->setID(profile.id);

    bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id("cell-bg")
        .opacity(geodeTheme ? 25 : 90)
        .color(geodeTheme ? ccWHITE : ccBLACK)
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .0f})
        .width(width)
        .height(35.f)
        .parent(this)
        .store(this->m_bg);

    auto sprite = LazySprite::create({25, 25}, true);
    sprite->setAutoResize(true);
    sprite->loadFromFile(dirs::getTempDir() / GEODE_MOD_ID / "profiles" / profile.id / "logo.png");
    
    auto logo = Build(sprite)
        .id("logo")
        .anchorPoint({.5f, .5f})
        .collect();
    // logo->setScale(std::min(25 / logo->getContentSize().width, 25 / logo->getContentSize().height));
    this->addChildAtPosition(logo, Anchor::Left, ccp(17.5, 17.5));

    auto infoLayout = ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setGap(0);
    infoLayout->ignoreInvisibleChildren(true);

    Build<CCNode>::create()
        .id("info-container")
        .anchorPoint({.0f, .0f})
        .contentSize(this->m_bg->getScaledContentSize())
        .layout(infoLayout)
        .parent(this)
        .store(this->m_infoContainer);

    Build<CCNode>::create()
        .id("title-container")
        .anchorPoint({.0f, .5f})
        .layout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, 1.f)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setAutoScale(false)
        )
        .width(width)
        .store(this->m_titleContainer);
    this->m_infoContainer->addChildAtPosition(this->m_titleContainer, Anchor::TopLeft, ccp(35.f, -10.f));

    Build(CCLabelBMFont::create(profile.name.c_str(), "bigFont.fnt"))
        .id("title-label")
        .scale(0.35f)
        .anchorPoint(0.f, 0.5f)
        .parent(this->m_titleContainer)
        .store(this->m_titleLabel);
    
    Build(CCLabelBMFont::create(fmt::format("v{}", profile.version).c_str(), "bigFont.fnt"))
        .id("version-label")
        .scale(0.2)
        .anchorPoint({0.f, 0.575f})
        .color(ccc3(0, 255, 0))
        .parent(this->m_titleContainer)
        .collect();
    this->m_titleContainer->updateLayout();
    
    Build<CCMenu>::create()
        .id("developers-menu")
        .ignoreAnchorPointForPos(false)
        .anchorPoint({.0f, .5f})
        .layout(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(this->m_developerMenu);
    
    auto devLabel = Build(CCLabelBMFont::create(profile.creator.c_str(), "goldFont.fnt"))
        .id("developer-label")
        .scale(0.35f)
        .anchorPoint({0.f, 0.35f})
        .collect();
    this->m_infoContainer->addChildAtPosition(devLabel, Anchor::BottomLeft, ccp(35.f, 10.f));

    auto viewLayout = RowLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setGap(2.5)
        ->setAutoGrowAxis(true);
    viewLayout->ignoreInvisibleChildren(true);

    Build<CCMenu>::create()
        .id("view-menu")
        .anchorPoint({1.f, .5f})
        .contentSize({50.f, 25.f})
        .layout(viewLayout)
        .scale(.55f)
        .store(this->m_viewMenu);
    
    Build(CCMenuItemToggler::createWithStandardSprites(this, menu_selector(Cell::onPackEnable), 1.f))
        .id("toggler")
        .parent(m_viewMenu)
        .store(m_toggler);

    // Build(CCMenuItemSpriteExtra::create(EditorButtonSprite::createWithSpriteFrameName("edit.png"_spr, 1, EditorBaseColor::Gray), this, menu_selector(Cell::onPackEdit)))
    //     .id("edit")
    //     .parent(m_viewMenu);

    m_viewMenu->updateLayout();
    this->m_infoContainer->addChildAtPosition(this->m_viewMenu, Anchor::Right, ccp(-7.5f, 0.f));
    this->setScaledContentSize(this->m_bg->getScaledContentSize());

    return true;
}

void Cell::onModEnable(CCObject*) {

}

void Cell::onPackEnable(CCObject*) {
        geode::createQuickPopup(
            "Select pack?",
            "Selecting this pack will <cr>uninstall</c> all of your current mods and <cy>restart the game</c>. Are you sure you want to select this pack?",
            "Cancel", 
            "Ok",
            [this](auto, bool btn2) {
            if (btn2) {
                auto importPack = modutils::Mod::get()->importPack(this->m_profile);
                if (!importPack) {
                FLAlertLayer::create(
                    "<cr>Pack Selection Error!</c>",
                    fmt::format("Error selecting this pack: <cr>{}</c>", importPack.unwrapErr()),
                    "<cg>Ok</c>"
                )->show();
                } else {
                importPack.unwrap();
                game::restart();
                }
            }
            }
        );
}

void Cell::onPackEdit(CCObject*) {

}

Cell* Cell::create(CellType type, Mod* mod, float width) {
    auto ret = new Cell();
    if (ret->init(type, mod, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

Cell* Cell::create(CellType type, ModProfile profile, float width) {
    auto ret = new Cell();
    if (ret->init(type, profile, width)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}