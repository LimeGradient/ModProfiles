#pragma once

#include <Geode/Geode.hpp>

#include <UI/ModCell.hpp>

using namespace geode::prelude;

class ExportList : public CCNode {
protected:
    bool init(std::vector<ModCell*> cells, std::string title);

    GJListLayer* m_listLayer;
    ScrollLayer* m_scrollLayer;
public:
    static ExportList* create(std::vector<ModCell*> cells, std::string title);
};