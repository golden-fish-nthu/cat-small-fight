#ifndef MONSTERWOLFKNIGHT_H_INCLUDED
#define MONSTERWOLFKNIGHT_H_INCLUDED

#include "Monster.h"

// fixed settings: MonsterWolfKnight attributes
class MonsterWolfKnight : public Monster {
   private:
    int bitmap_switch_freq;

   public:
    MonsterWolfKnight(const std::vector<Point>& path, bool way)
        : Monster{path, MonsterType::WOLFKNIGHT, way} {
        HP = 40;
        v = 130;
        money = 80;
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // UP
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // DOWN
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // LEFT
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // RIGHT
        bitmap_switch_freq = 20;
    }
};

#endif
