#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModProfilesLayer : public CCLayer {
    protected:
        bool init() override;
        void keyBackClicked() override;
    public:
        void onBack(CCObject *sender);

        static ModProfilesLayer *create();
        static CCScene *scene();
};