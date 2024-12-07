class AnimalDog : public Animal {
   public:
    AnimalDog(const std::vector<Point>& path)
        : Animal{path, AnimalType::dog} {
        HP = 25;                                                      // 設定生命值
        v = 40;                                                       // 設定移動速度
        money = 20;                                                   // 設定擊殺後獲得的金錢數量
        Bad_Good = false;                                             // 設定屬性，false 為惡
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // LEFT
        bitmap_img_ids.emplace_back(std::vector<int>({0, 1, 2, 3}));  // RIGHT
        bitmap_switch_freq = 20;                                      // 設定幀數
    }
};