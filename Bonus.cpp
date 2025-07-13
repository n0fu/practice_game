#include "Bonus.h"


Bonus::Bonus() : bonusTexture(), bonusSprite(bonusTexture)
{
    bonusTexture.loadFromFile("images/bonus.png");
    bonusSprite.setTexture(bonusTexture);
    bonusSprite.setTextureRect(IntRect({0,0},{64,64}));
    restart();
}

Bonus:: ~Bonus()
{

}

void Bonus::BonusMovement(float time)
{
    bonusSprite.move(Vector2f(0, static_cast<float>(0.3)*time));
    if (bonusSprite.getPosition().y > 720) restart();
}

void Bonus::restart()
{
    float x = static_cast<float>(rand()% 492 + 64);
    float y = static_cast<float>(rand()% 720 - 720); 
    bonusSprite.setPosition(Vector2f(x,y));
}

void Bonus::draw(RenderWindow& main_win)
{
    main_win.draw(bonusSprite);
}

bool Bonus::collision(FloatRect object)
{
    if (bonusSprite.getGlobalBounds().findIntersection(object)) return true;
    return false;
}
