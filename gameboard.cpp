/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QTimer>
#include <chrono>
#include <random>
#include <QKeyEvent>
#include <iostream>
#include <string>

//set seed for random generator
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator (seed);


/**
 *Default Constructor
*/
GameBoard::GameBoard(QWidget *parent) :
    QWidget(parent){};


/**
 *Regular Constructor
 *@param size is the size of the grid board that will be created on the game board
 **/
GameBoard::GameBoard(int size, std::string diff, int lev, int liv){

    setWindowTitle("Color Conundrum");

    difficulty = diff;

    //set window sizes based on difficulty level

    /*
    //mac sizes
    if (diff == "Easy") setFixedSize(360,560);
    else if (diff == "Medium") setFixedSize(400,600);
    else if (diff == "Difficult") setFixedSize(440,640);
    else if (diff == "Extreme") setFixedSize(520,720);
*/


    // windows sizes
    if (difficulty == "Easy") setFixedSize(340,520);
    else if (difficulty == "Medium") setFixedSize(380,560);
    else if (difficulty == "Difficult") setFixedSize(420,600);
    else if (difficulty == "Extreme") setFixedSize(500,680);

    //load in pictures
    mainchar = new QPixmap("/PIC 10C/Homework/ColorConundrum/mainchar.png");
    enemychar = new QPixmap("/PIC 10C/Homework/ColorConundrum/enemychar.jpg");
    fish = new QPixmap("/PIC 10C/Homework/ColorConundrum/fish.png");
    bomb = new QPixmap("/PIC 10C/Homework/ColorConundrum/bomb.png");
    life = new QPixmap("/PIC 10C/Homework/ColorConundrum/life.png");

    //windows filepath -- "/PIC 10C/Homework/ColorConundrum/mainchar.png"
    //mac filepath -- "/Users/carlyhines/Documents/UCLA Junior/Fall 2014/PIC 10C/Homework/ColorConundrum5/

    //create title
    title = new QWidget;
    QVBoxLayout *title_layout = new QVBoxLayout(title);

    QLabel* colorconundrum = new QLabel("COLOR CONUNDRUM");
    colorconundrum->setStyleSheet("color : #00007F");
    colorconundrum->setFont(QFont ("Heiti SC", 20));

    QLabel* difflevel = new QLabel("Difficulty: " + QString::fromStdString(difficulty));
    difflevel->setStyleSheet("color : #00007F ");

    level= lev;
    QLabel* level_text = new QLabel("Level: " + QString::number(level));
    level_text->setStyleSheet("color : #00007F");

    title_layout->addWidget(colorconundrum);
    title_layout->addWidget(difflevel);
    title_layout->addWidget(level_text);




    //create lives_bar

    lives_bar = new QWidget;
    QHBoxLayout* lives_layout = new QHBoxLayout(lives_bar);

    lives_remaining = 5; //can't be more than 5
    lives = new QLabel*[lives_remaining-1];

    for(size_t i=0;i<lives_remaining-1;i++) {
        lives[i] = new QLabel;
        lives[i]->setPixmap(*life);
        lives[i]->setStyleSheet("background-color: white");
        lives[i]->setMinimumSize(40,40);
        lives[i]->setMaximumSize(40,40);
        lives[i]->setScaledContents(true);
        lives_layout->addWidget(lives[i]);
    }


    //create level_bar

    level_bar = new QWidget;
    QHBoxLayout* level_layout = new QHBoxLayout(level_bar);



    seconds_left = 60;
    QLabel* timertext = new QLabel ("Time\nRemaining");
    timertext->setStyleSheet("color : #00007F ");
    timertext->setAlignment(Qt::AlignCenter);
    //windows
    timertext->setFixedWidth(60);
    //mac
    //timertext->setFixedWidth(80);

    timerdisplay = new QLCDNumber (2);
    timerdisplay->setStyleSheet("background-color : black ");
    timerdisplay->setFixedSize(40,40);
    timerdisplay->display(seconds_left);


    //level_layout->addWidget(level);
    level_layout->addWidget(timertext, Qt::AlignCenter);
    level_layout->addWidget(timerdisplay, Qt::AlignCenter);
    level_layout->setSizeConstraint(QLayout::SetFixedSize);


    //combine lives_bar and level_bar together to make top
    Top = new QWidget;
    QHBoxLayout* top_layout = new QHBoxLayout(Top);

    top_layout->addWidget(lives_bar);
    top_layout->addWidget(level_bar);







    //create central grid_board
    grid_board = new QWidget;
    board_size=size;

    boxes = new boxOnBoard*[board_size*board_size];

    QGridLayout *SquareGrid = new QGridLayout(grid_board);
    SquareGrid->setSpacing(0);

    for(size_t i=0;i<board_size;i++) {
        for(size_t j=0;j<board_size;j++) {

             // Create box and set properties.
             boxes[i*board_size+j] = new boxOnBoard;

             // Add box to the layout
             SquareGrid -> addWidget(boxes[i*board_size+j] ,i,j);
        }
    }

    if (difficulty == "Easy") grid_board->setFixedSize(320,320);
    else if (difficulty == "Medium") grid_board->setFixedSize(360,360);
    else if (difficulty == "Difficult") grid_board->setFixedSize(400,400);
    else if (difficulty == "Extreme") grid_board->setFixedSize(480,480);

    // Initialize the mainchar at the top left corner, coordinate (0,0).
    mainchar_position = new QPoint(0,0);
    boxes[0]->setPixmap(*mainchar);
    boxes[0]->setScaledContents(true);



    //piece together all components
    QVBoxLayout* piece_together = new QVBoxLayout;

    piece_together->addWidget(title);
    piece_together->addWidget(Top);
    piece_together->addWidget(grid_board, Qt::AlignCenter);

    this->setLayout(piece_together);
    this->setStyleSheet("background-color : #FFDEC1");


    //initialize enemies just outside board (more enemies as levels get higher)
    number_enemies = 4;
    enemychar_positions = new QPoint[number_enemies];
    enemychar_lifelost = new bool[number_enemies];


    // Set enemies randomly on board
    std::uniform_int_distribution<int> distribution(1,board_size);

    enemychar_positions[0].setY(-1);
    enemychar_positions[0].setX(distribution(generator));
    enemychar_lifelost[0] = false;

    enemychar_positions[1].setX(board_size);
    enemychar_positions[1].setY(distribution(generator));
    enemychar_lifelost[1] = false;

    enemychar_positions[2].setY(-1);
    enemychar_positions[2].setX(distribution(generator));
    enemychar_lifelost[2] = false;

    enemychar_positions[3].setY(board_size);
    enemychar_positions[3].setX(distribution(generator));
    enemychar_lifelost[3] = false;


    //set timer for enemy movement and color switching (faster if higher level)
    enemy_timer = new QTimer(this);
    QObject::connect(enemy_timer, SIGNAL(timeout()), this, SLOT(updateEnemies()));

    color_timer = new QTimer(this);
    QObject::connect(color_timer, SIGNAL(timeout()), this, SLOT(updateColors()));

    if (level == 1){
        enemy_timer->start(800);
        color_timer->start(2300);
    }
    else if (level == 2){
        enemy_timer->start(700);
        color_timer->start(2200);
    }
    else if (level == 3){
        enemy_timer->start(600);
        color_timer->start(2100);
    }
    else if (level == 4){
        enemy_timer->start(500);
        color_timer->start(2000);
    }
    else{
        enemy_timer->start(400);
        color_timer->start(1900);
    }


    //initialize fish powerup outside board
    std::uniform_int_distribution<int> powerupdist(0, board_size-1);
    fish_position = new QPoint(powerupdist(generator),-20);

    //initialize life powerup outside board
    life_position = new QPoint(powerupdist(generator),-40);

    //initialize bomb powerup outside board
    bomb_position = new QPoint(powerupdist(generator), -60);


    //create and start timer for fish powerup movement
    fish_timer = new QTimer(this);
    QObject::connect(fish_timer, SIGNAL(timeout()), this, SLOT(updateFishPowerup()));
    fish_timer->start(400);

    //create and start timer for life powerup movement
    life_timer = new QTimer(this);
    QObject::connect(life_timer, SIGNAL(timeout()), this, SLOT(updateLifePowerup()));
    life_timer->start(400);

    //create and start timer for bomb powerup movement
    bomb_timer = new QTimer(this);
    QObject::connect(bomb_timer, SIGNAL(timeout()), this, SLOT(updateBombPowerup()));
    bomb_timer->start(400);



    //create and start level timer
    level_timer = new QTimer(this);
    QObject::connect(level_timer, SIGNAL(timeout()), this, SLOT(subtractOneSecond()));
    level_timer->start(1000); //timeouts every second

    QCoreApplication::processEvents();
    repaint();
}

