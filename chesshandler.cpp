#include "chesshandler.h"
#include "gamesettings.h"
#include <assert.h>

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
    resetChessmanLayout();
    emit refreshGame(EVENT_NEW_GAME);
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
    for (int i = 0; i < 256; ++i)
    {
        if(STARTUP_LAYOUT[i] != 0)
        {
            addChessman(i, STARTUP_LAYOUT[i]);
        }
    }
}

void ChessHandler::addChessman(int index, char chessmanType)
{
    arrChessman[index] = chessmanType;
    currentZobrist.Xor(zobristTable[chessmanType - 1][index]);
    if (isBlackSide(chessmanType))
    {
        blackValue += CHESSMAN_VALUE[chessmanType - 1][index];
    }
    else
    {
        redValue += CHESSMAN_VALUE[chessmanType - 8][SQUARE_FLIP(index)];
    }
}

void ChessHandler::delChessman(int index, char chessmanType)
{
    arrChessman[index] = 0;
    currentZobrist.Xor(zobristTable[chessmanType - 1][index]);
    if (isBlackSide(chessmanType))
    {
        blackValue -= CHESSMAN_VALUE[chessmanType - 1][index];
    }
    else
    {
        redValue -= CHESSMAN_VALUE[chessmanType - 8][SQUARE_FLIP(index)];
    }
}

void ChessHandler::doMove(int index)
{
    assert(index >= 0x33 && index <= 0xcb);
    int fromPos = SRC(currentMoveInfo.move);
    int toPos = DST(currentMoveInfo.move);

    if (fromPos == index || toPos == index)
    {
        return;
    }

    bool legal = false;
    if (currentTurn == RED)
    {
        legal = redDoMove(index);
    }
    else
    {
        legal = blackDoMove(index);
    }

    if (legal)
    {
        if (SRC(currentMoveInfo.move) > 0 && DST(currentMoveInfo.move) > 0)
        {

        }
        else
        {
        }
    }
    else
    {
    }
}

bool ChessHandler::redDoMove(int index)
{
    int killedChessman = arrChessman[index];
    int movingChessman = currentMoveInfo.movingChessman;
    bool legal = false;
    if (movingChessman == 0)
    {
        if (isRedSide(killedChessman))
        {
            currentMoveInfo.movingChessman = killedChessman;
            currentMoveInfo.move = index;
            legal = true;
        }
    }
    else
    {
        if (isRedSide(killedChessman))
        {
            currentMoveInfo.movingChessman = killedChessman;
            currentMoveInfo.move = index;
            legal = true;
        }
        else
        {
            //判断走法是否合理
            if(moveGenerator.validateMove(arrChessman, SRC(currentMoveInfo.move), index))
            {
                //再判断走棋后，自己是否被对方将军，如果自己被对方将军，则走法不合理
                arrChessman[SRC(currentMoveInfo.move)] = 0;
                arrChessman[index] = movingChessman;
                if (moveGenerator.isAttackGeneral(arrChessman, RED_GENERAL))
                {
                    legal = false;
                }
                else
                {
                    //再判断自己是否将对方的军
                    currentMoveInfo.attackGeneral = moveGenerator.isAttackGeneral(arrChessman, BLACK_GENERAL);
                    legal = true;
                }

                arrChessman[SRC(currentMoveInfo.move)] = movingChessman;
                arrChessman[index] = killedChessman;

                if (legal)
                {
                    currentMoveInfo.killedChessman = killedChessman;
                    currentMoveInfo.move = MOVE(SRC(currentMoveInfo.move), index);
                    moveGenerator.getMoveStepAlpha(arrChessman, currentMoveInfo.move, currentMoveInfo.moveStepAlpha);
                }
            }
            else
            {
                legal = false;
            }
        }
    }

    return legal;
}

bool ChessHandler::blackDoMove(int index)
{
    int killedChessman = arrChessman[index];
    int movingChessman = currentMoveInfo.movingChessman;
    bool legal = false;
    if (movingChessman == 0)
    {
        if (isBlackSide(killedChessman))
        {
            currentMoveInfo.movingChessman = killedChessman;
            currentMoveInfo.move = index;
            legal = true;
        }
    }
    else
    {
        if (isBlackSide(killedChessman))
        {
            currentMoveInfo.movingChessman = killedChessman;
            currentMoveInfo.move = index;
            legal = true;
        }
        else
        {
            //判断走法是否合理
            if(moveGenerator.validateMove(arrChessman, SRC(currentMoveInfo.move), index))
            {
                //再判断走棋后，自己是否被对方将军，如果自己被对方将军，则走法不合理
                arrChessman[SRC(currentMoveInfo.move)] = 0;
                arrChessman[index] = movingChessman;
                if (moveGenerator.isAttackGeneral(arrChessman, BLACK_GENERAL))
                {
                    legal = false;
                }
                else
                {
                    //再判断自己是否将对方的军
                    currentMoveInfo.attackGeneral = moveGenerator.isAttackGeneral(arrChessman, RED_GENERAL);
                    legal = true;
                }

                arrChessman[SRC(currentMoveInfo.move)] = movingChessman;
                arrChessman[index] = killedChessman;

                if (legal)
                {
                    currentMoveInfo.killedChessman = killedChessman;
                    currentMoveInfo.move = MOVE(SRC(currentMoveInfo.move), index);
                    moveGenerator.getMoveStepAlpha(arrChessman, currentMoveInfo.move, currentMoveInfo.moveStepAlpha);
                }
            }
            else
            {
                legal = false;
            }
        }
    }

    return legal;
}

void ChessHandler::applyMove()
{
    doMakeMove(currentMoveInfo);

}

void ChessHandler::doMakeMove(MoveInfo &info, bool record)
{
    int fromPos = SRC(info.move);
    int toPos = DST(info.move);
    char movingChessman = arrChessman[fromPos];
    char killedChessman = arrChessman[toPos];

    if (killedChessman > 0)
    {
        delChessman(toPos, killedChessman);
    }

    delChessman(fromPos, movingChessman);
    addChessman(toPos, movingChessman);
    info.zobristKey = currentZobrist.key;

    if (record)
    {
        lstMoveInfo.push_back(info);
        currentTurn = currentTurn == BLACK ? RED : BLACK;
    }
    currentSearchMoveTurn = currentSearchMoveTurn == BLACK ? RED : BLACK;

    currentZobrist.Xor(initZobrist);
}

const char *ChessHandler::getChessman()
{
    return arrChessman;
}

