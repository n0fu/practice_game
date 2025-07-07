#include "game_lib.h"

// движение космического корабля
void SpaceshipMovement(Sprite& spaceship_shape, Vector2f&  spaceship_pos)
{
    spaceship_shape.move(spaceship_pos);
    if (spaceship_shape.getPosition().x < 0) spaceship_shape.setPosition(Vector2f(0,spaceship_shape.getPosition().y));
    if (spaceship_shape.getPosition().x > 656) spaceship_shape.setPosition(Vector2f(656,spaceship_shape.getPosition().y));
    if (spaceship_shape.getPosition().y < 0) spaceship_shape.setPosition(Vector2f(spaceship_shape.getPosition().x,0));
    if (spaceship_shape.getPosition().y > 656) spaceship_shape.setPosition(Vector2f(spaceship_shape.getPosition().x,656));
}

// анимация космического фона
void BackgroundMovement(RectangleShape& background_shape, RectangleShape& background_shape2, float time)
{
    Vector2f pos1, pos2;
    pos1 = background_shape.getPosition();
    pos2 = background_shape2.getPosition();
    if (pos1.y > 720) background_shape.setPosition(Vector2f(0,0));
    if (pos2.y > 0) background_shape2.setPosition(Vector2f(0,-720));
    background_shape2.move(Vector2f(0,0.2*time));
    background_shape.move(Vector2f(0,0.2*time));
}


