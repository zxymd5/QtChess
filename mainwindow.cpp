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
    gameStarted = false;
    gameOver = false;

    initActions();
}

void MainWindow::initActions()
{
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startGame()));
    connect(ui->actionNewGame, SIGNAL(triggered()), this, SLOT(newGame()));
    connect(ui->actionFlip, SIGNAL(triggered()), this, SLOT(flipChessBoard()));
    connect(chessBoard, SIGNAL(doMove(int)), this, SLOT(doMove(int)));
    connect(chessHandler, SIGNAL(refreshGame(int)), this, SLOT(processEvent(int)));
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

void MainWindow::flipChessBoard()
{
    chessBoard->changeFlip();
    chessBoard->loadPixmap(chessHandler->getChessman());
    chessBoard->update();
}

void MainWindow::doMove(int index)
{
    chessHandler->doMove(index);
}

void MainWindow::processEvent(int event)
{
    switch (event)
    {
    case EVENT_NEW_GAME:
        processNewGameEvent();
        break;
    case EVENT_UPDATE_MOVE:
        processUpdateMoveEvent();
        break;
    default:
        break;
    }
}

void MainWindow::processNewGameEvent()
{
    chessBoard->loadPixmap(chessHandler->getChessman());
    chessBoard->update();
    gameOver = false;
    gameStarted = true;
}

void MainWindow::processUpdateMoveEvent()
{
    MoveInfo info = chessHandler->getCurrentMoveInfo();
    int gameResult = chessHandler->getGameResult();
    //const char *chessman = chessHandler->getChessman();

    //如果移动了完整的一步，则需要先更新整个棋盘
    if (SRC(info.move) > 0 && DST(info.move) > 0)
    {
        chessBoard->loadPixmap(chessHandler->getChessman());
    }
    chessBoard->selectChessman(chessHandler->getChessman(), info.move);
}

