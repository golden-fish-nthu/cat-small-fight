#include "Monster.h"
#include <allegro5/allegro_primitives.h>
#include "../Level.h"
#include "../Player.h"
#include "../Utils.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include "MonsterCaveMan.h"
#include "MonsterDemonNinja.h"
#include "MonsterWolf.h"
#include "MonsterWolfKnight.h"

using namespace std;

// 固定設定
enum class Dir {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
namespace MonsterSetting {
static constexpr char monster_imgs_root_path[static_cast<int>(MonsterType::MONSTERTYPE_MAX)][40] = {
    "./assets/image/monster/Wolf",
    "./assets/image/monster/CaveMan",
    "./assets/image/monster/WolfKnight",
    "./assets/image/monster/DemonNinja"};
static constexpr char dir_path_prefix[][10] = {
    "UP", "DOWN", "LEFT", "RIGHT"};
}  // namespace MonsterSetting

/**
 * @brief 根據怪物類型創建一個 Monster* 實例。
 * @param type 怪物的類型。
 * @param path 怪物的行走路徑。路徑應該以道路網格格式表示。
 * @return 對應的 Monster* 實例。
 * @see Level::grid_to_region(const Point &grid) const
 */
Monster* Monster::create_monster(MonsterType type, const vector<Point>& path) {
    switch (type) {
        case MonsterType::WOLF: {
            return new MonsterWolf{path};
        }
        case MonsterType::CAVEMAN: {
            return new MonsterCaveMan{path};
        }
        case MonsterType::WOLFKNIGHT: {
            return new MonsterWolfKnight{path};
        }
        case MonsterType::DEMONNIJIA: {
            return new MonsterDemonNinja{path};
        }
        case MonsterType::MONSTERTYPE_MAX: {
        }
    }
    GAME_ASSERT(false, "monster type error.");
}

/**
 * @brief 根據 x 和 y 方向的速度，確定怪物應該面向的方向。
 */
Dir convert_dir(const Point& v) {
    if (v.y < 0 && abs(v.y) >= abs(v.x))
        return Dir::UP;
    if (v.y > 0 && abs(v.y) >= abs(v.x))
        return Dir::DOWN;
    if (v.x < 0 && abs(v.x) >= abs(v.y))
        return Dir::LEFT;
    if (v.x > 0 && abs(v.x) >= abs(v.y))
        return Dir::RIGHT;
    return Dir::RIGHT;
}

Monster::Monster(const vector<Point>& path, MonsterType type) {
    DataCenter* DC = DataCenter::get_instance();
    shape.reset(new Rectangle{0, 0, 0, 0});
    this->type = type;
    dir = Dir::LEFT;
    bitmap_img_id = 0;
    bitmap_switch_counter = 0;
    for (const Point& p : path)
        this->path.push(p);
    if (!path.empty()) {
        const Point& grid = this->path.front();
        const Rectangle& region = DC->level->grid_to_region(grid);
        // 暫時將邊界框設置為中心（無區域），因為我們還沒有獲取怪物的碰撞箱。
        // shape.reset(new Rectangle{region.center_x(), region.center_y(), region.center_x(), region.center_y()});
        shape.reset(new Rectangle(DC->game_field_length - 50,  // 使用 game_field_length 而不是 window_width
                                  DC->window_height / 2,
                                  DC->game_field_length,  // 使用 game_field_length 而不是 window_width
                                  DC->window_height / 2 + 50));
        this->path.pop();
    }
}

/**
 * @brief 更新怪物狀態
 *
 * 如果怪物已經死亡，則直接返回。
 *
 * 1. 更新動畫幀：
 *    - 如果 bitmap_switch_counter 不為零，則遞減它。
 *    - 否則，更新 bitmap_img_id 並重置 bitmap_switch_counter。
 *
 * 2. 設置方向為始終向左。
 *
 * 3. 以相同速度向左移動：
 *    - 根據 DataCenter 的 FPS 計算移動距離。
 *    - 更新形狀的中心 x 坐標。
 *
 * 4. 更新碰撞箱：
 *    - 根據當前動畫幀生成圖像路徑。
 *    - 從 ImageCenter 獲取對應的位圖。
 *    - 根據位圖的寬高計算新的碰撞箱。
 *    - 創建新的 Rectangle 對象並更新形狀。
 */
void Monster::update() {
    if (is_dead)
        return;
    DataCenter* DC = DataCenter::get_instance();
    ImageCenter* IC = ImageCenter::get_instance();

    // Update animation frame
    if (bitmap_switch_counter)
        --bitmap_switch_counter;
    else {
        bitmap_img_id = (bitmap_img_id + 1) % (bitmap_img_ids[static_cast<int>(dir)].size());
        bitmap_switch_counter = bitmap_switch_freq;
    }

    // Set direction to always right
    dir = Dir::LEFT;

    // Move right with the same speed
    double movement = v / DC->FPS;
    shape->update_center_x(shape->center_x() - movement);

    // Update collision box
    char buffer[50];
    sprintf(
        buffer, "%s/%s_%d.png",
        MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
        MonsterSetting::dir_path_prefix[static_cast<int>(dir)],
        bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]);
    ALLEGRO_BITMAP* bitmap = IC->get(buffer);
    const double& cx = shape->center_x();
    const double& cy = shape->center_y();
    const int& h = al_get_bitmap_width(bitmap) * 0.8;
    const int& w = al_get_bitmap_height(bitmap) * 0.8;
    shape.reset(new Rectangle{
        (cx - w / 2.), (cy - h / 2.),
        (cx - w / 2. + w), (cy - h / 2. + h)});
}

void Monster::draw() {
    if (is_dead)
        return;
    ImageCenter* IC = ImageCenter::get_instance();
    char buffer[50];
    sprintf(
        buffer, "%s/%s_%d.png",
        MonsterSetting::monster_imgs_root_path[static_cast<int>(type)],
        MonsterSetting::dir_path_prefix[static_cast<int>(dir)],
        bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]);
    ALLEGRO_BITMAP* bitmap = IC->get(buffer);
    al_draw_bitmap(
        bitmap,
        shape->center_x() - al_get_bitmap_width(bitmap) / 2,
        shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}
