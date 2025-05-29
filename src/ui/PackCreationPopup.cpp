#include "PackCreationPopup.hpp"

#include <UIBuilder.hpp>

#include <regex>
#include <utils/Mods.hpp>

using namespace geode::prelude;

bool PackCreationPopup::setup(std::vector<geode::Mod*> const& mods) {
    this->setTitle("Create Pack");

    this->mods = mods;
    for (auto mod : mods) {
        modutils::Mod::get()->isIndexMod(mod, [mod, this](ModProfile::Mod::ModType type) mutable {
            switch (type) {
                case ModProfile::Mod::ModType::index: {
                    modProfileMods.push_back(ModProfile::Mod(
                        mod->getID(), ModProfile::Mod::ModType::index
                    ));
                    break;
                }

                case ModProfile::Mod::ModType::packed: {
                    if (mod->getID() == "geode.loader") break;

                    modProfileMods.push_back(ModProfile::Mod(
                        mod->getID(), ModProfile::Mod::ModType::packed, "", mod->getPackagePath().string()
                    ));
                    break;
                }

                case ModProfile::Mod::ModType::remote: {

                }
            }
        });
    }

    auto menuLayout = ColumnLayout::create()
        ->setDefaultScaleLimits(.1f, 1.f)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAxisReverse(true)
        ->setAutoScale(false);
    auto leftMenu = Build<CCMenu>::create()
                .layout(menuLayout)
                .scale(0.8f)
                .anchorPoint({.5f, .0f})
                .collect();

    auto createComponent = [leftMenu, this](std::string label, CCPoint offset, TextInput*& store, CommonFilter filter = CommonFilter::Any, std::function<void(std::string const&)> func = nullptr) {
        auto labelNode = Build(CCLabelBMFont::create(label.c_str(), "bigFont.fnt"))
            .scale(0.65f)
            .parent(leftMenu)
            .collect();

        auto inputNode = Build<TextInput>::create(200.f, label.c_str(), "chatFont.fnt")
            .store(store)
            .parent(leftMenu)
            .collect();

        inputNode->setCommonFilter(filter);
        inputNode->setCallback(func);

        leftMenu->updateLayout();
    };
    m_mainLayer->addChildAtPosition(leftMenu, Anchor::Center, {-100,-125});

    createComponent("Pack Name", ccp(-100.f, 65.f), m_nameInput, CommonFilter::Name);
    createComponent("Pack ID", ccp(-100.f, 15.f), m_idInput, CommonFilter::ID);
    createComponent("Pack Description", ccp(-100.f, -35.f), m_descriptionInput, CommonFilter::Any);
    createComponent("Pack Creator", ccp(-100.f, -85.f), m_authorInput, CommonFilter::Name);
    auto verWarnBtn = CCMenuItemExt::createSpriteExtraWithFrameName("exMark_001.png", .45f, [this](CCMenuItemSpriteExtra*){
        FLAlertLayer::create("Invalid version", m_versionWarning.c_str(), "Ok")->show();
    });
    verWarnBtn->setVisible(false);
    createComponent("Pack Version", ccp(-100.f, -135.f), m_versionInput, CommonFilter::ID, [this, verWarnBtn](std::string const& text) {
        if (text == "") return verWarnBtn->setVisible(false);
        auto vi = VersionInfo::parse(text);
        if (vi.isErr()) {
            m_versionInput->getInputNode()->getTextLabel()->setColor(ccc3(200, 30, 30));
            verWarnBtn->setVisible(true);
            m_versionWarning = std::string(vi.unwrapErr());
        } else {
            m_versionInput->getInputNode()->getTextLabel()->setColor(ccc3(255, 255, 255));
            verWarnBtn->setVisible(false);
        }
    });
    m_buttonMenu->addChildAtPosition(verWarnBtn, Anchor::BottomLeft, {leftMenu->getPositionX() + m_versionInput->getContentWidth() / 2 - verWarnBtn->getContentWidth() - 11.25f, 17.5f + m_versionInput->getContentHeight()});

    auto logoSelectMenu = Build<CCMenu>::create()
        .layout(menuLayout)
        .width(width / 2)
        .height(height)
        .scale(0.8f)
        .anchorPoint({.5f, .5f})
        .collect();

    this->m_logoPreview = Build<LazySprite>::create(CCSize{125.f, 125.f}, false).collect();
    m_logoPreview->setAutoResize(true);

    auto logoSelectButton = Build<ButtonSprite>::create("Select Logo", "bigFont.fnt", "GJ_button_01.png", 0.8f)
        .intoMenuItem([this]() {
            m_pickListener.bind(this, &PackCreationPopup::onLogoSelect);
            file::FilePickOptions options = {
                std::nullopt,
                {{.description = "Image File",
                  .files = {"*.png", "*.jpg"}}}};
            m_pickListener.setFilter(file::pick(file::PickMode::OpenFile, options));
        })
    .collect();

    auto exportButton = Build<ButtonSprite>::create("Export Pack", "bigFont.fnt", "GJ_button_01.png", 0.8f)
        .intoMenuItem([this]() {
            if (m_nameInput->getString().empty() ||
                m_idInput->getString().empty() ||
                m_descriptionInput->getString().empty() ||
                m_authorInput->getString().empty() ||
                m_versionInput->getString().empty() ||
                m_logoPath.empty()
            ) {
                FLAlertLayer::create(
                    "Pack Creation Error!",
                    "One or more of the <cr>Required</c> fields has been left empty.",
                    "Ok"
                )->show();
                return;
            }

            m_pickListener.bind(this, &PackCreationPopup::onExport);
            file::FilePickOptions options = {
                std::nullopt,
                {{.description = "Mod Profile",
                .files = {".modprofile"}}}
            };
            m_pickListener.setFilter(file::pick(file::PickMode::SaveFile, options));
        })
        .collect();

    logoSelectMenu->addChild(logoSelectButton);
    logoSelectMenu->addChild(exportButton);
    logoSelectMenu->updateLayout();
    m_mainLayer->addChildAtPosition(m_logoPreview, Anchor::Right, ccp(-112.5f, 25.f));
    m_mainLayer->addChildAtPosition(logoSelectMenu, Anchor::Right, ccp(-112.5f, 0.f));

    return true;
}

