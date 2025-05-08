#include <filesystem>
#include <Geode/Geode.hpp>

using namespace geode::prelude;

$on_mod(Loaded) {
    for (auto file : std::filesystem::directory_iterator(dirs::getTempDir() / "profiles")) {
        std::filesystem::remove_all(file.path());
    }
}