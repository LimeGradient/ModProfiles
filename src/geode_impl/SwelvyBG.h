/*
Implementation of SwelvyBG
https://github.com/geode-sdk/geode/blob/main/loader/src/ui/mods/SwelvyBG.hpp
*/

#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/General.hpp>

using namespace geode::prelude;

class SwelvyBG : public CCNode {
protected:
    bool init();
    void updateSpritePosition(float dt);

public:
    static SwelvyBG* create();
};