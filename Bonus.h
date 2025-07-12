#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bonus
{
private:
    Texture bonusTexture;
    Sprite bonusSprite;

public:
    Bonus();
    ~Bonus();

    void BonusMovement(float time);
    void draw(RenderWindow& main_win);
    void restart();
    bool collision(FloatRect object);
    // FloatRect Bonus::getBonusXY();
};