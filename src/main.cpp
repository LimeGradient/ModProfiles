#include <fstream>
#include <iterator>
#include <numeric>

#include <Geode/Geode.hpp>
#include <Geode/utils/string.hpp>

#include <matjson.hpp>

using namespace geode::prelude;

namespace fs = std::filesystem;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
			this,
			menu_selector(MyMenuLayer::onMyButton)
		);

		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(myButton);

		myButton->setID("my-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMyButton(CCObject*) {
		geode::createQuickPopup("Mod Profiles", "Select your option", "Import Profile", "Export Profile", [&](bool btn1, bool btn2) {
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

		file::FilePickOptions::Filter filter = {
			"ModProfiles",
			{".modprofile"},
		};
		file::FilePickOptions options = {
			.filters = {filter},
		};

		std::ofstream out;  // Clear the file first
		out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ofstream::out | std::ofstream::trunc);
		out.close();

		out.open(fmt::format("{}/profile.txt", geode::dirs::getModConfigDir()), std::ios::app);
		
		for (std::string link : repoLinks) {
			out << link << "\n";
		}

		out.close();

		file::pickFile(file::PickMode::SaveFile, options, [](ghc::filesystem::path result) {
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
		}, []() {
			FLAlertLayer::create("Mod Profiles", "Failed to create profile\nError while moving file.", "Ok")->show();
		});
	}

	void importMods() {

	}
};