/**
 *destructor
*/
GameBoard::~GameBoard()
{
    delete[] mainchar_position;
    delete[] enemychar_positions;

}

/**
*setMainWindow member function sets the pointer to the MainWindow object that is correlated with this game board
*@param main_window is the MainWindow pointer to the object that will be set as the MainWindow pointer within this class
**/
void GameBoard::setMainWindow(MainWindow *main_window){
    mw = main_window;
}




/**
 *updateMainchar function changes the X and Y coordinates of the main character and repaints him on the board in the new position.
 *@param px is the previous x coordinate of the main character
 *@param py is the previous y coordinate of the main character
 *@param nx is the new x coordinate of the main character
 *@param ny is the new y coordinate of the main character
 **/
void GameBoard::updateMainchar(int px, int py, int nx, int ny) {
    boxes[py*board_size+px]->clear();

    mainchar_position->setX(nx);
    mainchar_position->setY(ny);

    checkForCollision();
    QCoreApplication::processEvents();
    repaint();
}


/**
*updateEnemies function updates the X and Y coordinates of the enemies in a random fashion and repaints them on the board.
*/
void GameBoard::updateEnemies() {

    std::normal_distribution<double> p(0,1);
    std::uniform_int_distribution<int> x_or_y(0,1);
    std::uniform_int_distribution<int> increment(0,1);

    for(size_t i=0;i<number_enemies;i++) {

        int px = enemychar_positions[i].rx();
        int py = enemychar_positions[i].ry();

        if(px>=0 && py >= 0 && px<board_size && py<board_size) {
            boxes[py*board_size+px]->clear();
        }

        int nx = px;
        int ny = py;

        if (p(generator)<0.1){
            if(x_or_y(generator) == 0) {
                nx = px + 2*increment(generator)-1;
                if(nx >= -1 && nx <= board_size)
                    enemychar_positions[i].setX(nx);
            }
            else {
                ny = py + 2*increment(generator)-1;
                if(ny >= -1 && ny <= board_size)
                    enemychar_positions[i].setY(ny);
            }
            enemychar_lifelost[i] = false;
        }

    }

    checkForCollision();

    QCoreApplication::processEvents();
    repaint();
}

