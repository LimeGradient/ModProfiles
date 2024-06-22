#include "ExportProfilesList.h"
#include "ModCell.h"

#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
using namespace geode::prelude;

bool ExportProfilesList::init(CCSize const& size) {
    if (!CCNode::init()) return false;

    this->setContentSize(size);
    this->setAnchorPoint({.5f, .5f});
    this->setID("ExportProfilesList");

    m_list = ScrollLayer::create(size);
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(2.5f)
    );
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0));

    m_topContainer = CCNode::create();
    m_topContainer->setID("top-container");
    m_topContainer->ignoreAnchorPointForPosition(false);
    m_topContainer->setContentWidth(size.width);
    m_topContainer->setAnchorPoint({ .5f, 1.f });

    float totalHeight = .0f;
    for (auto mod : Loader::get()->getAllMods()) {
        auto modCell = ModCell::create(mod);
        totalHeight += modCell->getScaledContentHeight();
        modCell->setPosition(50.f, totalHeight);
        m_list->m_contentLayer->addChild(modCell);
    }
    m_list->m_contentLayer->updateLayout();

    return true;
}

ExportProfilesList* ExportProfilesList::create(CCSize const& size) {
    auto ret = new ExportProfilesList();
    if (ret && ret->init(size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}