#include "Player.h"  // 包含 Player.h 文件

// 固定设置
namespace PlayerSetting {
constexpr int init_HP = 3;        // 初始生命值
constexpr int init_coin = 100;    // 初始金币
constexpr int coin_freq = 5;      // 金币频率
constexpr int coin_increase = 1;  // 金币增加量
constexpr int boss_hp = 200;      // boss 生命值
};  // namespace PlayerSetting

Player::Player()
    : HP(PlayerSetting::init_HP), coin(PlayerSetting::init_coin) {  // 构造函数，初始化生命值和金币
    this->coin_freq = PlayerSetting::coin_freq;                     // 初始化金币频率
    this->coin_increase = PlayerSetting::coin_increase;             // 初始化金币增加量
    coin_counter = PlayerSetting::coin_freq;                        // 初始化金币计数器
}

void Player::update() {  // 更新函数
    if (coin_counter)
        --coin_counter;            // 如果计数器不为零，递减计数器
    else {                         // 否则
        coin += coin_increase;     // 增加金币
        coin_counter = coin_freq;  // 重置计数器
    }
}
