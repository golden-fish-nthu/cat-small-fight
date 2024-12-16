#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED

#include <queue>
#include <vector>
#include "../Object.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"

enum class Dir;

// fixed settings
enum class MonsterType {
    WOLF,
    CAVEMAN,
    WOLFKNIGHT,
    DEMONNIJIA,
    MONSTERTYPE_MAX
};

class Monster : public Object {
   public:
    static Monster* create_monster(MonsterType type, const std::vector<Point>& path, bool way);

   public:
    Monster(const std::vector<Point>& path, MonsterType type, bool way);
    void update();
    void draw();
    const int& get_money() const { return money; }
    int get_position() const;
    int HP;
    bool is_dead = false;
    const std::queue<Point>& get_path() const { return path; }
    int v;
    int money;
    std::vector<std::vector<int>> bitmap_img_ids;
    int bitmap_switch_counter;
    int bitmap_switch_freq;
    int bitmap_img_id;
    bool way;

   private:
    MonsterType type;
    Dir dir;
    std::queue<Point> path;
};

#endif  // MONSTER_H_INCLUDED
