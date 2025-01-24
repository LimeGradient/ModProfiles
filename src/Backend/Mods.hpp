#pragma once

#include <vector>
#include <list>

#include <Geode/Geode.hpp>

#include "UI/ModCell.hpp"
using namespace geode::prelude;

class ModBackend {
public:
    static ModBackend* get() {
        static ModBackend* instance;
        return instance;
    }

    std::vector<ModCell*> getMods();
};