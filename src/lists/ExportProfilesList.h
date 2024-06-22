#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;

class ExportProfilesList : public CCNode {
protected:
    size_t m_page = 0;
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenuItemSpriteExtra* m_pagePrevBtn;
    CCMenuItemSpriteExtra* m_pageNextBtn;

    bool init(CCSize const& size);
    void updateTopContainer();
    void onPage(CCObject*);

public:
    static ExportProfilesList* create(CCSize const& size);
    size_t getPage() const;
    void reloadPage();
    void gotoPage(size_t page, bool update = false);
};