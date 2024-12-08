#include "Level.h"
#include <allegro5/allegro_primitives.h>
#include <array>
#include <iostream>
#include <string>
#include "Utils.h"
#include "data/DataCenter.h"
#include "monsters/Monster.h"
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
using namespace std;

// 固定設定
namespace LevelSetting {
constexpr char level_path_format[] = "./assets/level/LEVEL%d.txt";
//! @brief 每個關卡的網格大小
constexpr array<int, 4> grid_size = {
    40, 40, 40, 40};
constexpr int monster_spawn_rate = 90;
};  // namespace LevelSetting

void Level::init() {
    level = -1;
    grid_w = -1;
    grid_h = -1;
    monster_spawn_counter = 0;
}

/**
 * @brief 從輸入文件加載關卡數據。輸入文件需要遵循特定格式。
 * @param lvl 關卡索引。路徑格式是代碼中的固定設置。
 * @details 輸入文件的內容應按以下格式排列：
 *          * 怪物總數。
 *          * 每種不同怪物的數量。順序和數量遵循 MonsterType 的定義。
 *          * 不定數量的點座標 (x, y)，以網格格式表示。
 * @see level_path_format
 * @see MonsterType
 */
void Level::load_level(int lvl) {
    DataCenter* DC = DataCenter::get_instance();

    char buffer[50];
    sprintf(buffer, LevelSetting::level_path_format, lvl);
    FILE* f = fopen(buffer, "r");
    GAME_ASSERT(f != nullptr, "cannot find level.");
    level = lvl;
    grid_w = DC->game_field_length / LevelSetting::grid_size[lvl];
    grid_h = DC->game_field_length / LevelSetting::grid_size[lvl];
    cout << grid_w << " " << grid_h << endl;
    num_of_monsters.clear();
    road_path.clear();

    int num;
    // 讀取怪物總數和每種怪物的數量
    fscanf(f, "%d", &num);
    for (size_t i = 0; i < static_cast<size_t>(MonsterType::MONSTERTYPE_MAX); ++i) {
        fscanf(f, "%d", &num);
        num_of_monsters.emplace_back(num);
    }

    // 忽略文件中的道路路徑，手動生成直線道路
    for (int i = 0; i < grid_h; ++i) {
        road_path.emplace_back(i, 10);  // x 固定為 0，y 從 0 遞增
    }

    fclose(f);
    debug_log("<Level> load level %d.\n", lvl);
}

/**
 * @brief 更新怪物生成計數器並在需要時創建怪物。
 */
void Level::update() {
    if (monster_spawn_counter) {
        monster_spawn_counter--;
        return;
    }
    DataCenter* DC = DataCenter::get_instance();

    for (size_t i = 0; i < num_of_monsters.size(); ++i) {
        if (num_of_monsters[i] == 0)
            continue;
        DC->monsters.emplace_back(Monster::create_monster(static_cast<MonsterType>(i), DC->level->get_road_path()));
        num_of_monsters[i]--;
        break;
    }
    monster_spawn_counter = LevelSetting::monster_spawn_rate;
}

void Level::draw() {
    if (level == -1)
        return;
    for (const auto& [i, j] : road_path) {
        int x1 = i * LevelSetting::grid_size[level];
        int y1 = j * LevelSetting::grid_size[level];
        int x2 = x1 + LevelSetting::grid_size[level];
        int y2 = y1 + LevelSetting::grid_size[level];
        al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 244, 173));
    }
}

bool Level::is_onroad(const Rectangle& region) {
    for (const auto& [i, j] : road_path) {
        int x1 = i * LevelSetting::grid_size[level];
        int y1 = j * LevelSetting::grid_size[level];
        int x2 = x1 + LevelSetting::grid_size[level];
        int y2 = y1 + LevelSetting::grid_size[level];
        Rectangle road_rect(x1, y1, x2, y2);
        if (road_rect.overlaps(region)) {
            return true;
        }
    }
    return false;
}
Rectangle Level::grid_to_region(const Point& grid) const {
    int x1 = grid.x * LevelSetting::grid_size[level];
    int y1 = grid.y * LevelSetting::grid_size[level];
    int x2 = x1 + LevelSetting::grid_size[level];
    int y2 = y1 + LevelSetting::grid_size[level];
    return Rectangle(x1, y1, x2, y2);
}