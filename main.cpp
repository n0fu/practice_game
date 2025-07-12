#include "game_lib.h"
#include "Meteor.h"
#include "Bullet.h"
#include "Bonus.h"

int main(){
    srand(time(NULL));
    bool gameover = false;
    bool winner = false;
    bool bullet_out = false;
    bool game = false;
    Vector2f spaceship_crash_pos;
// создаем окно
    RenderWindow main_win(VideoMode({720,720}), "Space defender", Style::Close);
    Image winicon;
    if (!winicon.loadFromFile("images/icon.png")) return 1;
    main_win.setIcon(Vector2u(32,32),winicon.getPixelsPtr());

// задний фон
    Texture background;
    if (!background.loadFromFile("images/background.png")) return 1;
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
    float bullet_time;
    float meteor_time;
    float bonus_time;
    int countowin = 0;
// создаем космический шаттл
    Vector2f spaceship_pos = {0, 0};
    Texture spaceship;
    spaceship.loadFromFile("images/spaceship.png");
    Sprite spaceship_shape(spaceship);
    // spaceship_shape.setTextureRect(IntRect({0,0},{64,64}));
    spaceship_shape.setPosition(Vector2f(360,600));

// создаем спрайт взрыва космического корабля
    Texture explode;
    explode.loadFromFile("images/explode.png");
    Sprite explode_shape(explode);
    // explode_shape.setTextureRect(IntRect({0,0},{64,64}));

// создаем метеориты
    Meteor* meteors[10] = {nullptr};
    int nmeteors = 10;
    int enemyleft = 10;
    for (int m = 0; m < nmeteors; m++) meteors[m] = new Meteor();

// создаем пульки для корабля
    Bullet* bullet[75] = {nullptr};
    int bullets = 15;
    int ammoleft = 15;
    for (int b = 0; b < 75; b++) bullet[b] = new Bullet();
    int count_bullets = 0;

    Bonus* bonus = {nullptr};
    bonus = new Bonus();

// создаем текст для победы/проигрыша в игре
    Font font;
    font.openFromFile("font/static/BitcountGridDouble-Regular.ttf");
    Text win(font);
    win.setString("You win!");
    win.setCharacterSize(32);
    win.setFillColor(Color::Blue);
    win.setPosition({300,320});
    Text lose(font);
    lose.setString("You lose!");
    lose.setCharacterSize(32);
    lose.setFillColor(Color::Red);
    lose.setPosition({300,320});

// создаем текст для того, чтобы перезапустить/начать игру

    Text restartGame(font);
    restartGame.setString("Press F to restart game");
    restartGame.setCharacterSize(32);
    restartGame.setFillColor(Color::White);
    restartGame.setPosition({200,370});
    Text startGame(font);
    startGame.setString("Press space to start game");
    startGame.setCharacterSize(32);
    startGame.setFillColor(Color::White);
    startGame.setPosition({200,320});

// основной код игры
    while (main_win.isOpen()){
        background_time = clock.getElapsedTime().asMicroseconds();
        spaceship_time = clock.getElapsedTime().asMicroseconds();
        bullet_time = clock.getElapsedTime().asMicroseconds();
        meteor_time = clock.getElapsedTime().asMicroseconds();
        bonus_time = clock.getElapsedTime().asMicroseconds();
        spaceship_time = spaceship_time / 2000;
        background_time = background_time / 6000;
        bullet_time = bullet_time / 2000;
        meteor_time = meteor_time / 2000;
        bonus_time = bonus_time / 2000;
        clock.restart();

        // создаем элементы интерфейса
        std::ostringstream ammo;
        ammo << "Ammo: " << ammoleft;
        Text ammoInterface(font);
        ammoInterface.setString(ammo.str());
        ammoInterface.setPosition({500,24});

        std::ostringstream enemy;
        enemy << "Enemy: " << enemyleft;
        Text enemyInterface(font);
        enemyInterface.setString(enemy.str());
        enemyInterface.setPosition({20,24});

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
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space && count_bullets < bullets && !gameover && !winner && game){ // выстрел на пробел
                    bullet[count_bullets]->restart_bullet(spaceship_shape);
                    bullet[count_bullets]->ishoot = true;
                    ammoleft--;
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) game = true;
        main_win.clear();
        BackgroundMovement(background_shape, background_shape2, background_time); // имитация космического фона
        main_win.draw(background_shape);
        main_win.draw(background_shape2);
        if (game){
            if (bonus){
                bonus->BonusMovement(bonus_time);
                bonus->draw(main_win);
            }
            SpaceshipMovement(spaceship_shape, spaceship_pos, spaceship_time); // движение космического корабля
            for (int i = 0; i < nmeteors; i++){ // движение метеоритов
                if (meteors[i]) meteors[i]->MeteorMovement(meteor_time);
            }
            for (int m = 0; m < nmeteors; m++){
                if (meteors[m]){
                    meteors[m]->draw(main_win);
                }
            }
            if (gameover){
                spaceship_pos = {0,0};
                explode_shape.setPosition(spaceship_crash_pos);
                main_win.draw(explode_shape);
                main_win.draw(lose);
                main_win.draw(restartGame);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){
                    gameover = false;
                    nmeteors = 10;
                    enemyleft = 10;
                    for (int m = 0; m < nmeteors; m++) meteors[m] = new Meteor();
                    bullets = 15;
                    ammoleft = 15;
                    for (int b = 0; b < bullets; b++) bullet[b] = new Bullet();
                    count_bullets = 0;
                    countowin = 0;
                    bonus = new Bonus();
                }
            } else if (countowin == 10){
                for (int i = 0; i < bullets; i++){
                    if (bullet[i]){
                        delete bullet[i];
                        bullet[i] = nullptr;
                    }
                }
                winner = true;
                spaceship_shape.setPosition({-10,-10});
                main_win.draw(win);
                main_win.draw(restartGame);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){
                    winner = false;
                    nmeteors = 10;
                    enemyleft = 10;
                    for (int m = 0; m < nmeteors; m++) meteors[m] = new Meteor();
                    bullets = 15;
                    ammoleft = 15;
                    for (int b = 0; b < bullets; b++) bullet[b] = new Bullet();
                    count_bullets = 0;
                    countowin = 0;
                    spaceship_shape.setPosition(Vector2f(360,600));
                    bonus = new Bonus();
                }
            } else {
                main_win.draw(spaceship_shape);
                main_win.draw(ammoInterface);
                main_win.draw(enemyInterface);
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
                        bullet[i]->bulletMovement(bullet_time);
                        bullet[i]->draw(main_win);
                        if (bullet[i]->getOfBounds()){
                            delete bullet[i];
                            bullet[i] = nullptr;
                        }
                        for (int m = 0; m < nmeteors; m++){
                            if (bullet[i]){
                                if (meteors[m]){
                                    if (meteors[m]->collision(bullet[i]->getBulletXY())){
                                        countowin++;
                                        enemyleft--;
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
                if(bonus){
                    if (bonus->collision(spaceship_shape.getGlobalBounds())){
                        bullets += 15;
                        ammoleft += 15;
                        delete bonus;
                        bonus = {nullptr};
                    }
                }   
            }
        
        } else {
            main_win.draw(startGame);
        }
        main_win.display();
    }
}

