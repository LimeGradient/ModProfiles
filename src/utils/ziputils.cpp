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

Zip::Zip(std::string zipPath) {
    this->m_zip = zip_open(fmt::format("{}.geodepack", zipPath).c_str(), 9, 'w');
}

void Zip::zipFiles(std::vector<std::string> files, std::vector<std::string> modNames) {
    for (auto file : files) {
        zip_entry_open(this->m_zip, modNames.at(getIndex(files, file)).c_str());
        zip_entry_fwrite(this->m_zip, file.c_str());
        zip_entry_close(this->m_zip);
    }
}

void Zip::appendToZip(std::string filename, std::string file) {
    zip_entry_open(this->m_zip, filename.c_str());
    zip_entry_fwrite(this->m_zip, file.c_str());
    zip_entry_close(this->m_zip);
}

void Zip::writeStringToZip(std::string filename, std::string content) {
    zip_entry_open(this->m_zip, filename.c_str());
    zip_entry_write(this->m_zip, content.c_str(), strlen(content.c_str()));
    zip_entry_close(this->m_zip);
}

void Zip::close() {
    zip_close(this->m_zip);
}

void Zip::unzipIntoFolder(std::string filename, std::string dest) {
    int arg = 2;
    zip_extract(filename.c_str(), dest.c_str(), NULL, &arg);
}

std::string Zip::unzipFileIntoString(std::string archiveName, std::string filename) {
    void* buf = NULL;
    size_t bufsize;

    struct zip_t* zip = zip_open(archiveName.c_str(), 0, 'r'); 
    {
        zip_entry_open(zip, filename.c_str());
        {
            zip_entry_read(zip, &buf, &bufsize);
        }
        zip_entry_close(zip);
    }
    zip_close(zip);

    auto str = std::string((char*)buf);
    free(buf);
    return str;
}