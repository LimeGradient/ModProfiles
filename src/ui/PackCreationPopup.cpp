#include "PackCreationPopup.hpp"

#include <any>
#include <UIBuilder.hpp>

#include <utils/ModProfiles.hpp>

using namespace geode::prelude;

bool PackCreationPopup::setup(const std::vector<geode::Mod*> &mods) {
    this->setTitle("Create Pack");

    auto menuLayout = ColumnLayout::create()
        ->setDefaultScaleLimits(.1f, 1.f)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setAxisReverse(true)
        ->setAutoScale(false);


    auto createComponent = [menuLayout, this](std::string label, CCPoint offset, TextInput* store) {
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
        .scale(0.8f)
        .anchorPoint({.5f, .0f})
        .collect();
    
    Build<CCSprite>::create()
        .scale(.75f)
        .parent(logoSelectMenu)
        .store(m_logoPreview);
    
    auto logoSelectButton = Build<ButtonSprite>::create("Select Logo", "bigFont.fnt", "GJ_button_01.png", 0.8f)
        .intoMenuItem([this]() {
            m_pickListener.bind(this, &PackCreationPopup::onLogoSelect);
            m_pickListener.setFilter(file::pick(file::PickMode::OpenFile, {
                std::nullopt
            }));
        })
        .intoNewParent(CCMenu::create())
        .parent(logoSelectMenu)
        .collect();

    logoSelectMenu->updateLayout();
    m_mainLayer->addChildAtPosition(logoSelectMenu, Anchor::Center, ccp(75.f, 0.f));

    return true;
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

        auto image = new CCImage();
        image->initWithImageFile(result->unwrap().c_str());

        auto texture = new CCTexture2D();
        texture->initWithImage(image);

        m_logoPreview->createWithTexture(texture);

        image->release();
        texture->release();
    }
}

PackCreationPopup* PackCreationPopup::create(const std::vector<geode::Mod*> &mods) {
    auto ret = new PackCreationPopup();
    if (ret->initAnchored(width, height, mods)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}