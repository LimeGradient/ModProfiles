#include "ExportProfilesList.h"
#include "ModCell.h"

#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
using namespace geode::prelude;

bool sort_by_name(const Mod* modA, const Mod* modB) {
    return modA->getName() < modB->getName();
}

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

    std::list<Mod*> modsList;
    std::ranges::copy(Loader::get()->getAllMods(), std::back_inserter(modsList));
    modsList.sort(sort_by_name);

    for (auto mod : modsList) {
        auto modCell = ModCell::create(mod);
        modCell->setContentWidth(this->getScaledContentWidth());
        modCell->m_bg->setContentWidth(this->getScaledContentWidth() + 150.f);
        modCell->m_infoContainer->setPositionX(modCell->m_bg->getScaledContentWidth() / 2);
        modCell->m_viewMenu->setPositionX(modCell->m_bg->getScaledContentWidth() - 50.f);
        modCell->m_enableToggle->setPositionX(630.f);
        
        totalHeight += modCell->getScaledContentSize().height;
        modCell->setPosition(115.f, totalHeight);
        m_list->m_contentLayer->addChild(modCell);
        m_list->m_contentLayer->updateLayout();
        m_list->updateLayout();
    }

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