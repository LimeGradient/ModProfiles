#include "PackCreationPopup.hpp"

#include <UIBuilder.hpp>

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


    auto createComponent = [menuLayout, this](std::string label, CCPoint offset, TextInput*& store) {
        auto menu = Build<CCMenu>::create()
            .layout(menuLayout)
            .scale(0.8f)
            .anchorPoint({.5f, .0f})
            .collect();
        
        auto labelNode = Build(CCLabelBMFont::create(label.c_str(), "bigFont.fnt"))
            .scale(0.65f)
            .parent(menu)
            .collect();
        
        auto inputNode = Build<TextInput>::create(200.f, label.c_str(), "chatFont.fnt")
            .store(store)
            .parent(menu)
            .collect();
        inputNode->setCommonFilter(CommonFilter::Any);
        
        menu->updateLayout();
        m_mainLayer->addChildAtPosition(menu, Anchor::Center, offset);
    };

    createComponent("Pack Name:", ccp(-100.f, 65.f), m_nameInput);
    createComponent("Pack ID:", ccp(-100.f, 15.f), m_idInput);
    createComponent("Pack Description:", ccp(-100.f, -35.f), m_descriptionInput);
    createComponent("Pack Creator", ccp(-100.f, -85.f), m_authorInput);
    createComponent("Pack Version", ccp(-100.f, -135.f), m_versionInput);

    auto logoSelectMenu = Build<CCMenu>::create()
        .layout(menuLayout)
        .width(width / 2)
        .height(height)
        .scale(0.8f)
        .anchorPoint({.5f, .5f})
        .collect();
    
    this->m_logoPreview = LazySprite::create({500.f, 500.f}, false);
    
    auto logoSelectButton = Build<ButtonSprite>::create("Select Logo", "bigFont.fnt", "GJ_button_01.png", 0.8f)
        .intoMenuItem([this]() {
            m_pickListener.bind(this, &PackCreationPopup::onLogoSelect);
            file::FilePickOptions options = {
                std::nullopt,
                {{.description = "Image File",
                  .files = {"*.png", "*.jpg"}}}};
            m_pickListener.setFilter(file::pick(file::PickMode::OpenFile, options));
        })
        .intoNewParent(CCMenu::create())
        .width(width / 2)
        .height(height)
        .collect();
    
    auto createPackButton = Build<ButtonSprite>::create("Export Pack", "bigFont.fnt", "GJ_button_01.png", 0.8f)
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
        .intoNewParent(CCMenu::create())
        .width(width / 2)
        .height(height)
        .scale(.8f)
        .anchorPoint({.0f, .0f})
        .collect();

    logoSelectMenu->addChildAtPosition(logoSelectButton, Anchor::Bottom, ccp(0.f, 35.f));
    logoSelectMenu->addChildAtPosition(m_logoPreview, Anchor::Top, ccp(0.f, -125.f));
    m_mainLayer->addChildAtPosition(logoSelectMenu, Anchor::Center, ccp(100.f, 0.f));
    m_mainLayer->addChildAtPosition(createPackButton, Anchor::Bottom, ccp(100.f, 25.f));

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
                "Pack Creation Error!",
                fmt::format("Failed to create pack: {}", createPack.unwrapErr()),
                "Ok"
            )->show();
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