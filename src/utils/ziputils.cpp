#include "ziputils.h"

int getIndex(std::vector<std::string> vec, std::string element) {
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end()) {
        int index = it - vec.begin();
        return index;
    }
    else {
        return -1;
    }
}

void Zip::zipFiles(std::string zipPath, std::vector<std::string> files, std::vector<std::string> modNames) {
    struct zip_t* zip = zip_open(fmt::format("{}.zip", zipPath).c_str(), 9, 'w');
    {
        for (auto file : files) {
            log::info("zipping file: {}", file);
            zip_entry_open(zip, modNames.at(getIndex(files, file)).c_str());
            zip_entry_fwrite(zip, file.c_str());
            zip_entry_close(zip);
        }
    }
    zip_close(zip);
}