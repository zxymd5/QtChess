#include "steplist.h"
#include "commdef.h"

#include <QPainter>

StepList::StepList(QWidget *parent, bool left) : QWidget(parent)
{
    resize(STEPLIST_WIDTH, STEPLIST_HEIGHT);
    if(!left)
    {
        move(RIGHTSTEPLIST_STARTX, RIGHTSTEPLIST_STARTY);
        pixSide.load(tr(":/images/WOOD/BK.GIF"));
    }
    else
    {
        move(LEFTSTEPLIST_STARTX, LEFTSTEPLIST_STARTY);
        pixSide.load(tr(":/images/WOOD/RK.GIF"));
    }
    initPageInfo();
    initPushButtons();
    initTableView();
}

StepList::~StepList()
{

}

void StepList::initPushButtons()
{
    btnPrevPage = new QPushButton("<<", this);
    btnPrevPage->setGeometry(0, 570, 40, 40);
    btnPrevRecord = new QPushButton("<", this);
    btnPrevRecord->setGeometry(50, 570, 40, 40);
    btnNextRecord = new QPushButton(">", this);
    btnNextRecord->setGeometry(100, 570, 40, 40);
    btnNextPage = new QPushButton(">>", this);
    btnNextPage->setGeometry(150, 570, 40, 40);
}

void StepList::initPageInfo()
{
    lblPageInfo = new QLabel(this);
    lblPageInfo->setGeometry(0, 545, 200, 20);
    lblPageInfo->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    lblPageInfo->setText("当前页：0/0");
}

void StepList::initTableView()
{
    tableView = new QTableView(this);
    tableView->move(0, 60);

    model = new QStandardItemModel(15, 3);
    model->setHeaderData(0, Qt::Horizontal, tr("序号"));
    model->setHeaderData(1, Qt::Horizontal, tr("走法"));
    model->setHeaderData(2, Qt::Horizontal, tr("时间"));

    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setModel(model);
    tableView->setColumnWidth(0, 30);
    tableView->setColumnWidth(1, 70);
    tableView->setColumnWidth(2, 75);
    tableView->setFixedSize(200, 480);

    QModelIndex index;
    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            index = model->index(i, j, QModelIndex());
            model->setData(index, tr("炮四平五"));
        }
    }

    tableView->setCurrentIndex(index);
}

void StepList::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(72, 0, pixSide);
}

