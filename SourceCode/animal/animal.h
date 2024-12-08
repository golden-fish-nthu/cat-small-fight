// #ifndef ANIMAL_H_INCLUDED
// #define ANIMAL_H_INCLUDED

// #include <queue>
// #include <vector>
// #include "../Object.h"
// #include "../shapes/Point.h"
// #include "../shapes/Rectangle.h"

// enum class Dir;

// // 固定设置
// enum class AnimalType {
//     dog,
//     // pig,
//     // lepord,
//     // man,
//     // spider,
//     AnimalTYPE_MAX
// };

// /**
//  * @brief 怪物类（敌人）。
//  * @details 怪物继承自 Object 并使用 Rectangle 作为其碰撞箱。
//  */
// class Animal : public Object {
//    public:
//     static Animal* create_Animal(AnimalType type, const std::vector<Point>& path);

//    public:
//     Animal(const std::vector<Point>& path, AnimalType type);
//     void update();
//     void draw();
//     const int& get_money() const { return money; }
//     const std::queue<Point>& get_path() const { return path; }

//    protected:
//     /**
//      * @var HP
//      * @brief 怪物的生命值。
//      */
//     int HP;

//     /**
//      * @var Bad_Good
//      * @brief 怪物的属性，0为恶，1为善。
//      */
//     bool Bad_Good;

//     /**
//      * @var v
//      * @brief 怪物的移动速度。
//      */
//     int v;

//     /**
//      * @var money
//      * @brief 玩家击杀怪物后获得的金钱数量。
//      */
//     int money;

//     /**
//      * @var bitmap_img_ids
//      * @brief 第一个向量是 Dir 索引，第二个向量是图像 ID。
//      * @details `bitmap_img_ids[Dir][<ordered_id>]`
//      */
//     std::vector<std::vector<int>> bitmap_img_ids;

//     /**
//      * @var bitmap_switch_freq
//      * @brief 当前朝向的下一个移动姿势所需的帧数。
//      * @details 该变量由子类定义。
//      */
//     int bitmap_switch_freq;
//     int bitmap_switch_counter;
//     int bitmap_img_id;

//     /**
//      * @var path
//      * @brief 怪物的行走路径，以网格格式表示。
//      * @see Level::grid_to_region(const Point &grid) const
//      */
//     std::queue<Point> path;

//    private:
//     AnimalType type;
//     Dir dir;
// };

// #endif
