#include "steplist.h"
#include "movegenerator.h"

#include <QPainter>
#include <QDateTime>
#include <algorithm>

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
    currentOrderNumber = 0;
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

    connect(btnPrevPage, SIGNAL(clicked()), this, SLOT(prevPage()));
    connect(btnPrevRecord, SIGNAL(clicked()), this, SLOT(prevRecord()));
    connect(btnNextRecord, SIGNAL(clicked()), this, SLOT(nextRecord()));
    connect(btnNextPage, SIGNAL(clicked()), this, SLOT(nextPage()));
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

//    QModelIndex index;
//    for(int i = 0; i < 15; i++)
//    {
//        for(int j = 0; j < 3; j++)
//        {
//            index = model->index(i, j, QModelIndex());
//            model->setData(index, tr("炮四平五"));
//        }
//    }

//    tableView->setCurrentIndex(index);
}

void StepList::addMoveHistory(const MoveInfo &info)
{
    ChineseMoveStep moveStep;
    moveStep.orderNumber = vecMoveHistory.size() + 1;
    MoveGenerator::alphaFmtToChiness(info.moveStepAlpha, moveStep.moveStepInfo, isBlackSide(info.movingChessman));
    moveStep.moveStepTime = QDateTime::currentDateTime().toString(tr("yyyy-MM-dd hh:mm:ss"));
    vecMoveHistory.push_back(moveStep);

    currentOrderNumber = vecMoveHistory.size();
    updateHistoryDisplay();
}

void StepList::clearHistoryDisplay()
{
    vecMoveHistory.clear();
    currentOrderNumber = 0;
    updateHistoryDisplay();
}

int StepList::getPageNumber(int orderNumber)
{
    return orderNumber % MOVE_STEP_PER_PAGE == 0 ? orderNumber / MOVE_STEP_PER_PAGE : orderNumber / MOVE_STEP_PER_PAGE + 1;
}

int StepList::getRecordIndex(int orderNumber)
{
    return orderNumber % MOVE_STEP_PER_PAGE == 0 ? MOVE_STEP_PER_PAGE : orderNumber % MOVE_STEP_PER_PAGE;
}

void StepList::prevRecord()
{
    currentOrderNumber--;
    updateHistoryDisplay();
}

void StepList::prevPage()
{
    currentOrderNumber = std::max(currentOrderNumber - MOVE_STEP_PER_PAGE, 1);
    updateHistoryDisplay();
}

void StepList::nextRecord()
{
    currentOrderNumber++;
    updateHistoryDisplay();
}

void StepList::nextPage()
{
    currentOrderNumber = std::min(currentOrderNumber + MOVE_STEP_PER_PAGE, vecMoveHistory.size());
    updateHistoryDisplay();
}

void StepList::updateHistoryDisplay()
{
    QModelIndex index;
    for (int i = 0; i < MOVE_STEP_PER_PAGE; ++i)
    {
        for (int j = 0; j < COLUMN_PER_STEP; ++j)
        {
            index = model->index(i, j, QModelIndex());
            model->setData(index, tr(""));
        }
    }

    int currentPage = getPageNumber(currentOrderNumber);
    int totalPage = getPageNumber(vecMoveHistory.size());
    int currentRecordIndex = getRecordIndex(currentOrderNumber);

    if (vecMoveHistory.size() == 0)
    {
        btnPrevPage->setEnabled(false);
        btnPrevRecord->setEnabled(false);
        btnNextRecord->setEnabled(false);
        btnNextPage->setEnabled(false);
    }
    else
    {
        int startIndex = (currentPage - 1) * MOVE_STEP_PER_PAGE;
        int endIndex = std::min(currentPage * MOVE_STEP_PER_PAGE, vecMoveHistory.size()) - 1;

        QModelIndex modelIndex;
        for (int i = startIndex; i <= endIndex; i++)
        {
            modelIndex = model->index(i, 0, QModelIndex());
            model->setData(modelIndex, vecMoveHistory.at(i).orderNumber);

            modelIndex = model->index(i, 1, QModelIndex());
            model->setData(modelIndex, vecMoveHistory.at(i).moveStepInfo);

            modelIndex = model->index(i, 2, QModelIndex());
            model->setData(modelIndex, vecMoveHistory.at(i).moveStepTime);
        }
        tableView->setCurrentIndex(modelIndex);

        btnPrevRecord->setEnabled(currentRecordIndex > 1);
        btnPrevPage->setEnabled(currentPage > 1);
        btnNextRecord->setEnabled(currentOrderNumber < vecMoveHistory.size());
        btnNextPage->setEnabled(currentPage < totalPage);
    }

    QString pageInfo;
    pageInfo.sprintf("当前页：%d/%d", currentPage, totalPage);
    lblPageInfo->setText(pageInfo);
}

void StepList::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(72, 0, pixSide);
}