void PackCreationPopup::onExport(Task<Result<std::filesystem::path>>::Event *event) {
    if (event->isCancelled()) {
        return;
    }
    if (auto result = event->getValue()) {
        if (!result->isOk()) {
            FLAlertLayer::create(
                "Error",
                fmt::format("Failed to save file. Error: {}", result->err().value()),
                "Ok")
                ->show();
            return;
        }

        auto createPack = modutils::Mod::get()->createPack(
            ModProfile(
                m_nameInput->getString(),
                m_idInput->getString(),
                m_descriptionInput->getString(),
                m_authorInput->getString(),
                m_versionInput->getString(),
                modProfileMods
            ),
            this->m_logoPath,
            result->unwrap().string()
        );
        if (!createPack) {
            FLAlertLayer::create(
                "Pack Creation",
                fmt::format("Failed to create pack: {}", createPack.unwrapErr()),
                "Ok"
            )->show();
        } else {
            // createPack.unwrap();
            FLAlertLayer::create(
                "Pack Creation",
                "Pack created successfully.",
                "Ok"
            )->show();
            onClose(nullptr);
        }
    }
}

void PackCreationPopup::onLogoSelect(Task<Result<std::filesystem::path>>::Event *event) {
    if (event->isCancelled()) {
        return;
    }
    if (auto result = event->getValue()) {
        if (!result->isOk()) {
            FLAlertLayer::create(
                "Error",
                fmt::format("Failed to open file. Error: {}", result->err().value()),
                "Ok")
                ->show();
            return;
        }

        this->m_logoPath = result->unwrap().string();
        m_logoPreview->loadFromFile(m_logoPath);
    }
}

PackCreationPopup* PackCreationPopup::create(std::vector<geode::Mod*> const& mods) {
    auto ret = new PackCreationPopup();
    if (ret->initAnchored(width, height, mods)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}