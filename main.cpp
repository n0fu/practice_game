#include "game_lib.h"
#include "Meteor.h"
#include "Bullet.h"
#include "Bonus.h"


int main(){
    srand(time(NULL));
    bool gameover = false;
    bool winner = false; 
    bool game = false;
    int meteors = 10;
    int enemyleft = 10;
    int bullets = 15;
    int ammoleft = 15;
    int count_bullets = 0;
    Vector2f spaceship_crash_pos;

// создаем окно
    RenderWindow MainWindow(VideoMode({720,720}), "Space defender", Style::Close);
    MainWindow.setMouseCursorVisible(false);
    Image WindowIcon;
    if (!WindowIcon.loadFromFile("images/icon.png")) return 1;
    MainWindow.setIcon(Vector2u(32,32),WindowIcon.getPixelsPtr());

// задний фон
    Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("images/background.png")) return 1;
    RectangleShape Background1(Vector2f(720,720));
    RectangleShape Background2(Vector2f(720,720));
    Background1.setTexture(&BackgroundTexture);
    Background2.setTexture(&BackgroundTexture);
    Background1.setPosition(Vector2f(0,0));
    Background2.setPosition(Vector2f(0,-720));

    Vector2f pos1,pos2;
    Clock clock;
    struct timers
    {
        float background;
        float spaceship;
        float bullet;
        float meteor;
        float bonus;
    }timer;
    
    int countowin = 0;
// создаем космический шаттл
    Vector2f spaceship_pos = {0,0};
    Texture SpaceshipTexture;
    SpaceshipTexture.loadFromFile("images/spaceship.png");
    Sprite Spaceship(SpaceshipTexture);
    Spaceship.setPosition(Vector2f(360,600));

// создаем спрайт взрыва космического корабля
    Texture ExplodeTexture;
    ExplodeTexture.loadFromFile("images/explode.png");
    Sprite Explode(ExplodeTexture);

// создаем метеориты, снаряды и бонусы
    Meteor* Meteors[10] = {nullptr};
    Bullet* Bullets[75] = {nullptr};
    Bonus* Bonuses[2] = {nullptr};
    for (int m = 0; m < meteors; m++) Meteors[m] = new Meteor();
    for (int b = 0; b < 75; b++) Bullets[b] = new Bullet();
    for (int b = 0; b < 2; b++) Bonuses[b] = new Bonus();

// создаем текст для победы/проигрыша в игре
    Font font;
    font.openFromFile("font/static/BitcountGridDouble-Regular.ttf");
    Text Win(font);
    Text Lose(font);
    Win.setString("You win!");
    Lose.setString("You lose!");
    float winpos = (720 - Win.getGlobalBounds().size.length()) / 2;
    float losepos = (720 - Lose.getGlobalBounds().size.length()) / 2;
    Win.setCharacterSize(32);
    Lose.setCharacterSize(32);
    Win.setFillColor(Color::Blue);
    Lose.setFillColor(Color::Red);
    Win.setPosition({winpos,320});
    Lose.setPosition({losepos,320});

// создаем текст для того, чтобы перезапустить/начать игру
    Text RestartGame(font);
    Text StartGame(font);
    RestartGame.setString("Press F to restart game");
    StartGame.setString("Press space to start game");
    float restartpos = (720 - RestartGame.getGlobalBounds().size.length()) / 2;
    float startpos = (720 - StartGame.getGlobalBounds().size.length()) / 2;
    RestartGame.getGlobalBounds().size.length();
    StartGame.getGlobalBounds().size.length();
    RestartGame.setCharacterSize(32);
    StartGame.setCharacterSize(32);
    RestartGame.setFillColor(Color::White);
    StartGame.setFillColor(Color::White);
    RestartGame.setPosition({restartpos,370});
    StartGame.setPosition({startpos,320});

