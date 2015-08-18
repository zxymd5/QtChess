#include "chesshandler.h"
#include "gamesettings.h"

ChessHandler::ChessHandler(QObject *parent) : QObject(parent)
{

}

ChessHandler::~ChessHandler()
{

}

void ChessHandler::startGame()
{

}

void ChessHandler::newGame()
{
    reset();
    resetZobrist();
    currentMoveInfo.reset();

}

void ChessHandler::reset()
{
    currentTurn = g_gameSettings.getAhead();
    currentSearchMoveTurn = currentTurn;
    gameResult = -1;
    whoIsDead = 0;
    lstMoveInfo.clear();
    blackValue = 0;
    redValue = 0;
    memset(arrChessman, 0, sizeof(arrChessman));
}

void ChessHandler::resetZobrist()
{
    rc4.init();
    initZobrist.init(rc4);
    for (int i = 0; i < 14; ++i)
    {
        for (int j = 0; j < 256; ++j)
        {
            zobristTable[i][j].init(rc4);
        }
    }

    currentZobrist.reset();
}

void ChessHandler::resetChessmanLayout()
{

}

void ChessHandler::addChessman(int index, char chessmanType)
{
    arrChessman[index] = chessmanType;
    currentZobrist.Xor(zobristTable[chessmanType - 1][index]);
    if (chessmanType < 8)
    {
        blackValue += cucvlPiecePos[chessmanType][index];
    }
    else
    {
        redValue += cucvlPiecePos[chessmanType - 7][SQUARE_FLIP(index)];
    }
}

