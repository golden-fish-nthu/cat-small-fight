#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED

#include <tuple>
#include <utility>
#include <vector>
#include "./shapes/Rectangle.h"

/**
 * @brief The class manages data of each level.
 * @details The class could load level with designated input file and record. The level itself will decide when to create next monster.
 * @see DataCenter::level
 */
class Level {
   public:
    Level() {}
    void init();
    void load_level(int lvl);
    void update();
    void draw();
    bool is_onroad(const Rectangle& region);
    Rectangle grid_to_region(const Point& grid) const;
    int get_level_width() const;  // 添加這個函數的聲明
    const std::vector<Point>& get_road_path() const { return road_path; }
    int remain_monsters() const {
        int res = 0;
        for (const int& i : num_of_monsters)
            res += i;
        return res;
    }

   private:
    std::vector<Point> road_path;
    int level;
    int grid_w;
    int grid_h;
    int monster_spawn_counter;
    std::vector<int> num_of_monsters;
};

#endif
