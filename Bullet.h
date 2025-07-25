#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet
{
private:
    Texture bullet;
    Sprite bullet_shape;
public:
    Bullet();
    ~Bullet();
    void restart_bullet(Sprite& spaceship_shape);
    void bulletMovement(float time);
    void defaultpos();
    void draw(RenderWindow& main_win);
    bool getOfBounds();
    FloatRect getBulletXY();
    Vector2f bulletBounds();
    bool ishoot = false, controlflag = true;
};


