#include "Meteor.h"

Meteor::Meteor() : space_obj_text(), space_obj(space_obj_text)
{
    space_obj_text.loadFromFile("images/space_object.png");
    space_obj.setTexture(space_obj_text);
    space_obj.setTextureRect(IntRect({0,0},{64,64}));
    restart();
}

Meteor::~Meteor() 
{

}

void Meteor::MeteorMovement()
{
    space_obj.move(Vector2f(0, static_cast<float>(0.03)));
    if (space_obj.getPosition().y > 720) restart();
}

void Meteor::draw(RenderWindow& main_win)
{
    main_win.draw(space_obj);
}

bool Meteor::collision(FloatRect object)
{
    if (space_obj.getGlobalBounds().findIntersection(object)) return true;
    return false;
}

void Meteor::restart()
{
    float x = static_cast<float>(rand()% 492 + 64);
    float y = static_cast<float>(rand()% 720 - 720); 

    space_obj.setPosition(Vector2f(x,y));

}

FloatRect Meteor::getMeteorXY()
{
    return space_obj.getGlobalBounds();
}