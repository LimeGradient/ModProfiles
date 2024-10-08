#pragma once

#include <filesystem>

#include <Geode/Geode.hpp>
#include <Geode/Loader.hpp>
#include <Geode/utils/web.hpp>

#include "utils/PackInfo.h"
#include "ModCell.h"
#include "ModpackInfoPopup.h"
#include "utils/ziputils.h"
using namespace geode::prelude;
namespace fs = std::filesystem;
using FileTask = Task<Result<fs::path>>;

class ExportProfilesList : public CCNode {
protected:
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_exportBtn;

    std::vector<Mod*> modsToExport;

    bool init(CCSize const& size);
    void onExport(CCObject*);

public:
    static ExportProfilesList* create(CCSize const& size);
    void exportProfile(FileTask::Event* e, PackInfo* packInfo);
};