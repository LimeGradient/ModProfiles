#include "Mods.hpp"

namespace modutils {
    std::vector<geode::Mod*> Mod::getAllMods() {
        auto sort_by_name = [](geode::Mod* a, geode::Mod* b) {
            return a->getName() < b->getName();
        };
    
        std::list<geode::Mod*> mods;
        std::ranges::copy(geode::Loader::get()->getAllMods(), std::back_inserter(mods));
        mods.sort(sort_by_name);

        return std::vector<geode::Mod*>{std::begin(mods), std::end(mods)};
    }
}