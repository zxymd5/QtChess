#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T10:44:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtChess
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    chessboard.cpp \
    chesshandler.cpp

HEADERS  += \
    mainwindow.h \
    chessboard.h \
    chesshandler.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    qtchess.qrc
