#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include <filesystem>
#include "utils/PackInfo.h"
#include "utils/ziputils.h"
using namespace geode::prelude;
namespace fs = std::filesystem;
using FileTask = Task<Result<fs::path>>;

class PackSelectList : public CCNode {
protected:
    ScrollLayer* m_list;
    CCNode* m_topContainer;
    CCMenu* m_btnMenu;
    CCMenuItemSpriteExtra* m_importPackBtn;

    bool init(CCSize const& size);
    void getAllPacks();

    void onImportPack(CCObject*);

public:
    static PackSelectList* create(CCSize const& size);
};