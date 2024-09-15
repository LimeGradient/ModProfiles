#include "ExportProfilesList.h"
#include "ModCell.h"

#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/Utils/web.hpp>
using namespace geode::prelude;

bool sort_by_name(const Mod* modA, const Mod* modB) {
    return modA->getName() < modB->getName();
}

EventListener<FileTask> m_fileTaskListener;
EventListener<web::WebTask> m_webTaskListener;

file::FilePickOptions options = {
    std::nullopt,
    {
        {
            "Mod Profile",
            {".modprofile"}
        },
    }
};

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

    m_btnMenu = CCMenu::create();
    m_btnMenu->setID("button-menu");

    auto exportSpr = ButtonSprite::create("Export", "bigFont.fnt", "geode-button.png"_spr);
    exportSpr->setScale(.65f);
    m_exportBtn = CCMenuItemSpriteExtra::create(
        exportSpr, this, menu_selector(ExportProfilesList::onExport)
    );
    m_exportBtn->setID("export-button");
    m_btnMenu->addChild(m_exportBtn);
    m_btnMenu->setLayout(RowLayout::create());
    m_btnMenu->getLayout()->ignoreInvisibleChildren(true);

    this->addChildAtPosition(m_btnMenu, Anchor::Bottom, ccp(0, -13.f));

    return true;
}

FileTask exportToFile() {
    return file::pick(file::PickMode::SaveFile, options);
}

void ExportProfilesList::exportProfile(FileTask::Event* e) {
    std::vector<std::string> modLinks;

    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();

            CCObject* obj;
            CCARRAY_FOREACH(m_list->m_contentLayer->getChildren(), obj) {
                auto* node = static_cast<CCNode*>(obj);
                CCMenuItemToggler* toggler = static_cast<CCMenuItemToggler*>(node->getChildByID("view-menu")->getChildByID("enable-toggler"));
                log::info("mod: {} - is toggled: {}", node->getID(), toggler->isToggled());
                if (toggler->isToggled()) {
                    modLinks.push_back(fmt::format("https://api.geode-sdk.org/v1/mods/{}", node->getID()));
                }
            }

            std::ofstream out;
            out.open(fmt::format("{}_profile.modprofile", path), std::ios::trunc);
            for (std::string link : modLinks) {
                out << link << "\n";
            }
            out.close();
            geode::Notification::create("Success! Created Profile!", geode::NotificationIcon::Success)->show();
        }
    } else if (e->isCancelled()) {
        geode::Notification::create("File Operation Cancelled", geode::NotificationIcon::Error)->show();
    }
}

void ExportProfilesList::exportProfileWithLocalMods(FileTask::Event* e) {
    std::vector<std::string> modLinks;

    if (auto result = e->getValue()) {
        if (result->isOk()) {
            auto path = result->unwrap();
            CCObject* obj;

            CCARRAY_FOREACH(m_list->m_contentLayer->getChildren(), obj) {
                auto* node = static_cast<CCNode*>(obj);
                CCMenuItemToggler* toggler = static_cast<CCMenuItemToggler*>(node->getChildByID("view-menu")->getChildByID("enable-toggler"));
                m_webTaskListener.bind([=] (web::WebTask::Event* e) {
                    if (web::WebResponse* res = e->getValue()) {
                        if (res->ok()) {
                            auto modJSON = res->json();
                            if (modJSON.unwrap()["error"] == "") {
                                log::info("{} isnt local", node->getID());
                            } else if (modJSON.unwrap()["error"] != "") {
                                log::info("{} is local", node->getID());
                            }
                        }
                    }
                });

                if (toggler->isToggled()) {
                    auto req = web::WebRequest();
                    m_webTaskListener.setFilter(req.get(fmt::format("https://api.geode-sdk.org/v1/mods/{}", node->getID())));
                }
            }
        }
    }
}

void ExportProfilesList::onExport(CCObject*) {
    m_fileTaskListener.bind([=] (auto* e) {
        if (Mod::get()->getSavedValue<bool>("include-local-mods")) {
            exportProfileWithLocalMods(e);
        } else {
            exportProfile(e);
        }
        exportProfile(e);
    });
    m_fileTaskListener.setFilter(exportToFile());
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