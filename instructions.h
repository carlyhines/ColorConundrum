/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <QWidget>


/**
 *@class Instructions
 *@brief The Instructions class holds all widgets on the instruction window.
 */
namespace Ui {
class Instructions;
}

class Instructions : public QWidget
{
    Q_OBJECT

public:
    explicit Instructions(QWidget *parent = 0);
    ~Instructions();

private:
    Ui::Instructions *ui;
};

#endif // INSTRUCTIONS_H