/**
 * updateColors function switches the color at a random spot on the board to the opposite color
 */
void GameBoard::updateColors() {
   int x, y;
   std::uniform_int_distribution<int> random(0,board_size-1);
   x = random(generator);
   y = random(generator);

   if (boxes[y*board_size+x]->getColor()==0){
       boxes[y*board_size+x]->setStyleSheet("QLabel { background-color : #49C8F2 }");
       boxes[y*board_size+x]->setColor(1);
   }
   else if (boxes[y*board_size+x]->getColor()==1){
       boxes[y*board_size+x]->setStyleSheet("QLabel{ background-color : #ED72A3 }");
       boxes[y*board_size+x]->setColor(0);
   }

   QCoreApplication::processEvents();
   repaint();

   checkForWin();
}


/**
  * updateFishPowerup function moves the powerup one spot down on the board, or resets it to a random x if it goes off the bottom of the board.
  * It also checks for collisions between the powerup and mainchar
*/
void GameBoard::updateFishPowerup() {

    std::uniform_int_distribution<int> fishdist(0,board_size-1);
    int px = fish_position->rx();
    int py = fish_position->ry();

    if(py >= 0 && py<board_size) {
        boxes[py*board_size+px]->clear();
    }

    int nx, ny;

    //if y goes off board, reset y to -30 and new random x
    if (py >= 13){
        nx = fishdist(generator);
        ny = -30;
    }
    else {
        nx = px;
        ny = py+=1;
    }


    fish_position->setX(nx);
    fish_position->setY(ny);

    checkForCollision();

    QCoreApplication::processEvents();
    repaint();
}

