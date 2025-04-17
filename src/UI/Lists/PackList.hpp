#pragma once

#include <Geode/Geode.hpp>

#include <UIBuilder.hpp>

using namespace geode::prelude;

class PackList : public CCNode {
protected:
    bool init(std::string title);

    GJListLayer* m_listLayer;
    ScrollLayer* m_scrollLayer;
public:
    static PackList* create(std::string title);
};