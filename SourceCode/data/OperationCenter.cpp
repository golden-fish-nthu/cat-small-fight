#include "OperationCenter.h"
#include <algorithm>
#include "../Player.h"
#include "../hero.h"
#include "../monsters/Monster.h"
#include "../towers/Bullet.h"
#include "../towers/Tower.h"
#include "DataCenter.h"
void OperationCenter::update() {
    // 更新怪物。
    _update_monster();
    // 更新塔。
    _update_tower();
    // 更新塔的子彈。
    _update_towerBullet();
    // 如果任何子彈與任何怪物重疊，我們刪除子彈，減少怪物的HP，並在必要時刪除怪物。
    _update_monster_towerBullet();
    // 如果任何怪物到達終點，傷害玩家並刪除怪物。
    _update_monster_player();

    _update_monster_collision();
}

void OperationCenter::_update_monster() {
    std::vector<Monster*>& monsters = DataCenter::get_instance()->monsters;
    for (Monster* monster : monsters)
        monster->update();
}

void OperationCenter::_update_tower() {
    std::vector<Tower*>& towers = DataCenter::get_instance()->towers;
    for (Tower* tower : towers)
        tower->update();
}

void OperationCenter::_update_towerBullet() {
    std::vector<Bullet*>& towerBullets = DataCenter::get_instance()->towerBullets;
    for (Bullet* towerBullet : towerBullets)
        towerBullet->update();
    // 檢測子彈是否飛得太遠（超過其飛行距離限制），這意味著子彈的生命周期已結束。
    for (size_t i = 0; i < towerBullets.size(); ++i) {
        if (towerBullets[i]->get_fly_dist() <= 0) {
            towerBullets.erase(towerBullets.begin() + i);
            --i;
        }
    }
}

void OperationCenter::_update_monster_towerBullet() {
    DataCenter* DC = DataCenter::get_instance();
    std::vector<Monster*>& monsters = DC->monsters;
    std::vector<Bullet*>& towerBullets = DC->towerBullets;
    for (size_t i = 0; i < monsters.size(); ++i) {
        for (size_t j = 0; j < towerBullets.size(); ++j) {
            // 檢查子彈是否與怪物重疊。
            if (monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
                // 減少怪物的HP。刪除子彈。
                monsters[i]->HP -= towerBullets[j]->get_dmg();
                towerBullets.erase(towerBullets.begin() + j);
                --j;
            }
        }
    }
}

void OperationCenter::_update_monster_player() {
    DataCenter* DC = DataCenter::get_instance();
    std::vector<Monster*>& monsters = DC->monsters;
    Player*& player = DC->player;
    for (size_t i = 0; i < monsters.size(); ++i) {
        // 檢查怪物是否被殺死。
        if (monsters[i]->HP <= 0) {
            monsters[i]->is_dead = true;
            // 怪物被殺死。玩家獲得金錢。
            player->coin += monsters[i]->get_money();
            monsters.erase(monsters.begin() + i);
            --i;
            // 由於當前怪物被殺死，我們可以直接進入下一個怪物。
            break;
        }
        // 檢查怪物是否到達終點。
        if (monsters[i]->get_path().empty()) {
            monsters.erase(monsters.begin() + i);
            --i;
        }
    }
}

void OperationCenter::draw() {
    _draw_monster();
    _draw_tower();
    _draw_towerBullet();
}

void OperationCenter::_draw_monster() {
    std::vector<Monster*>& monsters = DataCenter::get_instance()->monsters;
    for (Monster* monster : monsters)
        monster->draw();
}

void OperationCenter::_draw_tower() {
    std::vector<Tower*>& towers = DataCenter::get_instance()->towers;
    for (Tower* tower : towers)
        tower->draw();
}

void OperationCenter::_draw_towerBullet() {
    std::vector<Bullet*>& towerBullets = DataCenter::get_instance()->towerBullets;
    for (Bullet* towerBullet : towerBullets)
        towerBullet->draw();
}

void OperationCenter::_update_monster_collision() {
    std::vector<Monster*>& monsters = DataCenter::get_instance()->monsters;
    for (size_t i = 0; i < monsters.size(); ++i) {
        for (size_t j = i + 1; j < monsters.size(); ++j) {
            if (monsters[i]->shape->overlap(*(monsters[j]->shape)) && monsters[i]->way != monsters[j]->way) {
                // 相減對方的HP
                int damage1 = monsters[i]->HP;
                int damage2 = monsters[j]->HP;
                monsters[i]->HP -= damage2;
                monsters[j]->HP -= damage1;
            }
        }
    }
    // 移除死亡的怪物
    monsters.erase(std::remove_if(monsters.begin(), monsters.end(),
                                  [](Monster* monster) { return monster->is_dead; }),
                   monsters.end());
}