/**
  * updateLifePowerup function moves the powerup one spot down on the board, or resets it to a random x if it goes off the bottom of the board.
  * It also checks for collisions between the powerup and mainchar
*/
void GameBoard::updateLifePowerup() {
    std::uniform_int_distribution<int> lifedist(0,board_size-1);
    int px = life_position->rx();
    int py = life_position->ry();

    if(py >= 0 && py<board_size) {
        boxes[py*board_size+px]->clear();
    }

    int nx, ny;

    //if y goes off board, reset y to -40 and new random x
    if (py >= 13){
        nx = lifedist(generator);
        ny = -40;
    }
    else {
        nx = px;
        ny = py+=1;
    }


    life_position->setX(nx);
    life_position->setY(ny);

    checkForCollision();

    QCoreApplication::processEvents();
    repaint();
}


/**
  * updateBombPowerup function moves the powerup one spot down on the board, or resets it to a random x if it goes off the bottom of the board.
  * It also checks for collisions between the powerup and mainchar
*/
void GameBoard::updateBombPowerup(){

    std::uniform_int_distribution<int> bombdist(0,board_size-1);
    int px = bomb_position->rx();
    int py = bomb_position->ry();

    if(py >= 0 && py<board_size) {
        boxes[py*board_size+px]->clear();
    }

    int nx, ny;

    //if y goes off board, reset y to -80 and new random x
    if (py >= 13){
        nx = bombdist(generator);
        ny = -80;
    }
    else {
        nx = px;
        ny = py+=1;
    }


    bomb_position->setX(nx);
    bomb_position->setY(ny);

    checkForCollision();

    QCoreApplication::processEvents();
    repaint();
}



void GameBoard::nextLevel(){

    GameBoard* levelup = new GameBoard(board_size, difficulty, level+1, lives_remaining);
    levelup->setMainWindow(mw);
    levelup->show();
}




/**
 * subtractOneSecond function subtracts a second from the timer.
 * If timer is at 0, ends game and outputs game over screen
*/
void GameBoard::subtractOneSecond() {

    if (seconds_left!=0){
        seconds_left -= 1;
        timerdisplay->display(seconds_left);
    }

    //else game over

    else{

        this->close();

        level_timer->stop();
        fish_timer->stop();
        bomb_timer->stop();
        life_timer->stop();
        enemy_timer->stop();
        color_timer->stop();

        QWidget* gameover = new QWidget;
        QVBoxLayout* gameover_layout = new QVBoxLayout(gameover);

        QLabel* gameover_text = new QLabel("You ran out of time!\n Game over!");
        gameover_text->setStyleSheet("color : #00007F");
        gameover_text->setAlignment(Qt::AlignCenter);
        gameover_layout->addWidget(gameover_text, Qt::AlignCenter);

        QPushButton* tryagain = new QPushButton("Try a Different Difficulty!");
        tryagain->setStyleSheet("color : #00007F");
        gameover_layout->addWidget(tryagain, Qt::AlignCenter);

        QPushButton* quit = new QPushButton("Quit");
        quit->setStyleSheet("color : #00007F");
        gameover_layout->addWidget(quit, Qt::AlignCenter);

        gameover->setStyleSheet("background-color : #FFDEC1");
        gameover->show();

        QObject::connect(tryagain, SIGNAL(clicked()), mw, SLOT(show()));
        QObject::connect(tryagain, SIGNAL(clicked()), gameover, SLOT(close()));

        QObject::connect(quit, SIGNAL(clicked()), gameover, SLOT(close()));

    }

    QCoreApplication::processEvents();
    repaint();
}


