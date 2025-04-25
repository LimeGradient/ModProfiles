#pragma once

#include <vector>

#include <Geode/loader/Mod.hpp>

namespace modutils {
    class Mod {
    public:
        static Mod* get() {
            static Mod instance;
            return &instance;
        }

        std::vector<geode::Mod*> getAllMods();
    };
}