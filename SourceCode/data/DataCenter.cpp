#include "DataCenter.h"
#include <cstring>
#include "../Level.h"
#include "../Player.h"
// #include "../animal/animal.h"
#include "../hero.h"
#include "../monsters/Monster.h"
#include "../towers/Bullet.h"
#include "../towers/Tower.h"
// fixed settings
namespace DataSetting {
constexpr double FPS = 60;
constexpr int window_width = 1000; 
constexpr int window_height = 600;
constexpr int game_field_length = 1050;
}  // namespace DataSetting

DataCenter::DataCenter() {
    this->FPS = DataSetting::FPS;
    this->window_width = DataSetting::window_width;
    this->window_height = DataSetting::window_height;
    this->game_field_length = DataSetting::game_field_length;
    memset(key_state, false, sizeof(key_state));
    memset(prev_key_state, false, sizeof(prev_key_state));
    mouse = Point(0, 0);
    memset(mouse_state, false, sizeof(mouse_state));
    memset(prev_mouse_state, false, sizeof(prev_mouse_state));
    player = new Player();
    level = new Level();
}

DataCenter::~DataCenter() {
    delete player;
    delete level;
    // delete hero;

    for (Monster*& m : monsters) {
        delete m;
    }
    for (Tower*& t : towers) {
        delete t;
    }
    for (Bullet*& b : towerBullets) {
        delete b;
    }
}
