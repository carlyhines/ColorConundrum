#-------------------------------------------------
# Carly Hines
# Homework 9 - Final Game Project
# 12/10/2014
# PIC 10C Fall 2014
# Color Conundrum Game
# Move around the board, avoid enemies, and change colors of spaces until all spaces are the same color!
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorConundrum4b
TEMPLATE = app


SOURCES += \
    mainwindow.cpp \
    main.cpp \
    gameboard.cpp \
    instructions.cpp

HEADERS  += \
    mainwindow.h \
    gameboard.h \
    instructions.h

FORMS    += \
    instructions.ui \
    mainwindow.ui

macx {
    QMAKE_MAC_SDK = macosx10.9
}

CONFIG += c++11