/**
 * changeColor function switches the color at a spot on the board given by the parameters
 * @param x is the x coordinate of the spot on the board that will have its color switched
 * @param y is the y coordinate of the spot on the board that will have its color switched
 */
void GameBoard::changeColor(int x, int y) {
    if (boxes[y*board_size+x]->getColor()==0){
        boxes[y*board_size+x]->setStyleSheet("QLabel { background-color : #49C8F2 }");
        boxes[y*board_size+x]->setColor(1);
    }
    else if (boxes[y*board_size+x]->getColor()==1){
        boxes[y*board_size+x]->setStyleSheet("QLabel{ background-color : #ED72A3 }");
        boxes[y*board_size+x]->setColor(0);
    }

    QCoreApplication::processEvents();
    repaint();

    checkForWin();
}


/**
 * checkForCollision function checks to see if the main character and any enemy occupy the same space on the board.
 * If they do, the user loses a life.
 * If there are no lives left, it opens game over window where the user can either quit or try again, which redirects to the main window (menu).
 */
void GameBoard::checkForCollision(){

    //check for collision between mainchar and enemy
    for(size_t i=0; i<number_enemies; i++){
        if (enemychar_positions[i]==*mainchar_position && enemychar_lifelost[i] == false){

            //lose life
            if(lives_remaining > 1) {
                lives_remaining--;
                lives[lives_remaining-1]->clear();
                enemychar_lifelost[i] = true;
            }

            //if no lives left, game over
            else{
                this->close();

                level_timer->stop();
                fish_timer->stop();
                bomb_timer->stop();
                life_timer->stop();
                enemy_timer->stop();
                color_timer->stop();

                QWidget* gameover = new QWidget;
                QVBoxLayout* gameover_layout = new QVBoxLayout(gameover);

                QLabel* gameover_text = new QLabel("You're out of lives!\n Game over!");
                gameover_text->setStyleSheet("color : #00007F");
                gameover_text->setAlignment(Qt::AlignCenter);
                gameover_layout->addWidget(gameover_text, Qt::AlignCenter);

                QPushButton* tryagain = new QPushButton("Try a Different Difficulty!");
                tryagain->setStyleSheet("color : #00007F");
                gameover_layout->addWidget(tryagain, Qt::AlignCenter);

                QPushButton* quit = new QPushButton("Quit");
                quit->setStyleSheet("color : #00007F");
                gameover_layout->addWidget(quit, Qt::AlignCenter);

                gameover->setStyleSheet("background-color : #FFDEC1");
                gameover->show();

                QObject::connect(tryagain, SIGNAL(clicked()), mw, SLOT(show()));
                QObject::connect(tryagain, SIGNAL(clicked()), gameover, SLOT(close()));

                QObject::connect(quit, SIGNAL(clicked()), gameover, SLOT(close()));

            }
        }
    }

    //check for collision between mainchar and fish powerup
    std::uniform_int_distribution<int> powerupdist(0,board_size-1);

    if (*mainchar_position == *fish_position){
        seconds_left += 5;
        boxes[fish_position->ry()*board_size+fish_position->rx()]->clear();
        fish_position->setX(powerupdist(generator));
        fish_position->setY(-30);
    }

    //check for collision between mainchar and life powerup
    if (*mainchar_position == *life_position){
        if(lives_remaining < 5){
            lives[lives_remaining-1]->setPixmap(*life);
            lives_remaining += 1;
        }

        boxes[life_position->ry()*board_size+life_position->rx()]->clear();
        life_position->setX(powerupdist(generator));
        life_position->setY(-50);
    }


    //check for collision between mainchar and bomb powerup
    if(*mainchar_position == *bomb_position){
        for (int i=0; i<number_enemies; i++){
            int enemy_x = enemychar_positions[i].rx();
            int enemy_y = enemychar_positions[i].ry();
            if (enemy_x >= 0 && enemy_y >= 0 && enemy_x < board_size && enemy_y < board_size){
                boxes[enemychar_positions[i].ry()*board_size+enemychar_positions[i].rx()]->clear();
                enemychar_positions[i].setX(-1);
                enemychar_positions[i].setY(powerupdist(generator));
            }
        }

        boxes[bomb_position->ry()*board_size+bomb_position->rx()]->clear();
        bomb_position->setX(powerupdist(generator));
        bomb_position->setY(-80);
    }

    QCoreApplication::processEvents();
    repaint();

}


