#include "ModCell.hpp"

bool ModCell::init(Mod* mod) {
    if (!CCNode::init()) return false;

    this->m_mod = mod;

    Build<CCScale9Sprite>::create("square02b_small.png")
        .id(this->makeID("bg"))
        .ignoreAnchorPointForPos(false)
        .anchorPoint({0.f, 0.f})
        .scale(0.7f)
        .color(ccWHITE)
        .opacity(25)
        .parent(this)
        .store(m_bg);

    m_logo = geode::createModLogo(mod);
    m_logo->setID("logo-sprite");
    m_logo->setScale(0.5f);
    m_logo->setAnchorPoint({.0f, .0f});
    m_logo->setPositionY(m_logo->getPositionY() + 2.f);
    this->addChild(m_logo);

    Build<CCNode>::create()
        .id(this->makeID("info-container"))
        .scale(.4f)
        .anchorPoint({0.f, 0.5f})
        .layout(ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0)
        )
        .parent(this)
        .store(m_infoContainer);
    
    this->m_infoContainer->getLayout()->ignoreInvisibleChildren(true);

    Build<CCNode>::create()
        .id(this->makeID("title-container"))
        .anchorPoint({0.f, 0.5f})
        .layout(RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setAxisAlignment(AxisAlignment::Start)
        )
        .store(m_titleContainer)
        .parent(m_infoContainer)
        .center()
        .intoNewChild(CCLabelBMFont::create(mod->getName().c_str(), "bigFont.fnt"));
    
    this->setID(mod->getID());
    this->setContentSize(CCSize{m_bg->getScaledContentWidth(), m_bg->getScaledContentHeight()});
    return true;
}

std::string ModCell::makeID(std::string id) {
    return fmt::format("{}-{}", this->m_mod->getID(), id);
}

ModCell* ModCell::create(Mod* mod) {
    auto ret = new ModCell();
    if (ret && ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}