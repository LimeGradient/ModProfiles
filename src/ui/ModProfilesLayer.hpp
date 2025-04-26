#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModProfilesLayer : public CCLayer {
protected:
    bool init() override;
    void keyBackClicked() override;

    std::vector<CCMenuItemSpriteExtra*> m_tabs;
    std::string m_currentTab = "my-packs";
    std::map<std::string, Ref<ScrollLayer>> m_scrolls;
    EventListener<Task<Result<std::filesystem::path>>> m_pickListener;

    CCNode *m_listFrame = nullptr;
public:
    void onBack(CCObject *sender);
    void onTab(CCObject *sender);
    void onImport(CCObject *sender);
    void onExport(CCObject *sender);

    void goToTab(std::string id);

    void onFileOpen(Task<Result<std::filesystem::path>>::Event *event);

    static ModProfilesLayer *create();
    static CCScene *scene();
};

class TabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static TabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);

    void select(bool selected);
    void disable(bool disabled);
};