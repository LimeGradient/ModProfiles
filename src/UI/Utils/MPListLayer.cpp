#include "MPListLayer.hpp"
#include "UI/Lists/ExportList.hpp"
#include "UI/Lists/ImportList.hpp"
#include "UI/Lists/PackList.hpp"

bool MPListLayer::init(std::vector<ModCell*> cells, std::string title, MPListType type) {
    switch (type) {
        case MPListType::ExportList: {
            auto exportList = ExportList::create(cells, "Export Mods");
            exportList->setPositionY(exportList->getPositionY() + 10.f);
            this->addChild(exportList);
            break;
        }
        case MPListType::ImportList: {
            auto importList = ImportList::create("Import Mods");
            importList->setPositionY(importList->getPositionY() + 10.f);
            this->addChild(importList);
            break;
        }
        case MPListType::ProfilesList: {
            auto packList = PackList::create("Profiles List");
            packList->setPositionY(packList->getPositionY() + 10.f);
            this->addChild(packList);
            break;
        }
    }

    return true;
}

MPListLayer* MPListLayer::create(std::vector<ModCell*> cells, std::string title, MPListType type) {
    auto ret = new MPListLayer();
    if (ret && ret->init(cells, title, type)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}