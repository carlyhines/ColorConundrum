/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboard.h"

namespace Ui {
class MainWindow;
}


/**
* @class MainWindow
* @brief MainWindow class holds all widgets on the menu system.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGameBoard(GameBoard* game_board);

public slots:
    void createEasyGB();
    void createMediumGB();
    void createDifficultGB();
    void createExtremeGB();

    void BeatEasy();
    void BeatMedium();
    void BeatDifficult();
    void BeatExtreme();

private:
    Ui::MainWindow *ui;
    GameBoard* gb;
    int board_size;

    QPixmap* mainchar;
    QPixmap* enemychar;
};

#endif // MAINWINDOW_H
