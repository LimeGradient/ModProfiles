#pragma once

#include <string>
#include <vector>

#include <Geode/Geode.hpp>
#include <UIBuilder.hpp>

#include "UI/ModCell.hpp"

using namespace geode::prelude;

class MPListLayer : public CCNode {
protected:
    bool init(std::vector<ModCell*> cells, std::string title);

    GJListLayer* m_listLayer;
    ScrollLayer* m_scrollLayer;

public:
    void refreshList(std::vector<ModCell*> cells);

    static MPListLayer* create(std::vector<ModCell*> cells, std::string title);
};