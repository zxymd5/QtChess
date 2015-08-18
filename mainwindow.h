#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboard.h"
#include "steplist.h"
#include "chesshandler.h"
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initActions();
    ~MainWindow();

private slots:
    void startGame();
    void newGame();
    void settings();

private:
    Ui::MainWindow *ui;
    ChessBoard *chessBoard;
    StepList *leftStepList;
    StepList *rightStepList;
    ChessHandler *chessHandler;
    SettingsDialog *settingsDialog;
};

#endif // MAINWINDOW_H
