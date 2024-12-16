#include "Game.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <cstring>
#include <vector>
#include "Level.h"
#include "Player.h"
#include "Utils.h"
// #include "animal/animal.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "hero.h"
#include "monsters/Monster.h"
// 固定设置
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_img_path[] = "./assets/image/StartBackground.jpg";
constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg";

/**
 * @brief 游戏入口。
 * @details 该函数处理所有 Allegro 事件，并将事件状态更新到通用数据存储（即 DataCenter）。
 * 对于计时器事件，只有当当前是计时器时才会调用 game_update 和 game_draw 函数。
 */
void Game::execute() {
    DataCenter* DC = DataCenter::get_instance();
    // 主游戏循环
    bool run = true;
    while (run) {
        // 在此处理所有事件
        al_wait_for_event(event_queue, &event);
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER: {
                run &= game_update();
                game_draw();
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {  // 停止游戏
                run = false;
                break;
            }
            case ALLEGRO_EVENT_KEY_DOWN: {
                DC->key_state[event.keyboard.keycode] = true;
                // 處理數字鍵
                if (event.keyboard.keycode >= ALLEGRO_KEY_1 && event.keyboard.keycode <= ALLEGRO_KEY_4) {
                    double current_time = al_get_time();                       // 獲取當前時間
                    if (current_time - last_summon_time >= summon_cooldown) {  // 檢查冷卻時間是否已過
                        int monster_type = event.keyboard.keycode - ALLEGRO_KEY_1;
                        int monster_cost = 50 * (monster_type + 1);  // 每个怪物的成本是 50 金币乘上按的数字键
                        printf("monster_cost: %d\n", monster_cost);
                        if (DC->player->coin >= monster_cost) {
                            DC->player->coin -= monster_cost;
                            DC->monsters.emplace_back(Monster::create_monster(static_cast<MonsterType>(monster_type),
                                                                              DC->level->get_road_path(), true));
                            debug_log("Generated monster type %d\n", monster_type);
                            debug_log("Player's coin: %d\n", DC->player->coin);
                            last_summon_time = current_time;  // 更新上次召喚時間
                        } else {
                            debug_log("Player's coin: %d\n", DC->player->coin);
                            debug_log("Not enough money to generate monster type %d\n", monster_type);
                        }
                    } else {
                        debug_log("Summon on cooldown. Please wait.\n");
                    }
                }
                break;
            }
            case ALLEGRO_EVENT_KEY_UP: {
                DC->key_state[event.keyboard.keycode] = false;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_AXES: {
                DC->mouse.x = event.mouse.x;
                DC->mouse.y = event.mouse.y;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
                DC->mouse_state[event.mouse.button] = true;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                DC->mouse_state[event.mouse.button] = false;
                break;
            }
            default:
                break;
        }
    }
}

/**
 * @brief 初始化所有 Allegro 插件和游戏主体。
 * @details 只创建一个计时器，因为游戏及其所有数据都应该同步处理。
 */
Game::Game() {
    DataCenter* DC = DataCenter::get_instance();
    GAME_ASSERT(al_init(), "failed to initialize allegro.");

    // 初始化 Allegro 插件
    bool addon_init = true;
    addon_init &= al_init_primitives_addon();
    addon_init &= al_init_font_addon();
    addon_init &= al_init_ttf_addon();
    addon_init &= al_init_image_addon();
    addon_init &= al_init_acodec_addon();
    GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

    // 初始化事件
    bool event_init = true;
    event_init &= al_install_keyboard();
    event_init &= al_install_mouse();
    event_init &= al_install_audio();
    GAME_ASSERT(event_init, "failed to initialize allegro events.");

    // 初始化游戏主体
    GAME_ASSERT(display = al_create_display(DC->window_width, DC->window_height), "failed to create display.");
    GAME_ASSERT(timer = al_create_timer(1.0 / DC->FPS), "failed to create timer.");
    GAME_ASSERT(event_queue = al_create_event_queue(), "failed to create event queue.");

    debug_log("Game initialized.\n");
    game_init();
}

/**
 * @brief 初始化所有辅助资源。
 */
void Game::game_init() {
    DataCenter* DC = DataCenter::get_instance();
    SoundCenter* SC = SoundCenter::get_instance();
    ImageCenter* IC = ImageCenter::get_instance();
    FontCenter* FC = FontCenter::get_instance();
    // 设置窗口图标
    game_icon = IC->get(game_icon_img_path);
    al_set_display_icon(display, game_icon);
    summon_cooldown = 0.3;                // 設置冷卻時間為 0.3秒
    last_summon_time = -summon_cooldown;  // 初始化為負冷卻時間，確保遊戲開始時可以立即召喚

    // 注册事件到事件队列
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // 初始化声音设置
    SC->init();
    // 初始化字体设置
    FC->init();
    ui = new UI();
    ui->init();
    DC->level->init();
    DC->player = new Player();
    DC->player->HP = 100;
    nlvl = 1;
    // 游戏开始
    background = IC->get(background_img_path);
    debug_log("Game state: change to START\n");
    state = STATE::START;
    al_start_timer(timer);
}

/**
 * @brief 该函数处理所有数据更新。
 * @details 整个游戏主体的行为由其状态决定。
 * @return 游戏是否应该继续运行（true）或达到终止条件（false）。
 * @see Game::STATE
 */

bool Game::game_update() {
    DataCenter* DC = DataCenter::get_instance();
    OperationCenter* OC = OperationCenter::get_instance();
    SoundCenter* SC = SoundCenter::get_instance();
    static ALLEGRO_SAMPLE_INSTANCE* background = nullptr;

    switch (state) {
        static bool is_played = false;

        case STATE::START: {
            static ALLEGRO_SAMPLE_INSTANCE* instance = nullptr;
            if (!is_played) {
                instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);

                DC->level->load_level(nlvl);
                is_played = true;
            }

            if (!SC->is_playing(instance)) {
                debug_log("<Game> state: change to LEVEL\n");
                state = STATE::LEVEL;
            }
            break;
        }
        case STATE::LEVEL: {
            static bool BGM_played = false;
            if (DC->key_state[ALLEGRO_KEY_L] && !DC->prev_key_state[ALLEGRO_KEY_L]) {
                debug_log("<Game> state: change to WIN\n");
                state = STATE::WIN;
            }
            if (!BGM_played) {
                background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
                BGM_played = true;
            }

            if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
                SC->toggle_playing(background);
                debug_log("<Game> state: change to PAUSE\n");
                state = STATE::PAUSE;
            }
            if (DC->level->remain_monsters() == 0 || DC->player->boss_hp == 0) {
                if (nlvl < 2) {
                    nlvl++;
                    debug_log("<Game> state: change to START\n");
                    state = STATE::START;
                    is_played = false;
                } else {
                    debug_log("<Game> state: change to END\n");
                    state = STATE::WIN;
                }
            }
            if (DC->player->HP <= 0) {
                debug_log("<Game> state: change to END\n");
                state = STATE::LOSE;
            }
            break;
        }
        case STATE::PAUSE: {
            if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
                SC->toggle_playing(background);
                debug_log("<Game> state: change to LEVEL\n");
                state = STATE::LEVEL;
            }
            break;
        }
        case STATE::WIN: {
            if (DC->key_state[ALLEGRO_KEY_ESCAPE]) {
                return false;
            }
            break;
        }
        case STATE::LOSE: {
            if (DC->key_state[ALLEGRO_KEY_ESCAPE]) {
                return false;
            }
            break;
        }
    }
    // 如果游戏没有暂停，我们应该继续更新。
    if (state != STATE::PAUSE) {
        DC->player->update();
        SC->update();
        ui->update();
        if (state != STATE::START) {
            DC->level->update();
            OC->update();
        }
    }
    // game_update 完成。当前帧的状态将成为下一帧的前一状态。
    memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
    memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
    return true;
}

