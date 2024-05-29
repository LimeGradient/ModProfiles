#include "BetterFLAlert.h"

class ModProfilesPopup : public BetterFLAlert {
    protected:
        virtual void setup();
        file::FilePickOptions::Filter m_filter;
		file::FilePickOptions m_options;
    public:
        static ModProfilesPopup* create();
        void exportMods(CCObject*);
        void importMods(CCObject*);
        void onDiscord(CCObject*);
        void onRestart(CCObject*);
};