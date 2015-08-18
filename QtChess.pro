#-------------------------------------------------
#
# Project created by QtCreator 2015-07-07T10:44:11
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtChess
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    chessboard.cpp \
    chesshandler.cpp \
    steplist.cpp \
    gamesettings.cpp \
    settingsdialog.cpp \
    rc4.cpp \
    zobrist.cpp \
    movegenerator.cpp

HEADERS  += \
    mainwindow.h \
    chessboard.h \
    chesshandler.h \
    steplist.h \
    commdef.h \
    gamesettings.h \
    settingsdialog.h \
    rc4.h \
    zobrist.h \
    movegenerator.h

FORMS    += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    qtchess.qrc
