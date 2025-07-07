#include "game_lib.h"
#include "Meteor.h"
#include "Bullet.h"

int main(){
    srand(time(NULL));
    bool gameover = false;
    bool bullet_out = false;
// создаем окно
    RenderWindow main_win(VideoMode({720,720}), "Game", Style::Close);

// добавляем иконку к окну
    Image winicon;
    if (!winicon.loadFromFile("images/icon.png")) return 1;
    main_win.setIcon(Vector2u(32,32),winicon.getPixelsPtr());

// задний фон
    Texture background;
    if (!background.loadFromFile("images/zxc.png")) return 1;
    RectangleShape background_shape(Vector2f(720,720));
    background_shape.setTexture(&background);
    background_shape.setPosition(Vector2f(0,0));
    
    RectangleShape background_shape2(Vector2f(720,720));
    background_shape2.setTexture(&background);
    background_shape2.setPosition(Vector2f(0,-720));
    Vector2f pos1,pos2;
    Clock clock;
    float time;

// создаем космический шаттл
    Vector2f spaceship_pos = {0, 0};
    Texture spaceship;
    spaceship.loadFromFile("images/spaceship.png");
    Sprite spaceship_shape(spaceship);
    spaceship_shape.setTextureRect(IntRect({0,0},{64,64}));
    spaceship_shape.setPosition(Vector2f(360,600));

// создаем спрайт взрыва космического корабля
    Texture explode;
    explode.loadFromFile("images/explode.png");
    Sprite explode_shape(explode);
    explode_shape.setTextureRect(IntRect({0,0},{64,64}));

// создаем метеориты
    Meteor meteors[10];
    int nmeteors = 10;

    Bullet* bullet[15] = {nullptr};
    int bullets = 15;
    for (int i = 0; i < bullets; i++) bullet[i] = new Bullet();
    int count_bullets = 0;
    
// основной код игры
    while (main_win.isOpen()){
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 6000;
        clock.restart();

         while (const std::optional event = main_win.pollEvent()){
            if (event->is<sf::Event::Closed>())
                main_win.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                    spaceship_pos.x = -0.05;
                if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                    spaceship_pos.x = 0.05;
                if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                    spaceship_pos.y = -0.05;
                if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                    spaceship_pos.y = 0.05;
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space && count_bullets < bullets && !gameover){
                    bullet[count_bullets]->restart_bullet(spaceship_shape);
                    bullet[count_bullets]->ishoot = true;
                    count_bullets += 1;
            }
            } else  if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                if (keyReleased->scancode == sf::Keyboard::Scancode::A)
                    spaceship_pos.x = 0;
                if (keyReleased->scancode == sf::Keyboard::Scancode::D)
                    spaceship_pos.x = 0;
                if (keyReleased->scancode == sf::Keyboard::Scancode::W)
                    spaceship_pos.y = 0;
                if (keyReleased->scancode == sf::Keyboard::Scancode::S)
                    spaceship_pos.y = 0;
            }
        }
        BackgroundMovement(background_shape, background_shape2, time);
        SpaceshipMovement(spaceship_shape, spaceship_pos);
        for (int i = 0; i < nmeteors; i++) meteors[i].MeteorMovement(); 
        for (int i = 0; i < nmeteors; i++) 
        { if (meteors[i].collision(spaceship_shape.getGlobalBounds()))
            {   
                gameover = true; 
                explode_shape.setPosition(spaceship_shape.getPosition());
            } 
        }
        main_win.clear();
        main_win.draw(background_shape);
        main_win.draw(background_shape2);
        if (gameover)
        {
            spaceship_pos = {0,0};
            main_win.draw(explode_shape);
        } else {main_win.draw(spaceship_shape);}
        for (int i = 0; i < nmeteors; i++) meteors[i].draw(main_win);
        for (int i = 0; i < bullets; i++)
        {
            if (bullet[i] && bullet[i]->ishoot)
            {
                bullet[i]->bulletMovement();
                bullet[i]->draw(main_win);
                if (bullet[i]->getOfBounds()) {delete bullet[i]; bullet[i] = nullptr;}
            }
        }

        main_win.display();
    
    }
}

