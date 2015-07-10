#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include<QFrame>

class ChessBoard : public QFrame
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = 0);
    ~ChessBoard();
};

#endif // CHESSBOARD_H
