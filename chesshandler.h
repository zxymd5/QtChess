#ifndef CHESSHANDLER_H
#define CHESSHANDLER_H

#include "commdef.h"
#include "rc4.h"
#include "zobrist.h"
#include "movegenerator.h"
#include <QObject>
#include <QList>

class ChessHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChessHandler(QObject *parent = 0);
    ~ChessHandler();
    void startGame();
    void newGame();
    void reset();
    void resetZobrist();
    void resetChessmanLayout();
    void addChessman(int index, char chessmanType);

signals:

public slots:

private:
    char arrChessman[256];
    int currentTurn;
    int currentSearchMoveTurn;
    int gameResult;
    char whoIsDead;
    MoveGenerator moveGenerator;
    QList<MoveInfo> lstMoveInfo;
    MoveInfo currentMoveInfo;

    //子力值
    int blackValue;
    int redValue;

    //用于重复局面检测
    RC4    rc4;
    Zobrist currentZobrist;
    Zobrist initZobrist;
    Zobrist zobristTable[CHESSMAN_TYPE_COUNT][256];

};

#endif // CHESSHANDLER_H
