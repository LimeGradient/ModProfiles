#include "Mods.hpp"

std::vector<ModCell*> ModBackend::getMods() {
    // sort the list by mod name
    auto sort_by_name = [](Mod* a, Mod* b) {
        return a->getName() < b->getName();
    };

    std::list<Mod*> mods;
    std::ranges::copy(Loader::get()->getAllMods(), std::back_inserter(mods));
    mods.sort(sort_by_name);

    std::vector<ModCell*> modCells;
    for (Mod* mod : mods) {
        modCells.push_back(ModCell::create(mod));
    }

    mods.clear();
    return modCells;
}