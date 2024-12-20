#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include <string>
#include <unordered_set>
#include "UI.h"
/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */
class Game {
   public:
    void execute();
    int nlvl = 1;
    double summon_cooldown;                 // 冷卻時間
    double last_summon_time;                // 上次召喚時間
    std::unordered_set<char> key_sequence;  // 用來跟踪按鍵序列
    ALLEGRO_BITMAP* homo_background;        // 用來保存背景圖片
    ALLEGRO_BITMAP* win_background;         // 用來保存背景圖片
    ALLEGRO_BITMAP* lose_background;        // 用來保存背景圖片
   public:
    Game();
    ~Game();
    void game_init();
    bool game_update();
    void game_draw();

   private:
    /**
     * @brief States of the game process in game_update.
     * @see Game::game_update()
     */
    enum class STATE {
        START,  // -> LEVEL
        LEVEL,  // -> PAUSE, END
        PAUSE,  // -> LEVEL
        WIN,
        LOSE,
        HOMO
    };
    STATE state;
    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP* game_icon;
    ALLEGRO_BITMAP* background;

   private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;
    UI* ui;
};

#endif
