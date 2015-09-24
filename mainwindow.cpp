#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commdef.h"
#include <QSound>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timerThread(this)
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
    stepInterval = 0;
    lastMoveInfo.reset();

    initActions();
}

void MainWindow::initActions()
{
    connect(ui->actionStart, SIGNAL(triggered()), this, SLOT(startGame()));
    connect(ui->actionNewGame, SIGNAL(triggered()), this, SLOT(newGame()));
    connect(ui->actionFlip, SIGNAL(triggered()), this, SLOT(flipChessBoard()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(fallback()));
    connect(ui->actionLose, SIGNAL(triggered()), this, SLOT(loseGame()));
    connect(ui->actionTie, SIGNAL(triggered()), this, SLOT(drawnGame()));
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

//保存棋局为FEN串
void MainWindow::save()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("保存棋局"), QString(), tr("Plain text Files(*.txt);;All Files (*)"));
    if (fn.isEmpty())
    {
        return;
    }
    if (! fn.endsWith(".txt",Qt::CaseInsensitive))
    {
        fn += ".txt";
    }

    QFile file(fn);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    out << MoveGenerator::chessmanToFEN(chessHandler->getChessman(), chessHandler->getCurrentTurn());
    file.close();
}

//从FEE串加载棋局
void MainWindow::open()
{
    QString fn = QFileDialog::getOpenFileName(this,tr("加载棋局"),QString(),tr("All Files(*);;Text Files(*.txt)"));
    QFile file(fn);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    if (line.isNull())
    {
        return;
    }

    char arrChessman[256];
    int currentTurn = 0;
    memset(arrChessman, 0, sizeof(arrChessman));
    MoveGenerator::FENTochessman(line, arrChessman, currentTurn);
    chessHandler->messGame(arrChessman, currentTurn);
}

void MainWindow::fallback()
{
    chessHandler->fallback();
}

void MainWindow::loseGame()
{
    chessHandler->loseGame();
}

void MainWindow::drawnGame()
{
    chessHandler->drawnGame();
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
    case EVENT_ILLEGAL_MOVE:
        processIllegalMoveEvent();
        break;
    case EVENT_FALLBACK:
        processFallbackEvent();
        break;
    case EVENT_GAME_RESULT:
        processGameResultEvent();
        break;
    default:
        break;
    }
}

void MainWindow::processNewGameEvent()
{
    chessBoard->loadPixmap(chessHandler->getChessman());
    chessBoard->update();
    leftStepList->clearHistoryDisplay();
    rightStepList->clearHistoryDisplay();
    gameOver = false;
    gameStarted = true;
    lastMoveInfo.reset();

    if (g_gameSettings.getStepTime() > 0)
    {
        timerThread.start();
        usleep(50);
        gameStartCond.wakeAll();
    }
}

void MainWindow::processUpdateMoveEvent()
{
    MoveInfo info = chessHandler->getCurrentMoveInfo();
    int gameResult = chessHandler->getGameResult();

    //如果移动了完整的一步，则需要先更新整个棋盘
    if (SRC(info.move) > 0 && DST(info.move) > 0)
    {
        chessBoard->loadPixmap(chessHandler->getChessman());
        addToStepList(info);
        if (gameResult == -1 && g_gameSettings.getStepTime() > 0)
        {
            stepOverCond.wakeAll();
        }
    }

    if (isSameSide(lastMoveInfo.movingChessman, info.movingChessman))
    {
        chessBoard->showMoveRoute(lastMoveInfo.movingChessman, lastMoveInfo.move, false);
    }
    chessBoard->showMoveRoute(info.movingChessman, info.move, true);
    chessBoard->update();

    playTipSound(info, gameResult);

    if (gameResult != -1)
    {
        if (gameResult != 0)
        {
            updateGeneralDisplay(gameResult);
        }

        showResultView(gameResult);
    }

    lastMoveInfo = info;
    gameOver = gameResult != -1;
}

void MainWindow::processIllegalMoveEvent()
{
    QSound::play(AUDIO_ILLEGAL);
}

void MainWindow::processFallbackEvent()
{
    chessBoard->loadPixmap(chessHandler->getChessman());
    chessBoard->update();
    lastMoveInfo = chessHandler->getCurrentMoveInfo();
    leftStepList->fallbackMoveHistory();
    rightStepList->fallbackMoveHistory();
}

void MainWindow::processGameResultEvent()
{
    int gameResult = chessHandler->getGameResult();
    gameOver = gameResult != -1;
    playGameResultSound(gameResult);
    showResultView(gameResult);
}

void MainWindow::playTipSound(const MoveInfo &info, int gameResult)
{
    if (gameResult != -1)
    {
        playGameResultSound(gameResult);
    }
    else
    {
        if (SRC(info.move) > 0 && DST(info.move) > 0)
        {
            if (info.attackGeneral)
            {
                QSound::play(AUDIO_ATTACK_KING);
            }
            else
            {
                QSound::play(info.killedChessman == 0 ? AUDIO_MOVE : AUDIO_EAT);
            }
        }
        else
        {
            QSound::play(AUDIO_CHOOSE);
        }
    }
}

void MainWindow::playGameResultSound(int gameResult)
{
    if (gameResult != TIE)
    {
        QSound::play(gameResult == g_gameSettings.getCompetitorSide() ? AUDIO_LOSS : AUDIO_WIN);
    }
    else
    {
        QSound::play(AUDIO_TIE);
    }
}

void MainWindow::addToStepList(const MoveInfo &info)
{
    bool blackSide = isBlackSide(info.movingChessman);
    if (blackSide)
    {
        rightStepList->addMoveHistory(info);
    }
    else
    {
        leftStepList->addMoveHistory(info);
    }
}

void MainWindow::updateGeneralDisplay(int gameResult)
{
    int pos = 0;
    switch(gameResult)
    {
    case BLACK:
        {
            if (chessHandler->getDeadOne() == RED &&
                (pos = chessHandler->getGeneralPos(RED_GENERAL)) > 0)
            {
                chessBoard->updateGeneralDisplay(RED_GENERAL, pos, true);
            }
        }
        break;
    case RED:
        {
            if (chessHandler->getDeadOne() == BLACK &&
                (pos = chessHandler->getGeneralPos(BLACK_GENERAL)) > 0)
            {
                chessBoard->updateGeneralDisplay(BLACK_GENERAL, pos, true);
            }
        }
        break;
    default:
        break;
    }

    chessBoard->update();
}

void MainWindow::showResultView(int gameResult)
{
    switch (gameResult)
    {
    case TIE:
        QMessageBox::about(NULL, tr("比赛结果"), tr("和局"));
        break;
    case BLACK:
        QMessageBox::about(NULL, tr("比赛结果"), tr("黑方胜"));
        break;
    case RED:
        QMessageBox::about(NULL, tr("比赛结果"), tr("红方胜"));
        break;
    default:
        break;
    }
}

void MainWindow::displayStepTime(int interval)
{
    if(ui->timeLeft)
    {
        ui->timeLeft->display(convertToTimeStr(interval));
    }
    if (ui->lblTurn)
    {
        ui->lblTurn->setText(chessHandler->getCurrentTurn() == BLACK ? tr("黑方走棋") : tr("红方走棋"));
    }
}

bool MainWindow::isGameOver()
{
    return gameOver;
}

void MainWindow::setGameOver(bool isGameOver)
{
    gameOver = isGameOver;
}

void MainWindow::stepTimeOver()
{
    chessHandler->stepTimeOver();
}

void MainWindow::setStepInterval(int interval)
{
    stepInterval = interval;
}

int MainWindow::getStepInterval()
{
    return stepInterval;
}

