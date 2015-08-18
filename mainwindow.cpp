#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commdef.h"
#include <QSound>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(WINDOW_STARTX, WINDOW_STARTY);
    chessBoard = new ChessBoard(this);
    leftStepList = new StepList(this, true);
    rightStepList = new StepList(this, false);
    chessHandler = new ChessHandler(this);
    settingsDialog = new SettingsDialog(this);
    settingsDialog->setVisible(false);

    initActions();
}

void MainWindow::initActions()
{
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startGame()));
    connect(ui->actionNewGame, SIGNAL(triggered()), this, SLOT(newGame()));
}

MainWindow::~MainWindow()
{
    delete chessBoard;
    delete leftStepList;
    delete rightStepList;
    delete chessHandler;
    delete ui;
}

void MainWindow::startGame()
{
    settingsDialog->updateDialog();
    settings();
    chessHandler->startGame();
}

void MainWindow::newGame()
{
    chessHandler->newGame();
    QSound::play(AUDIO_NEW_GAME);

}

void MainWindow::settings()
{
    if(settingsDialog->isVisible())
    {
        settingsDialog->activateWindow();
    }
    else
    {
        settingsDialog->show();
    }

    settingsDialog->exec();
}

