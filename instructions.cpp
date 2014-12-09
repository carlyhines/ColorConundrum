/********************
Carly Hines
Homework 9 - Final Game Project
12/10/2014
PIC 10C Fall 2014
Color Conundrum Game
Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
********************/

#include "instructions.h"
#include "ui_instructions.h"

/**
 * default constructor
 */
Instructions::Instructions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Instructions)
{
    ui->setupUi(this);

    setWindowTitle("Color Conundrum Instructions");
}


/**
 * destructor
 */
Instructions::~Instructions()
{
    delete ui;
}
