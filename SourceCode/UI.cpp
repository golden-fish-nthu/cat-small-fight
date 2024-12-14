#include "UI.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>
#include "Level.h"
#include "Player.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "towers/Tower.h"
constexpr char love_img_path[] = "./assets/image/love.png";
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 30;  // 塔圖片左邊距
constexpr int tower_img_top_padding = 30;   // 塔圖片上邊距
constexpr int tower_img_bottom_padding = 120;

void UI::init() {
    DataCenter* DC = DataCenter::get_instance();
    ImageCenter* IC = ImageCenter::get_instance();
    // int tl_x = DC->game_field_length + tower_img_left_padding;
    // int tl_y = tower_img_top_padding;
    int tl_x = tower_img_left_padding;
    int tl_y = DC->window_height - tower_img_bottom_padding;
    int max_height = 0;
    // 安排塔商店
    for (size_t i = 0; i < (size_t)(TowerType::TOWERTYPE_MAX); ++i) {
        ALLEGRO_BITMAP* bitmap = IC->get(TowerSetting::tower_menu_img_path[i]);
        int w = al_get_bitmap_width(bitmap);
        int h = al_get_bitmap_height(bitmap);
        if (tl_x + w > DC->window_width) {
            tl_x = DC->game_field_length + tower_img_left_padding;
            tl_y += max_height + tower_img_top_padding;
            max_height = 0;
        }
        tower_items.emplace_back(bitmap, Point{tl_x, tl_y}, TowerSetting::tower_price[i]);
        tl_x += w + tower_img_left_padding;
        max_height = std::max(max_height, h);
    }
    debug_log("<UI> state: change to HALT\n");
    state = STATE::HALT;
    on_item = -1;
}

void UI::update() {
    DataCenter* DC = DataCenter::get_instance();
    const Point& mouse = DC->mouse;

    switch (state) {
        case STATE::HALT: {
            // // 檢查是否懸停在商店塔項目上
            // for (size_t i = 0; i < tower_items.size(); ++i) {
            //     auto& [bitmap, p, price] = tower_items[i];
            //     int w = al_get_bitmap_width(bitmap);
            //     int h = al_get_bitmap_height(bitmap);
            //     // 懸停在商店塔項目上
            //     if (mouse.overlap(Rectangle{p.x, p.y, p.x + w, p.y + h})) {
            //         on_item = i;
            //         debug_log("<UI> state: change to HOVER\n");
            //         state = STATE::HOVER;
            //         break;
            //     }
            // }
            // break;
        }
        case STATE::HOVER: {
            // auto& [bitmap, p, price] = tower_items[on_item];
            // int w = al_get_bitmap_width(bitmap);
            // int h = al_get_bitmap_height(bitmap);
            // if (!mouse.overlap(Rectangle{p.x, p.y, p.x + w, p.y + h})) {
            //     on_item = -1;
            //     debug_log("<UI> state: change to HALT\n");
            //     state = STATE::HALT;
            //     break;
            // }
            // // 點擊滑鼠左鍵
            // if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
            //     // 沒有足夠的金錢
            //     if (price > DC->player->coin) {
            //         debug_log("<UI> Not enough money to buy tower %d.\n", on_item);
            //         break;
            //     }
            //     debug_log("<UI> state: change to SELECT\n");
            //     state = STATE::SELECT;
            // }
            // break;
        }
        case STATE::SELECT: {
            // // 點擊滑鼠左鍵：放置
            // if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
            //     debug_log("<UI> state: change to PLACE\n");
            //     state = STATE::PLACE;
            // }
            // // 點擊滑鼠右鍵：取消
            // if (DC->mouse_state[2] && !DC->prev_mouse_state[2]) {
            //     on_item = -1;
            //     debug_log("<UI> state: change to HALT\n");
            //     state = STATE::HALT;
            // }
            // break;
        }
        case STATE::PLACE: {
            // // 檢查放置合法性
            // ALLEGRO_BITMAP* bitmap = Tower::get_bitmap(static_cast<TowerType>(on_item));
            // int w = al_get_bitmap_width(bitmap);
            // int h = al_get_bitmap_height(bitmap);
            // Rectangle place_region{mouse.x - w / 2, mouse.y - h / 2, DC->mouse.x + w / 2, DC->mouse.y + h / 2};
            // bool place = true;
            // // 塔不能放置在道路上
            // place &= (!DC->level->is_onroad(place_region));
            // // 塔不能與其他塔重疊
            // for (Tower* tower : DC->towers) {
            //     place &= (!place_region.overlap(tower->get_region()));
            // }
            // if (!place) {
            //     debug_log("<UI> Tower place failed.\n");
            // } else {
            //     DC->towers.emplace_back(Tower::create_tower(static_cast<TowerType>(on_item), mouse));
            //     DC->player->coin -= std::get<2>(tower_items[on_item]);
            // }
            // debug_log("<UI> state: change to HALT\n");
            // state = STATE::HALT;
            // break;
        }
    }
}

