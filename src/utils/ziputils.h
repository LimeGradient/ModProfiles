#pragma once

#include <string>
#include <vector>

#include <Geode/Geode.hpp>
#include <zip.h>

using namespace geode::prelude;

class Zip {
public:
    Zip(std::string zipPath);

    void zipFiles(std::vector<std::string> files, std::vector<std::string> modNames);
    void appendToZip(std::string filename, std::string file);
    void writeStringToZip(std::string filename, std::string content);
    void close();

    void unzipIntoFolder(std::string filename, std::string dest);

    zip_t* m_zip;
};