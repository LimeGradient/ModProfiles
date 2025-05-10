#include <filesystem>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    if (std::filesystem::exists(dirs::getTempDir() / "profiles")) {
        for (auto file : std::filesystem::directory_iterator(dirs::getTempDir() / "profiles")) {
            std::filesystem::remove_all(file.path());
        }
    }
}