#pragma once

#include <Geode/Geode.hpp>
#include <filesystem>
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

    void exportProfile(FileTask::Event* e);
    void exportProfileWithLocalMods(FileTask::Event* e);

public:
    static ExportProfilesList* create(CCSize const& size);
    size_t getPage() const;
    void reloadPage();
    void gotoPage(size_t page, bool update = false);
};