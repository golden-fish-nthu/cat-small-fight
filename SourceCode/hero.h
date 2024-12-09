#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include <map>
#include <queue>
#include <string>
#include <vector>
#include "Object.h"
#include "shapes/Rectangle.h"
enum class Hstate {
    left,
    right,
    back,
    front,
    max
};

class Hero : public Object {
   public:
    void draw();
    void init();
    //void update();

   private:
    Hstate state = Hstate::front;
    double speed = 5;
    std::map<Hstate, std::string> gifPath;
};
#endif
