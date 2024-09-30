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

    if (!packInfo->logo.empty()) {
        auto testSprite = CCSprite::create(packInfo->logo.c_str());
        if (!testSprite) return true;

        CCImage* image = new CCImage();
        image->initWithImageFile(packInfo->logo.c_str());
        CCTexture2D* texture = new CCTexture2D();
        texture->initWithImage(image);
        m_logo = CCSprite::createWithTexture(texture);
        image->release();
        texture->release();
        
        m_logo->setScale(0.135f);
    } else {
        m_logo = CCSprite::createWithSpriteFrameName("discord-icon.png"_spr);
        m_logo->setScale(0.75f);
    }

    m_logo->setID("logo-sprite");
    m_logo->setAnchorPoint({.5f, .5f});
    m_logo->setPosition({15.f, 14.f});
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setID("info-container");
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({ .0f, .0f });
    m_infoContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0)
    );
    m_infoContainer->getLayout()->ignoreInvisibleChildren(true);
    m_infoContainer->setPositionY(m_infoContainer->getPositionY() + 20.f);

    m_titleContainer = CCNode::create();
    m_titleContainer->setID("title-container");
    m_titleContainer->setAnchorPoint({ .0f, .5f });
    m_titleContainer->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setAxisAlignment(AxisAlignment::Start)
    );

    m_titleLabel = CCLabelBMFont::create(packInfo->title.c_str(), "bigFont.fnt");
    m_titleLabel->setID("title-label");
    m_titleContainer->addChild(m_titleLabel);

    m_infoContainer->addChild(m_titleContainer);
    
    m_developers = CCMenu::create();
    m_developers->setID("developers-menu");
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({ .0f, .5f });

    m_developerLabel = CCLabelBMFont::create(packInfo->author.c_str(), "goldFont.fnt");
    m_developerLabel->setID("developers-label");
    m_developers->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    m_developers->addChild(m_developerLabel);
    m_developers->setPosition({0.f, 130.f});
    m_infoContainer->addChild(m_developers);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setAnchorPoint({ 1.f, .0f });
    m_viewMenu->setScale(.55f);
     m_viewMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(10)
    );
    m_viewMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));
    m_viewMenu->setPositionY(m_viewMenu->getPositionY() + 4.f);

    m_enableToggle = CCMenuItemToggler::createWithStandardSprites(this, menu_selector(PackCell::onEnable), 1.f);
    m_enableToggle->setID("enable-toggler");
    m_viewMenu->addChild(m_enableToggle);
    m_viewMenu->updateLayout();

    this->addChild(m_infoContainer);
    this->setContentSize(CCSize{m_bg->getScaledContentWidth(), m_bg->getScaledContentHeight()});
    m_infoContainer->setContentSize(this->getScaledContentSize());

    return true;
}

void PackCell::onEnable(CCObject*) {
    auto packSelectLayer = static_cast<PackSelectList*>(CCScene::get()->getChildByIDRecursive("pack-select-list"));
    if (!this->m_enableToggle->isToggled()) {
        packSelectLayer->packSelect(this->m_packInfo);
    } else {
        packSelectLayer->packDeselect();
    }
}

PackCell* PackCell::create(PackInfo* packInfo) {
    auto ret = new PackCell();
    if (ret && ret->init(packInfo)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}