/**
 * @brief 绘制整个游戏和对象。
 */
void Game::game_draw() {
    DataCenter* DC = DataCenter::get_instance();
    OperationCenter* OC = OperationCenter::get_instance();
    FontCenter* FC = FontCenter::get_instance();

    // 首先清空屏幕。
    al_clear_to_color(al_map_rgb(100, 100, 100));
    if (state != STATE::WIN && state != STATE::LOSE) {
        // 背景
        al_draw_bitmap(background, 0, 0, 0);
        if (DC->game_field_length < DC->window_width)
            al_draw_filled_rectangle(DC->game_field_length, 0, DC->window_width, DC->window_height,
                                     al_map_rgb(100, 100, 100));
        if (DC->game_field_length < DC->window_height)
            al_draw_filled_rectangle(0, DC->game_field_length, DC->window_width, DC->window_height,
                                     al_map_rgb(100, 100, 100));
        // 用户界面
        if (state != STATE::START) {
            DC->level->draw();
            ui->draw();
            OC->draw();
        }
    }
    switch (state) {
        case STATE::START: {
        }
        case STATE::LEVEL: {
            break;
        }
        case STATE::PAUSE: {
            // 游戏布局覆盖
            al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(50, 50, 50, 64));
            al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255), DC->window_width / 2.,
                         DC->window_height / 2., ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
            break;
        }
        case STATE::WIN: {
            al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255), DC->window_width / 2.,
                         DC->window_height / 2., ALLEGRO_ALIGN_CENTRE, "YOU WIN!");
            break;
        }
        case STATE::LOSE: {
            al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255), DC->window_width / 2.,
                         DC->window_height / 2., ALLEGRO_ALIGN_CENTRE, "YOU LOSE!");
            break;
        }
    }
    al_flip_display();
}

Game::~Game() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}
