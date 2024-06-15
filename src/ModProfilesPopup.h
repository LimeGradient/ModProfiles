#pragma once

#include <filesystem>

#include "BetterFLAlert.h"

namespace fs = std::filesystem;

class ModProfilesPopup : public BetterFLAlert {
    protected:
        virtual void setup();
    public:
        static const file::FilePickOptions m_options;

        static ModProfilesPopup* create();
        void exportMods(CCObject*);
        void importMods(CCObject*);
        void onDiscord(CCObject*);
        void onRestart(CCObject*);
};