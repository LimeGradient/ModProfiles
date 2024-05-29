#include <fstream>
#include <iterator>
#include <numeric>

#include <Geode/Geode.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/web.hpp>

#include <matjson.hpp>

using namespace geode::prelude;

namespace fs = std::filesystem;

#include <Geode/modify/MenuLayer.hpp>
class $modify(ModProfilesMenuLayer, MenuLayer) {
	struct Fields {
		file::FilePickOptions::Filter m_filter = {
			"Mod Profile",
			{".profile"},
		};
		file::FilePickOptions m_options = {
			.filters = {m_filter},
		};
	};

	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::create("mod-profiles-button.png"_spr),
			this,
			menu_selector(ModProfilesMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		geode::createQuickPopup("Mod Profiles", "Select your option\nImport Profile: Downloads all mods from a Profile\nExport Profile: Creates a Profile from installed mods", "Import Profile", "Export Profile", [&](bool btn1, bool btn2) {			
			if (btn1) importMods();
			if (btn2) getAllMods();
		});
	}

	void getAllMods() {
		std::vector<std::string> repoLinks;
		std::string profileFilePath;

		for (const auto& unzippedMod : fs::directory_iterator(geode::dirs::getModRuntimeDir().c_str())) {
			std::ifstream modJSONFile(fmt::format("{}/mod.json", unzippedMod.path()));
			std::string buffer ((std::istreambuf_iterator<char>(modJSONFile)),
								(std::istreambuf_iterator<char>()));

			if (buffer == "") continue;

			auto modJSON = matjson::parse(buffer);
			if (modJSON.contains("repository")) {
				log::info("{}", modJSON["repository"].as_string());
				repoLinks.push_back(modJSON["repository"].as_string());
			} else {
				continue;
			}
		}

		std::ofstream out;  // Clear the file first
		out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ofstream::out | std::ofstream::trunc);
		out.close();

		out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ios::app);
		
		for (std::string link : repoLinks) {
			out << link << "\n";
		}

		out.close();

		file::pickFile(file::PickMode::SaveFile, m_fields->m_options, [](ghc::filesystem::path result) {
			auto path = fs::path(result.c_str());
			#ifdef GEODE_IS_WINDOWS
			auto strPath = geode::utils::string::wideToUtf8(result.c_str());
			#else
			std::string strPath = result.c_str();
			#endif

			try {
				fs::rename(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), fmt::format("{}.profile", strPath));
			} catch (fs::filesystem_error& e) {
				FLAlertLayer::create("Mod Profiles", fmt::format("Failed to create profile\nError: {}", e.what()), "Ok")->show();
			}

			geode::Notification::create("Success! Created Profile!", geode::NotificationIcon::Success);
		}, []() {
			FLAlertLayer::create("Mod Profiles", "Failed to create profile\nError while moving file.", "Ok")->show();
		});
	}

	void importMods() {
		std::vector<std::string> repoLinks;

		file::pickFile(file::PickMode::OpenFile, m_fields->m_options, [&](ghc::filesystem::path result) {
			auto path = fs::path(result.c_str());
			#ifdef GEODE_IS_WINDOWS
			auto strPath = geode::utils::string::wideToUtf8(result.c_str());
			#else
			std::string strPath = result.c_str();
			#endif

			if (!strPath.ends_with(".profile")) {
				FLAlertLayer::create("Mod Profiles", "Failed to import profile\nWrong file type. File should end with .profile", "Ok")->show();
				return;
			}

			try {
				fs::rename(strPath, fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()));
			} catch (fs::filesystem_error& e) {
				FLAlertLayer::create("Mod Profiles", fmt::format("Failed to import profile\nError: {}", e.what()), "Ok")->show();
				return;
			}
		}, [=]() {
			FLAlertLayer::create("Mod Profiles", "Failed to import profile\nError with file picking", "Ok")->show();
			return;
		});

		if (fs::exists(fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()))) {
			std::ifstream profile(fmt::format("{}/imported_profile.txt", geode::dirs::getModConfigDir()));
			std::string line;
			if (profile.is_open()) {
				while (std::getline(profile, line)) {
					repoLinks.push_back(line);
				}
				profile.close();
			} else {
				log::info("Unable to open Profile");
				return;
			}
			
			geode::Notification::create("Downloading Mods from Profile...", geode::NotificationIcon::Loading);
			for (std::string link : repoLinks) {
				link.erase(link.begin(), std::next(link.begin(), 19));
				std::string apiLink = fmt::format("https://api.github.com/repos/{}/releases/latest", link);
				web::AsyncWebRequest()
					.userAgent("ModProfiles")
					.fetch(apiLink)
					.text()
					.then([](std::string const& value) {
						auto json = matjson::parse(value);
						log::info("Latest Release Download URL: {}", json["assets"][0]["browser_download_url"].as_string());

						auto modFilePath = fmt::format("{}/{}", geode::dirs::getModsDir(), json["assets"][0]["name"].as_string());
						
						if (modFilePath.find(".geode") == std::string::npos) {
							return;
						}

						if (!fs::exists(modFilePath)) {
							web::fetchFile(json["assets"][0]["browser_download_url"].as_string(), modFilePath);
						} else {
							return;
						}

						geode::createQuickPopup("Mod Profiles", "All mods installed successfully.\nPress restart to load mods.", "Restart", "Ok", [&](bool btn1, bool btn2) {
							if (!btn2) geode::utils::game::restart();
						});

					})
					.expect([](std::string const& error) {
						FLAlertLayer::create("Mod Profiles", fmt::format("Error while downloading mods\nError: {}", error), "Ok")->show();
					});
			}
		}
	}
};

