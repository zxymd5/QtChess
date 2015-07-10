#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QPainter>
#include <QImage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    QStandardItemModel *model = new QStandardItemModel(10, 3);
    model->setHeaderData(0, Qt::Horizontal, tr("序号"));
    model->setHeaderData(1, Qt::Horizontal, tr("走法"));
    model->setHeaderData(2, Qt::Horizontal, tr("时间"));

    ui->leftTableView->setModel(model);
    ui->leftTableView->setColumnWidth(0, 30);
    ui->leftTableView->setColumnWidth(1, 70);
    ui->leftTableView->setColumnWidth(2, 70);

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            QModelIndex index = model->index(i, j, QModelIndex());
            model->setData(index, tr("炮四平五"));
        }
    }

    QPixmap image;
    image.load( ":/images/WOOD/BA.GIF" );

    QPainter painter(this);
    painter.drawPixmap(300,300, image);

}