void UI::draw() {
    DataCenter* DC = DataCenter::get_instance();
    FontCenter* FC = FontCenter::get_instance();
    const Point& mouse = DC->mouse;
    // 繪製 HP
    const int& game_field_length = DC->game_field_length;
    const int& player_HP = DC->player->HP;

	// int love_width = al_get_bitmap_width(love);
	// for(int i = 1; i <= player_HP; ++i) {
	// 	al_draw_bitmap(love, game_field_length - (love_width + love_img_padding) * i, love_img_padding, 0);
	// }
	
    // 繪製金幣
    const int& player_coin = DC->player->coin;
	al_draw_textf(
		FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
		game_field_length - 400, DC->window_height - tower_img_bottom_padding,
		ALLEGRO_ALIGN_LEFT, "coin: %5d", player_coin);
    // 繪製塔商店項目
    for (auto& [bitmap, p, price] : tower_items) {
        int w = al_get_bitmap_width(bitmap);
        int h = al_get_bitmap_height(bitmap);
        al_draw_bitmap(bitmap, p.x, p.y, 0);
        al_draw_rectangle(
            p.x - 1, p.y - 1,
            p.x + w + 1, p.y + h + 1,
            al_map_rgb(0, 0, 0), 1);
        if (price > DC->player->coin) {
                // 金額不足，顯示半透明遮罩
                al_draw_filled_rectangle(p.x, p.y, p.x + w, p.y + h, al_map_rgba(50, 50, 50, 128));
            }
        al_draw_textf(
            FC->courier_new[FontSize::MEDIUM], al_map_rgb(0, 0, 0),
            p.x + w / 2, p.y + h,
            ALLEGRO_ALIGN_CENTRE, "%d", price);
    }

    switch (state) {
        static Tower* selected_tower = nullptr;
        case STATE::HALT: {
            // HALT 狀態下不應選擇任何塔。
            if (selected_tower != nullptr) {
                delete selected_tower;
                selected_tower = nullptr;
            }
            break;
        }
        case STATE::HOVER: {
            auto& [bitmap, p, price] = tower_items[on_item];
            int w = al_get_bitmap_width(bitmap);
            int h = al_get_bitmap_height(bitmap);
            // 創建一個半透明的遮罩覆蓋在懸停的塔上。
            al_draw_filled_rectangle(p.x, p.y, p.x + w, p.y + h, al_map_rgba(50, 50, 50, 64));
            break;
        }
        case STATE::SELECT: {
            // 如果選擇了一個塔，我們需要新建一個對應的塔來預覽。
            if (selected_tower == nullptr) {
                selected_tower = Tower::create_tower(static_cast<TowerType>(on_item), mouse);
            } else {
                selected_tower->shape->update_center_x(mouse.x);
                selected_tower->shape->update_center_y(mouse.y);
            }
        }
        case STATE::PLACE: {
            // 如果我們從菜單中選擇了一個塔，我們需要預覽塔將被建造的位置及其攻擊範圍。
            ALLEGRO_BITMAP* bitmap = Tower::get_bitmap(static_cast<TowerType>(on_item));
            al_draw_filled_circle(mouse.x, mouse.y, selected_tower->attack_range(), al_map_rgba(255, 0, 0, 32));
            int w = al_get_bitmap_width(bitmap);
            int h = al_get_bitmap_height(bitmap);
            al_draw_bitmap(bitmap, mouse.x - w / 2, mouse.y - h / 2, 0);
            break;
        }
    }
}
