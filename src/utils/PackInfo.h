#pragma once

#include <string>

class PackInfo {
public:
    PackInfo() {};
    PackInfo(std::string title, std::string description, std::string author, bool includesLocalMods);

    void setPackInfo(std::string title, std::string description, std::string author, bool includesLocalMods);

    std::string title;
    std::string description;
    std::string author;
    std::string logo;
    bool includesLocalMods;
};