#pragma once

#include <Geode/Geode.hpp>
#include <filesystem>

#include "utils/PackInfo.h"

using namespace geode::prelude;
namespace fs = std::filesystem;
#ifndef FileTask
using FileTask = Task<Result<fs::path>>;
#endif

class ModpackInfoPopup : public geode::Popup<> {
    protected:
        EventListener<FileTask> m_fileTaskListener;

        TextInput* m_modpackTitle;
        TextInput* m_modpackAuthor;
        TextInput* m_modpackDescription;
        CCMenuItemSpriteExtra* m_chooseLogoBtn;
        CCLabelBMFont* m_logoPathText;
        CCMenuItemSpriteExtra* m_createPackBtn;

        PackInfo* m_packInfo;

        void getLogo(FileTask::Event* e);

        void onChooseLogo(CCObject*);
        void onCreatePack(CCObject*);

        bool setup() override;

    public:
        static constexpr float POPUP_WIDTH = 300.f;
        static constexpr float POPUP_HEIGHT = 300.f;

        static ModpackInfoPopup* create();
};