/**
 * checkForWin function checks to see if all the spots on the board are the same color.
 * If they are, winner window opens where user can continue on to try another difficulty level.
 */
void GameBoard::checkForWin(){
    bool win = true;
    int start_color = boxes[0]->getColor();

    //check if all boxes are the same color
    for(size_t i=0; i<board_size; i++){
        for(size_t j=0; j<board_size; j++){
            if( boxes[i*board_size+j]->getColor() != start_color)
                win=false;
        }
    }

    //if all spots are the same color, end level and user wins
    if (win==true){
        this->close();

        level_timer->stop();
        fish_timer->stop();
        bomb_timer->stop();
        life_timer->stop();
        enemy_timer->stop();
        color_timer->stop();


        if(level < 5){
            QWidget* nextlevel = new QWidget;
            QVBoxLayout* nextlevel_layout = new QVBoxLayout(nextlevel);

            QLabel* nextlevel_text = new QLabel("Congratulations!\n You have beat Level " + QString::number(level) + "!\n");
            nextlevel_text->setStyleSheet("color : #00007F");
            nextlevel_text->setAlignment(Qt::AlignCenter);
            nextlevel_layout->addWidget(nextlevel_text, Qt::AlignCenter);

            QPushButton* next = new QPushButton("Start Level " + QString::number(level+1));
            next->setStyleSheet("color : #00007F");
            nextlevel_layout->addWidget(next, Qt::AlignCenter);

            nextlevel->setStyleSheet("background-color : #FFDEC1");
            nextlevel->show();

            QObject::connect(next, SIGNAL(clicked()), this, SLOT(nextLevel()));
            QObject::connect(next, SIGNAL(clicked()), nextlevel, SLOT(close()));
        }
        else{

            QWidget* win = new QWidget;
            QVBoxLayout* win_layout = new QVBoxLayout(win);

            QLabel* win_text = new QLabel ("Congratulations!\n You beat this difficulty level!");
            win_text->setAlignment(Qt::AlignCenter);
            win_text->setStyleSheet("color : #00007F");
            win_layout->addWidget(win_text);

            QPushButton* newdiff = new QPushButton("Try A Different Difficulty");
            newdiff->setStyleSheet("color : #00007F");
            win_layout->addWidget(newdiff, Qt::AlignCenter);

            win->setStyleSheet("background-color : #FFDEC1");
            win->show();

            lives_remaining = 5;
            for(size_t i=0;i<lives_remaining-1;i++) {
                lives[i]->setPixmap(*mainchar);
            }

            mainchar_position->setX(0);
            mainchar_position->setY(0);

            QObject::connect(newdiff, SIGNAL(clicked()), mw, SLOT(show()));

            if (difficulty == "Easy")
                QObject::connect(newdiff, SIGNAL(clicked()), mw, SLOT(BeatEasy()));
            if (difficulty == "Medium")
                QObject::connect(newdiff, SIGNAL(clicked()), mw, SLOT(BeatMedium()));
            if (difficulty == "Difficult")
                QObject::connect(newdiff, SIGNAL(clicked()), mw, SLOT(BeatDifficult()));
            if (difficulty == "Extreme")
                QObject::connect(newdiff, SIGNAL(clicked()), mw, SLOT(BeatExtreme()));

            QObject::connect(newdiff, SIGNAL(clicked()), win, SLOT(close()));
        }
    }
}


