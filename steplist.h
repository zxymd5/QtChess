#ifndef STEPLIST_H
#define STEPLIST_H

#include <QWidget>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QPaintEvent>

class StepList : public QWidget
{
    Q_OBJECT
public:
    explicit StepList(QWidget *parent = 0, bool left = true);
    ~StepList();
    void initPushButtons();
    void initPageInfo();
    void initTableView();

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
private:
    QPixmap pixSide;
    QTableView  *tableView;
    QStandardItemModel *model;
    QPushButton *btnPrevRecord;
    QPushButton *btnPrevPage;
    QPushButton *btnNextRecord;
    QPushButton *btnNextPage;
    QLabel *lblPageInfo;
};

#endif // STEPLIST_H
