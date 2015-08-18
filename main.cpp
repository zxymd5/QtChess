#include "mainwindow.h"
#include "gamesettings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_gameSettings.loadSettings();

    MainWindow w;
    w.show();

    g_gameSettings.saveSettings();
    return a.exec();
}
