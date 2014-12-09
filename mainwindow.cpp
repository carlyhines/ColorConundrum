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
#include "instructions.h"

/**
 *default constructor
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //load in character graphics
    mainchar = new QPixmap("/PIC 10C/Homework/ColorConundrum/mainchar.png");
    ui->mainchar_label->setPixmap(*mainchar);
    ui->mainchar_label->setFixedSize(40,40);

    enemychar = new QPixmap("/PIC 10C/Homework/ColorConundrum/enemychar.jpg");
    ui->enemychar_label->setPixmap(*enemychar);


    //set window title
    setWindowTitle("Color Conundrum");


    //connect difficulty level buttons on mainwindow to create and show their respective game boards
    QObject::connect(ui->easyButton, SIGNAL(clicked()), this, SLOT(createEasyGB()));
    QObject::connect(ui->easyButton, SIGNAL(clicked()), this, SLOT(hide()));

    QObject::connect(ui->mediumButton, SIGNAL(clicked()), this, SLOT(createMediumGB()));
    QObject::connect(ui->mediumButton, SIGNAL(clicked()), this, SLOT(hide()));

    QObject::connect(ui->difficultButton, SIGNAL(clicked()), this, SLOT(createDifficultGB()));
    QObject::connect(ui->difficultButton, SIGNAL(clicked()), this, SLOT(hide()));

    QObject::connect(ui->extremeButton, SIGNAL(clicked()), this, SLOT(createExtremeGB()));
    QObject::connect(ui->extremeButton, SIGNAL(clicked()), this, SLOT(hide()));


    //create instruction window
    Instructions* i = new Instructions();

    //connect instruction button to show instruction window
    QObject::connect(ui->instructionsButton, SIGNAL(clicked()), i, SLOT(show()));
}

/**
 *destructor
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * createEasyGB is a public slot that creates and displays a new gameboard at the easy difficulty level (8x8)
 */
void MainWindow::createEasyGB(){
    GameBoard* easyGB = new GameBoard(8, "Easy", 1, 5);
    easyGB->setMainWindow(this);
    easyGB->show();
}

/**
 * createMediumGB is a public slot that creates and displays a new gameboard at the medium difficulty level (10x10)
 */
void MainWindow::createMediumGB(){
    GameBoard* mediumGB = new GameBoard(9, "Medium", 1, 5);
    mediumGB->setMainWindow(this);
    mediumGB->show();
}

/**
 * createDifficultGB is a public slot that creates and displays a new gameboard at the difficult difficulty level (12x12)
 */
void MainWindow::createDifficultGB(){
    GameBoard* difficultGB = new GameBoard(10, "Difficult", 1, 5);
    difficultGB->setMainWindow(this);
    difficultGB->show();
}

/**
 * createExtremeGB is a public slot that creates and displays a new gameboard at the extreme difficulty level (14x14)
 */
void MainWindow::createExtremeGB(){
    GameBoard* extremeGB = new GameBoard(12, "Extreme", 1, 5);
    extremeGB->setMainWindow(this);
    extremeGB->show();
}

/**
 * BeatEasy is a public slot that displays COMPLETE on the menu next to the easy difficulty level if you have completed all five levels within that difficulty
 */
void MainWindow::BeatEasy(){
    ui->easyButton->setText("Easy (8x8 Board) COMPLETE");
    ui->easyButton->setStyleSheet("color : #F52887");
}

/**
 * BeatMedium is a public slot that displays COMPLETE on the menu next to the medium difficulty level if you have completed all five levels within that difficulty
 */
void MainWindow::BeatMedium(){
    ui->mediumButton->setText("Medium (9x9 Board) COMPLETE");
    ui->mediumButton->setStyleSheet("color : #F52887");
}

/**
 * BeatDifficult is a public slot that displays COMPLETE on the menu next to the difficult difficulty level if you have completed all five levels within that difficulty
 */
void MainWindow::BeatDifficult(){
    ui->difficultButton->setText("Difficult (10x10 Board) COMPLETE");
    ui->difficultButton->setStyleSheet("color : #F52887");
}

/**
 * BeatExtreme is a public slot that displays COMPLETE on the menu next to the extreme difficulty level if you have completed all five levels within that difficulty
 */
void MainWindow::BeatExtreme(){
    ui->extremeButton->setText("Extreme (12x12 Board) COMPLETE");
    ui->extremeButton->setStyleSheet("color : #F52887");
}
