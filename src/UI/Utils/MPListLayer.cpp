#include "MPListLayer.hpp"

bool MPListLayer::init(std::vector<ModCell*> cells, std::string title) {
    auto listView = ListView::create(CCArray::create(), 0.f, 358.f, 220.f);

    Build<GJListLayer>::create(listView, title.c_str(), ccc4(168, 85, 44, 255), 358.f, 220.f, 0)
        .anchorPoint({0.5f, 0.55f})
        .id("list-layer")
        .parent(this)
        .ignoreAnchorPointForPos(false)
        .center()
        .store(m_listLayer);
    
    Build<ScrollLayer>::create(ccp(358.f, 220.f))
        .id("scroll-layer")
        .parent(m_listLayer)
        .store(m_scrollLayer);

    m_scrollLayer->m_contentLayer->setLayout(ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(m_listLayer->getContentHeight() - 30.f)
        ->setGap(2.5f)
    );

    float totalHeight = .0f;
    for (ModCell* cell : cells) {
        cell->setContentWidth(m_scrollLayer->getScaledContentWidth());
        cell->m_bg->setContentWidth(m_scrollLayer->getScaledContentWidth() + 125.f);
        cell->m_infoContainer->setPositionX(cell->m_bg->getScaledContentWidth() / 2);
        totalHeight += cell->getScaledContentSize().height;
        cell->setPosition(125.f, totalHeight);

        m_scrollLayer->m_contentLayer->addChild(cell);
        m_scrollLayer->m_contentLayer->updateLayout();
        m_scrollLayer->updateLayout();
    }
    
    return true;
}

MPListLayer* MPListLayer::create(std::vector<ModCell*> cells, std::string title) {
    auto ret = new MPListLayer();
    if (ret && ret->init(cells, title)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}