/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/

#include "mainwindow.h"
#include "gameboard.h"
#include <QApplication>
#include <QObject>
#include <cstdlib>
#include <ctime>
#include <iostream>

//MAIN
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(time(NULL));

    MainWindow* w = new MainWindow;

    w->show();

    return a.exec();

}
