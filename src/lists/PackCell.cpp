#include "PackCell.h"

bool PackCell::init(PackInfo* packInfo) {
    if (!CCNode::init()) return false;

    this->m_packInfo = packInfo;

    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .0f, .0f });
    m_bg->setScale(.7f);
    m_bg->setColor(ccWHITE);
    m_bg->setOpacity(25);
    this->addChild(m_bg);

}