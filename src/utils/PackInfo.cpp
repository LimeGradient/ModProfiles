#include "PackInfo.h"

PackInfo::PackInfo(std::string title, std::string description, std::string author, bool includesLocalMods) {
    this->title = title;
    this->description = description;
    this->author = author;
    this->includesLocalMods = includesLocalMods;
}

void PackInfo::setPackInfo(std::string title, std::string description, std::string author, bool includesLocalMods) {
    this->title = title;
    this->description = description;
    this->author = author;
    this->includesLocalMods = includesLocalMods;
}