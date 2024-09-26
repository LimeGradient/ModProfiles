#pragma once

#include <Geode/Geode.hpp>
#include <filesystem>
using namespace geode::prelude;
namespace fs = std::filesystem;
using FileTask = Task<Result<fs::path>>;

class ImportProfilesList : public CCNode {
protected:
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_exportBtn;

    std::vector<Mod*> modsToExport;

    bool init(CCSize const& size);
    void onExport(CCObject*);

    void importProfile(FileTask::Event* e);

public:
    static ImportProfilesList* create(CCSize const& size);
};