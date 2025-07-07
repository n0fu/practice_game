#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

// движение космического корабля
void SpaceshipMovement(Sprite& spaceship_shape, Vector2f&  spaceship_pos);

// анимация космического фона
void BackgroundMovement(RectangleShape& background_shape, RectangleShape& background_shape2, float time);

