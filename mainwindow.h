#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboard.h"
#include "steplist.h"
#include "chesshandler.h"
#include "settingsdialog.h"
#include "gamesettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initActions();
    void processNewGameEvent();
    void processUpdateMoveEvent();
    void processIllegalMoveEvent();
    void playTipSound(const MoveInfo &info, int gameResult);
    void playGameResultSound(int gameResult);
    void addToStepList(const MoveInfo &info);
    void updateGeneralDisplay(int gameResult);
    void showResultView(int gameResult);

    ~MainWindow();

private slots:
    void startGame();
    void newGame();
    void settings();
    void flipChessBoard();
    void save();
    void open();

    void doMove(int index);

    void processEvent(int event);

private:
    Ui::MainWindow *ui;
    ChessBoard *chessBoard;
    StepList *leftStepList;
    StepList *rightStepList;
    ChessHandler *chessHandler;
    SettingsDialog *settingsDialog;
    bool gameStarted;
    bool gameOver;
    MoveInfo lastMoveInfo;
};

#endif // MAINWINDOW_H
