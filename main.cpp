#include "game_lib.h"
#include "Meteor.h"
#include "Bullet.h"

int main(){
    srand(time(NULL));
    bool gameover = false;
    bool bullet_out = false;
    Vector2f spaceship_crash_pos;
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
    float background_time;
    float spaceship_time;

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
    Meteor* meteors[10] = {nullptr};
    int nmeteors = 10;
    for (int m = 0; m < nmeteors; m++) meteors[m] = new Meteor();

// создаем пульки для корабля
    Bullet* bullet[15] = {nullptr};
    int bullets = 15;
    for (int b = 0; b < bullets; b++) bullet[b] = new Bullet();
    int count_bullets = 0;
    
// основной код игры
    while (main_win.isOpen()){
        background_time = clock.getElapsedTime().asMicroseconds();
        spaceship_time = clock.getElapsedTime().asMicroseconds();
        spaceship_time = spaceship_time / 2000;
        background_time = background_time / 6000;
        clock.restart();

         while (const std::optional event = main_win.pollEvent()){
            if (event->is<sf::Event::Closed>())
                main_win.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){ //блок обработки нажатия кнопок
                if (!gameover){
                    if (keyPressed->scancode == sf::Keyboard::Scancode::A)
                    spaceship_pos.x = -0.5;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::D)
                        spaceship_pos.x = 0.5;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::W)
                        spaceship_pos.y = -0.5;
                    if (keyPressed->scancode == sf::Keyboard::Scancode::S)
                        spaceship_pos.y = 0.5;
                }
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space && count_bullets < bullets && !gameover){ // выстрел на пробел
                    bullet[count_bullets]->restart_bullet(spaceship_shape);
                    bullet[count_bullets]->ishoot = true;
                    count_bullets += 1;
                }
            } else  if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
                if (!gameover){
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
        }
        BackgroundMovement(background_shape, background_shape2, background_time); // имитация космического фона
        SpaceshipMovement(spaceship_shape, spaceship_pos, spaceship_time); // движение космического корабля
        for (int i = 0; i < nmeteors; i++){ // движение метеоритов
            if (meteors[i]) meteors[i]->MeteorMovement();
        }
        main_win.clear();
        main_win.draw(background_shape);
        main_win.draw(background_shape2);
        for (int m = 0; m < nmeteors; m++){
            if (meteors[m]){
                meteors[m]->draw(main_win);
            }
        }
        if (gameover){
            spaceship_pos = {0,0};
            explode_shape.setPosition(spaceship_crash_pos);
            main_win.draw(explode_shape);
        } else {
            main_win.draw(spaceship_shape);
            for (int i = 0; i < nmeteors;i++){
                if (meteors[i]){
                    if (meteors[i]->collision(spaceship_shape.getGlobalBounds())){
                        gameover = true;
                        spaceship_crash_pos = spaceship_shape.getPosition();
                    }
                }
            }
             for (int i = 0; i < bullets; i++){
                if (bullet[i] && bullet[i]->ishoot){
                    bullet[i]->bulletMovement();
                    bullet[i]->draw(main_win);
                    if (bullet[i]->getOfBounds()){
                        delete bullet[i];
                        bullet[i] = nullptr;
                    }
                    for (int m = 0; m < nmeteors; m++){
                        if (bullet[i]){
                            if (meteors[m]){
                                if (meteors[m]->collision(bullet[i]->getBulletXY())){
                                    delete meteors[m];
                                    meteors[m] = nullptr;
                                    delete bullet[i];
                                    bullet[i] = nullptr;
                                }
                            }
                        }
                    }
                }
            }
                
        }
        // for (int i = 0; i < nmeteors; i++){
        //     meteors[i]->MeteorMovement();
        // } 
        // for (int i = 0; i < nmeteors; i++) {
        //     if (meteors[i]->collision(spaceship_shape.getGlobalBounds())){   // блок обработки столкновения корабля и метеорита
        //         gameover = true; 
        //         explode_shape.setPosition(spaceship_shape.getPosition());
        //     }
        // }
        // main_win.clear();
        // main_win.draw(background_shape);
        // main_win.draw(background_shape2);
        // if (gameover)
        // {
        //     spaceship_pos = {0,0};
        //     main_win.draw(explode_shape);
        // } else {main_win.draw(spaceship_shape);}
        // for (int i = 0; i < nmeteors && meteors[i]; i++){
        //     meteors[i]->draw(main_win);
        // }
        // for (int i = 0; i < bullets; i++)
        // {
        //     if (bullet[i] && bullet[i]->ishoot)
        //     {
        //         bullet[i]->bulletMovement();
        //         bullet[i]->draw(main_win);
        //         if (bullet[i]->getOfBounds()){
        //             delete bullet[i];
        //             bullet[i] = nullptr;
        //         }
        //     }
        // }
        main_win.display();
    
    }
}