// основной код игры
    while (MainWindow.isOpen()){
        timer.background = clock.getElapsedTime().asMicroseconds();
        timer.spaceship = clock.getElapsedTime().asMicroseconds();
        timer.bullet = clock.getElapsedTime().asMicroseconds();
        timer.meteor = clock.getElapsedTime().asMicroseconds();
        timer.bonus = clock.getElapsedTime().asMicroseconds();
        timer.background = timer.background / 6000;
        timer.spaceship = timer.spaceship / 2000;
        timer.bullet = timer.bullet / 2000;
        timer.meteor = timer.meteor / 2000;
        timer.bonus = timer.bonus / 2000;
        clock.restart();

        // создаем элементы интерфейса
        std::ostringstream ammo;
        ammo << "Ammo: " << ammoleft;
        Text AmmoInterface(font);
        AmmoInterface.setString(ammo.str());
        AmmoInterface.setPosition({500,24});

        std::ostringstream enemy;
        enemy << "Enemy: " << enemyleft;
        Text EnemyInterface(font);
        EnemyInterface.setString(enemy.str());
        EnemyInterface.setPosition({20,24});

        while (const std::optional event = MainWindow.pollEvent()){
            if (event->is<sf::Event::Closed>())
                MainWindow.close();
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
                    Bullets[count_bullets]->restart_bullet(Spaceship);
                    Bullets[count_bullets]->ishoot = true;
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
        
        MainWindow.clear();
        BackgroundMovement(Background1, Background2, timer.background); 
        MainWindow.draw(Background1);
        MainWindow.draw(Background2);

        if (game){
            for (int i = 0; i < 2; i++){
                if (Bonuses[i]){
                    Bonuses[i]->BonusMovement(timer.bonus);
                    Bonuses[i]->draw(MainWindow);
                }
            }

            SpaceshipMovement(Spaceship, spaceship_pos, timer.spaceship); // движение космического корабля

            for (int i = 0; i < meteors; i++){ // движение метеоритов
                if (Meteors[i]){
                    Meteors[i]->MeteorMovement(timer.meteor);
                    Meteors[i]->draw(MainWindow);
                }
            }
            
            if (gameover){
                spaceship_pos = {0,0};
                Explode.setPosition(spaceship_crash_pos);
                MainWindow.draw(Explode);
                MainWindow.draw(Lose);
                MainWindow.draw(RestartGame);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){
                    gameover = false;
                    meteors = 10;
                    enemyleft = 10;
                    bullets = 15;
                    ammoleft = 15;
                    count_bullets = 0;
                    countowin = 0;
                    for (int m = 0; m < meteors; m++) Meteors[m] = new Meteor();
                    for (int b = 0; b < bullets; b++) Bullets[b] = new Bullet();
                    for (int b = 0; b < 2; b++) Bonuses[b] = new Bonus();
                }
            } else if (countowin == 10){
                for (int i = 0; i < bullets; i++){
                    if (Bullets[i]){
                        delete Bullets[i];
                        Bullets[i] = nullptr;
                    }
                }
                winner = true;
                Spaceship.setPosition({-10,-10});
                MainWindow.draw(Win);
                MainWindow.draw(RestartGame);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)){
                    winner = false;
                    meteors = 10;
                    enemyleft = 10;
                    bullets = 15;
                    ammoleft = 15;
                    count_bullets = 0;
                    countowin = 0;
                    for (int m = 0; m < meteors; m++) Meteors[m] = new Meteor();
                    for (int b = 0; b < bullets; b++) Bullets[b] = new Bullet();
                    for (int b = 0; b < 2; b++) Bonuses[b] = new Bonus();
                    Spaceship.setPosition(Vector2f(360,600));
                }
            } else {
                MainWindow.draw(Spaceship);
                MainWindow.draw(AmmoInterface);
                MainWindow.draw(EnemyInterface);
                for (int i = 0; i < meteors;i++){
                    if (Meteors[i] && Meteors[i]->collision(Spaceship.getGlobalBounds())){
                        gameover = true;
                        spaceship_crash_pos = Spaceship.getPosition();
                    }
                }
                for (int i = 0; i < bullets; i++){
                    if (Bullets[i] && Bullets[i]->ishoot){
                        Bullets[i]->bulletMovement(timer.bullet);
                        Bullets[i]->draw(MainWindow);
                        
                        if (Bullets[i]->getOfBounds()){
                            delete Bullets[i];
                            Bullets[i] = nullptr;
                        }
                        
                        for (int m = 0; m < meteors; m++){
                            if (Bullets[i]){
                                if (Meteors[m]){
                                    if (Meteors[m]->collision(Bullets[i]->getBulletXY())){
                                        countowin++;
                                        enemyleft--;
                                        delete Meteors[m];
                                        delete Bullets[i];
                                        Meteors[m] = nullptr;
                                        Bullets[i] = nullptr;
                                    }
                                }
                            }
                        }
                    }
                }
                for (int i = 0; i < 2; i++){
                    if(Bonuses[i] && Bonuses[i]->collision(Spaceship.getGlobalBounds())){
                        bullets += 15;
                        ammoleft += 15;
                        delete Bonuses[i];
                        Bonuses[i] = {nullptr};
                    }   
                }
            }
        } else MainWindow.draw(StartGame);
        MainWindow.display();
    }
}

