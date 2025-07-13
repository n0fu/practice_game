#include "Bullet.h"

Bullet::Bullet() : bullet(), bullet_shape(bullet)
{
    bullet.loadFromFile("images/beam.png");
    bullet_shape.setTexture(bullet);
    bullet_shape.setTextureRect(IntRect({0,0},{7,13}));
}

Bullet::~Bullet() 
{

}

void Bullet::restart_bullet(Sprite &spaceship_shape)
{
    bullet_shape.setPosition({spaceship_shape.getPosition().x+38,spaceship_shape.getPosition().y+23});
}

void Bullet::bulletMovement(float time)
{
    bullet_shape.move({0,-0.5*time});
}

void Bullet::draw(RenderWindow &main_win)
{
    main_win.draw(bullet_shape);
}

void Bullet::defaultpos()
{
    bullet_shape.setPosition({128,128});
}

bool Bullet::getOfBounds()
{
    if (bullet_shape.getPosition().y < 0) return true;
    return false;
    
}

FloatRect Bullet::getBulletXY()
{
    return bullet_shape.getGlobalBounds();
}