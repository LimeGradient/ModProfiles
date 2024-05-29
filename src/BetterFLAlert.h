#include <Geode/Geode.hpp>

using namespace geode::prelude;
class BetterFLAlert : public FLAlertLayer {
    protected:
        cocos2d::CCSize m_layerSize;
        cocos2d::extension::CCScale9Sprite* m_bgSprite;

        virtual bool init(float width, float height, const char* bg = "GJ_square02.png", const char* title = "");
        virtual void setup() = 0;
        virtual void keyDown(cocos2d::enumKeyCodes) override;

        virtual void onClose(cocos2d::CCObject*);

        ~BetterFLAlert() override {
            cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
        }
};