#pragma once

#include <string>
#include <vector>

#include <Geode/Geode.hpp>
#include <zip.h>

using namespace geode::prelude;

class Zip {
public:
    void zipFiles(std::string zipPath, std::vector<std::string> files, std::vector<std::string> modNames);
};