#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "commdef.h"
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void loadPixmap(const char *chessman);
    void selectChessman(const char *chessman, int move);
    void changeFlip();

public slots:
signals:
    void doMove(int index);

private:
    QPixmap background;
    QPixmap arrChessman[CHESSBOARD_ROW * CHESSBOARD_COLUMN];

    bool fliped;
};

#endif // CHESSBOARD_H
