#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <filesystem>
#include "utils/PackInfo.h"
#include "utils/ziputils.h"
#include "PackCell.h"
using namespace geode::prelude;
namespace fs = std::filesystem;
using FileTask = Task<Result<fs::path>>;

class PackSelectList : public CCNode {
protected:
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenu* m_btnMenu;
    ButtonSprite* m_importPackBtnSpr;
    CCMenuItemSpriteExtra* m_importPackBtn;
    CCMenuItemSpriteExtra* m_restartBtn;

    std::vector<PackInfo*> m_packs;
    std::string m_selectedPackPath;

    bool init(CCSize const& size);
    void getAllPacks();

    void onImportPack(CCObject*);
    void onRestartGame(CCObject*);

public:
    static PackSelectList* create(CCSize const& size);

    void packSelect(PackInfo* packInfo);
};