#ifndef MONSTERWOLF_H_INCLUDED
#define MONSTERWOLF_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterWolf attributes
class MonsterWolf : public Monster {
   public:
    MonsterWolf(const std::vector<Point>& path, bool way)
        : Monster{path, MonsterType::WOLF, way} {
        HP = 10;
        v = 60;
        money = 40;
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));     // UP
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));     // DOWN
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3, 4}));  // LEFT
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3, 4}));  // RIGHT
        bitmap_switch_freq = 20;
    }
};

#endif
