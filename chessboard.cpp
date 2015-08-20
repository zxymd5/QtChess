#include "chessboard.h"
#include "commdef.h"
#include <QPainter>
#include <QDebug>

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
    resize(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
    move(CHESSBOARD_STARTX, CHESSBOARD_STARTY);
    fliped = false;
    loadPixmap(STARTUP_LAYOUT);
}

ChessBoard::~ChessBoard()
{

}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    int xx = 0;
    int yy = 0;
    int index = 0;

    QPainter painter(this);
    painter.drawPixmap(0, 0, background);

    for (int row = 0; row < CHESSBOARD_ROW; ++row)
    {
        for (int column = 0; column < CHESSBOARD_COLUMN; ++column)
        {
            index = row * CHESSBOARD_COLUMN + column;
            getPixmapPos(row, column, xx, yy);
            painter.drawPixmap(xx, yy ,arrChessman[index]);
        }
    }
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << event->pos().x() << tr(" ") << event->pos().y();
        int row = 0;
        int column = 0;
        getPixmapIndex(event->pos().x(), event->pos().y(), row, column);

        int sq = getChessmanIndex(row, column, fliped);
        emit doMove(sq);
//        int pc = STARTUP_LAYOUT[sq];
//        if(pc != 0)
//        {
//            arrChessman[row * CHESSBOARD_COLUMN + column].load(getChessmanPic(pc, true, false));
//        }

//        update();
    }
}

void ChessBoard::loadPixmap(const char *chessman)
{
    int sq = 0;
    char pc = 0;
    int index = 0;

    background.load(":/images/WOOD.GIF");
    for(int row = 0; row < CHESSBOARD_ROW; ++row)
    {
        for (int column = 0; column < CHESSBOARD_COLUMN; ++column)
        {
            index = row * CHESSBOARD_COLUMN + column;
            sq = getChessmanIndex(row, column, fliped);
            pc = chessman[sq];
            if (pc != 0)
            {
                arrChessman[index].load(getChessmanPic(pc, false, false));
            }
            else
            {
                arrChessman[index].fill(Qt::transparent);
            }

        }
    }

}

void ChessBoard::changeFlip()
{
    fliped = !fliped;
}
