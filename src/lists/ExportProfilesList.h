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

public:
    static ExportProfilesList* create(CCSize const& size);
    void exportProfile(FileTask::Event* e);
    void exportProfileWithLocalMods(FileTask::Event* e);
};

class ModpackInfoPopup : public geode::Popup<> {
    protected:
        TextInput* m_modpackTitle;
        TextInput* m_modpackAuthor;
        TextInput* m_modpackDescription;
        CCMenuItemSpriteExtra* m_createPackBtn;

        void onCreatePack(CCObject* sender);

        bool setup() override;

    public:
        static constexpr float POPUP_WIDTH = 300.f;
        static constexpr float POPUP_HEIGHT = 300.f;

        static ModpackInfoPopup* create();
};