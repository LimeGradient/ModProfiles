#include "ModUtils.h"

bool ModUtils::isModCompatible(std::string modPath) {
    Zip* zip = new Zip("");
    auto filenames = zip->getAllFileNames(modPath);

    std::vector<std::string>::iterator it;
    #ifdef _WIN32
        it = std::find(filenames.begin(), filenames.end(), ".dll");
    #elif defined(__APPLE__)
        it = std::find(filenames.begin(), filenames.end(), ".dylib");
    #elif defined(__ANDROID__)
        it = std::find(filenames.begin(), filenames.end(), ".so");
    #endif

    if (it != filenames.end()) {
        return true;
    } else {
        return false;
    }
}