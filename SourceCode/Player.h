#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Player {
   public:
    Player();
    void update();
    int HP;
    int coin;
    int boss_hp;

   private:
    int coin_freq;
    int coin_increase;
    int coin_counter;
};

#endif
