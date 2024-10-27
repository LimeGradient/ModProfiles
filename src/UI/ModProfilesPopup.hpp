#pragma once

#include <Geode/Geode.hpp>

#include <UIBuilder.hpp>

using namespace geode::prelude;

class ModProfilesPopup : public geode::Popup<> {
protected:
    bool setup() override;

    static constexpr float popupWidth = 400.f;
    static constexpr float popupHeight = 240.f;

    ButtonSprite* m_exportBtn;
    ButtonSprite* m_importPackBtn;

public:
    static ModProfilesPopup* create() {
        auto ret = new ModProfilesPopup();
        if (ret->init(popupWidth, popupHeight, "geode.loader/GE_square02.png")) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};