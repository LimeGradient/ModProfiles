#pragma once

#include <string>

class PackInfo {
public:
    PackInfo(std::string title, std::string description, std::string author, bool includesLocalMods);

    std::string title;
    std::string description;
    std::string author;
    bool includesLocalMods;
};