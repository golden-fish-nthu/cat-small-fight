#ifndef MONSTERDEMONNINJA_H_INCLUDED
#define MONSTERDEMONNINJA_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterDemonNinja attributes
class MonsterDemonNinja : public Monster {
   public:
    MonsterDemonNinja(const std::vector<Point>& path, bool way)
        : Monster{path, MonsterType::DEMONNIJIA, way} {
        HP = 50;
        v = 70;
        money = 140;
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // UP
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // DOWN
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // LEFT
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // RIGHT
        bitmap_switch_freq = 20;
    }
};

#endif
