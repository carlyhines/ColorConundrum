/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/




//set timers to stop when win game
//fix graphics files
//fix layout size restraints
//add bool for enemies

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QLabel>
#include <string>
#include <QLCDNumber>
#include <QSpacerItem>


//forward declarations
class boxOnBoard;
class MainWindow;



/**
 * @class GameBoard
 * @brief The GameBoard class creates and holds all widgets on the main game board interface
 */
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = 0);
    ~GameBoard();
    GameBoard(int size, std::string diff, int lev, int liv);

    void setMainWindow(MainWindow* main_window);
    void updateMainchar(int px, int py, int nx, int ny);
    void changeColor(int x, int y);
    void checkForCollision();
    void checkForWin();

    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void updateEnemies();
    void updateColors();

    //powerups
    void updateFishPowerup();
    void updateLifePowerup();
    void updateBombPowerup();

    //level up
    void nextLevel();

    void subtractOneSecond();

private:

    //Picture Variables
    QPixmap* mainchar;
    QPixmap* enemychar;
    QPixmap* fish;
    QPixmap* bomb;
    QPixmap* life;

    //Position Variables
    QPoint *mainchar_position;
    QPoint *enemychar_positions;
    bool *enemychar_lifelost;
    size_t number_enemies;
    QPoint *fish_position;
    QPoint *bomb_position;
    QPoint *life_position;

    //Timers
    QTimer *enemy_timer;
    QTimer *color_timer;
    QTimer *fish_timer;
    QTimer *bomb_timer;
    QTimer *life_timer;


    //Title Variables
    QWidget* title;

    //Lives Variables
    unsigned int lives_remaining;
    QLabel** lives;
    QWidget* lives_bar;

    //Level Variables
    QTimer *level_timer;
    int seconds_left;
    QLCDNumber* timerdisplay;
    QWidget* level_bar;
    int level;

    //Top Variables
    QWidget* Top;

    //Board Variables
    QWidget* grid_board;
    boxOnBoard** boxes;
    size_t board_size;
    std::string difficulty;


    MainWindow* mw;

};



/**
 * @class boxOnBoard
 * @brief The boxOnBoard class creates a single box widget that will be put on the grid on the board
 */
class boxOnBoard : public QLabel
{
public:
    /**
     * regular constructor -- sets box to random color
     */
    boxOnBoard(){
        //set to random color - pink or blue
        color = rand()%2;

        if(color==0) //pink
           this->setStyleSheet("QLabel{ background-color : #ED72A3 }");
        else if(color==1) //blue
           this->setStyleSheet("QLabel { background-color : #49C8F2 }");

        //give standard properties
        this->setFixedSize(40,40);
        this->setFrameStyle(3);
        this->setAlignment(Qt::AlignCenter);
    }

    /**
     * getColor function returns the color of the block
     * @return color returns the color of the block
     */
    int getColor() {return color;}

    /**
     * setColor function changes color of the block
     * @param col is the new color that the block is changed to
     */
    void setColor(int col){
        color=col;
    }

private:
    int color; //0=pink, 1=blue
};



#endif // GAMEBOARD_H
