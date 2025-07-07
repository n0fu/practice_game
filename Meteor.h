#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Meteor
{
public:
    Meteor();
    ~Meteor();

    void MeteorMovement();
    void draw(RenderWindow& main_win);
    bool collision(FloatRect object);
    bool isBombed = false;
    void restart();
    Vector2f getMeteorXY();

private:
    Sprite space_obj;
    Texture space_obj_text;
};