#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED

#include <allegro5/bitmap.h>
#include <tuple>
#include <vector>
#include "./shapes/Point.h"
// #include "data/DataCenter.h"

class UI {
   public:
    UI() {}
    void init();
    void update();
    void draw();

   private:
    enum class STATE {
        HALT,    // -> HOVER
        HOVER,   // -> HALT, SELECT
        SELECT,  // -> HALT, PLACE
        PLACE    // -> HALT
    };
    STATE state;
    ALLEGRO_BITMAP* love;
    // tower menu bitmap, (top-left x, top-left y), price
    std::vector<std::tuple<ALLEGRO_BITMAP*, Point, int>> tower_items;
    // // cat menu bitmap, (top-left x, top-left y), price
    // std::vector<std::tuple<ALLEGRO_BITMAP*, Point, int>> monster_items;
    int on_item;
};

#endif