/**
 * paintEvent function repaints maincharacters and enemies on the board in their X and Y coordinate positions.
 * Also paints all powerups (fish, life, bomb) in their X and Y coordinate positions.
 * Called with repaint() function.
 */
void GameBoard::paintEvent(QPaintEvent *e) {

    //draw fish
    int fish_x = fish_position->rx();
    int fish_y = fish_position->ry();

    if(fish_x >= 0 && fish_y >= 0 && fish_x < board_size && fish_y < board_size){
        boxes[fish_y*board_size+fish_x]->setPixmap(*fish);
        boxes[fish_y*board_size+fish_x]->setScaledContents(true);
    }

    //draw life
    int life_x = life_position->rx();
    int life_y = life_position->ry();

    if(life_x >= 0 && life_y >= 0 && life_x < board_size && life_y < board_size){
        boxes[life_y*board_size+life_x]->setPixmap(*life);
        boxes[life_y*board_size+life_x]->setScaledContents(true);
    }

    //draw bomb
    int bomb_x = bomb_position->rx();
    int bomb_y = bomb_position->ry();

    if(bomb_x >= 0 && bomb_y >= 0 && bomb_x < board_size && bomb_y < board_size){
        boxes[bomb_y*board_size+bomb_x]->setPixmap(*bomb);
        boxes[bomb_y*board_size+bomb_x]->setScaledContents(true);
    }


    //draw mainchar
    int x = mainchar_position->rx();
    int y = mainchar_position->ry();

    boxes[y*board_size+x]->setPixmap(*mainchar);
    boxes[y*board_size+x]->setScaledContents(true);


    //draw enemychar
    for(size_t i=0;i<number_enemies;i++) {
        int px = enemychar_positions[i].rx();
        int py = enemychar_positions[i].ry();

        if(px >= 0 && py >= 0 && px < board_size && py < board_size) {
            boxes[py*board_size+px]->setPixmap(*enemychar);
            boxes[py*board_size+px]->setScaledContents(true);
        }
    }

}


/**
 * keyPressEvent function controls all actions when keys on the keyboard are pressed.
 * Arrow keys move the main character around
 * Space bar changes the color of the space that the maincharacter is on
 */
void GameBoard::keyPressEvent(QKeyEvent *event){

    int x = mainchar_position->rx();
    int y = mainchar_position->ry();

    //arrow keys move mainchar around the board
    switch (event->key()) {
    case Qt::Key_Left:
        if(mainchar_position->rx() != 0)
            updateMainchar(x,y,x-1,y);
        break;

    case Qt::Key_Right:
        if(mainchar_position->rx() != board_size-1)
            updateMainchar(x,y,x+1,y);
        break;

    case Qt::Key_Up:
        if(mainchar_position->ry() != 0)
            updateMainchar(x,y,x,y-1);
        break;

    case Qt::Key_Down:
        if(mainchar_position->ry() != board_size-1)
            updateMainchar(x,y,x,y+1);
        break;

    //space bar changes color of the spot that the mainchar is on
    case Qt::Key_Space:
        changeColor(x,y);
        break;

    //other keys handled by default
    default:
        QWidget::keyPressEvent(event);
    }

    QCoreApplication::processEvents();
    repaint();
    return;
}
