#pragma once

#include <Geode/Geode.hpp>
#include <filesystem>
#include "utils/PackInfo.h"
using namespace geode::prelude;
namespace fs = std::filesystem;
using FileTask = Task<Result<fs::path>>;

class ExportProfilesList : public CCNode {
protected:
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_exportBtn;

    std::vector<Mod*> modsToExport;

    bool init(CCSize const& size);
    void updateTopContainer();
    void onPage(CCObject*);
    void onExport(CCObject*);

public:
    static ExportProfilesList* create(CCSize const& size);
    void exportProfile(FileTask::Event* e, PackInfo* packInfo);
};