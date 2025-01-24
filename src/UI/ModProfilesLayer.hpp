#pragma once

#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>

#include "ModCell.hpp"
#include "Utils/MPListLayer.hpp"
#include "Backend/Mods.hpp"

using namespace geode::prelude;

class ModProfilesLayer : public CCLayer {
protected:
    CCScale9Sprite* m_bg;
    GJListLayer* m_listLayer;
    
    CCMenu* m_tabButtonMenu;
    TabButton* m_exportTab;
    TabButton* m_importTab;
    TabButton* m_packsTab;

    MPListLayer* m_mpListLayer;

    void onClose(CCObject*);

    void keyBackClicked() override;

public:
    static ModProfilesLayer* create();
    static CCScene* scene();
    bool init() override;
};