#include "movegenerator.h"
#include "commdef.h"
#include <math.h>

MoveGenerator::MoveGenerator()
{

}

MoveGenerator::~MoveGenerator()
{

}

bool MoveGenerator::validateMove(const char *arrChessman, int srcPos, int dstPos)
{
    if (srcPos == dstPos)
    {
        return false;
    }

    int movingChessman = arrChessman[srcPos];
    int killedChessman = arrChessman[dstPos];

    if (isSameSide(movingChessman, killedChessman))
    {
        return false;
    }

    bool legal = false;

    switch (movingChessman)
    {
    case BLACK_GENERAL:
    case RED_GENERAL:
        legal = validateGeneralMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_ADVISOR:
    case RED_ADVISOR:
        legal = validateAdvisorMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_MINISTER:
    case RED_MINISTER:
        legal = validateMinisterMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_HORSE:
    case RED_HORSE:
        legal = validateHorseMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_CHARIOT:
    case RED_CHARIOT:
        legal = validateChariotMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_CANNON:
    case RED_CANNON:
        legal = validateCannonMove(arrChessman, srcPos, dstPos);
        break;
    case BLACK_SOLDIER:
    case RED_SOLDIER:
        legal = validateSoldierMove(arrChessman, srcPos, dstPos);
        break;
    default:
        break;
    }

    return legal;
}

bool MoveGenerator::validateGeneralMove(const char *arrChessman, int srcPos, int dstPos)
{
    return IN_FORT(dstPos) && KING_SPAN(srcPos, dstPos);
}

bool MoveGenerator::validateAdvisorMove(const char *arrChessman, int srcPos, int dstPos)
{
    return IN_FORT(dstPos) && ADVISOR_SPAN(srcPos, dstPos);
}

bool MoveGenerator::validateMinisterMove(const char *arrChessman, int srcPos, int dstPos)
{
    return SAME_HALF(srcPos, dstPos) && BISHOP_SPAN(srcPos, dstPos) &&
            arrChessman[BISHOP_PIN(srcPos, dstPos)] == 0;
}

bool MoveGenerator::validateHorseMove(const char *arrChessman, int srcPos, int dstPos)
{
    int sqPin = KNIGHT_PIN(srcPos, dstPos);
    return sqPin != srcPos && arrChessman[sqPin] == 0;
}

bool MoveGenerator::validateChariotMove(const char *arrChessman, int srcPos, int dstPos)
{
    int delta = 0;
    if (SAME_RANK(srcPos, dstPos))
    {
        delta = (dstPos < srcPos ? -1 : 1);
    }
    else if(SAME_FILE(srcPos, dstPos))
    {
        delta = (dstPos < srcPos ? -16 : 16);
    }
    else
    {
        return false;
    }

    int sqPin = srcPos + delta;
    while (sqPin != dstPos && arrChessman[sqPin] == 0)
    {
        sqPin += delta;
    }

    return sqPin == dstPos;
}

bool MoveGenerator::validateCannonMove(const char *arrChessman, int srcPos, int dstPos)
{
    int delta = 0;

    if (SAME_RANK(srcPos, dstPos))
    {
        delta = (dstPos < srcPos ? -1 : 1);
    }
    else if(SAME_FILE(srcPos, dstPos))
    {
        delta = (dstPos < srcPos ? -16 : 16);
    }
    else
    {
        return false;
    }

    int sqPin = srcPos + delta;
    while (sqPin != dstPos && arrChessman[sqPin] == 0)
    {
        sqPin += delta;
    }

    if (sqPin == dstPos)
    {
        return arrChessman[dstPos] == 0;
    }

    sqPin += delta;
    while (sqPin != dstPos && arrChessman[sqPin] == 0)
    {
        sqPin += delta;
    }

    return sqPin == dstPos && arrChessman[dstPos] != 0;
}

bool MoveGenerator::validateSoldierMove(const char *arrChessman, int srcPos, int dstPos)
{
    int side = isBlackSide(arrChessman[srcPos]) ? 1 : 0;
    if (AWAY_HALF(dstPos, side) && (dstPos == srcPos - 1 || dstPos == srcPos + 1))
    {
      return true;
    }
    return dstPos == SQUARE_FORWARD(srcPos, side);
}

bool MoveGenerator::isAttackGeneral(const char *arrChessman, char attackedGeneral)
{
    int pos[5];
    getChessmanPos(arrChessman, attackedGeneral, pos);
    bool attacked = false;

    if (attackedGeneral == BLACK_GENERAL)
    {
        attacked = attackedByGeneral(arrChessman, pos[0], RED_GENERAL) ||
                   attackGeneral(arrChessman, pos[0], RED_CHARIOT) ||
                   attackGeneral(arrChessman, pos[0], RED_CANNON) ||
                   attackGeneral(arrChessman, pos[0], RED_HORSE) ||
                   attackGeneral(arrChessman, pos[0], RED_SOLDIER);
    }
    else
    {
        attacked = attackedByGeneral(arrChessman, pos[0], BLACK_GENERAL) ||
                attackGeneral(arrChessman, pos[0], BLACK_CHARIOT) ||
                attackGeneral(arrChessman, pos[0], BLACK_CANNON) ||
                attackGeneral(arrChessman, pos[0], BLACK_HORSE) ||
                attackGeneral(arrChessman, pos[0], BLACK_SOLDIER);
    }

    return attacked;
}

bool MoveGenerator::attackGeneral(const char *arrChessman, int attackedPos, char chessman)
{
    int chessmanCount = 0;
    int pos[5];

    chessmanCount = getChessmanPos(arrChessman, chessman, pos);
    for (int i = 0; i < chessmanCount; ++i)
    {
        if (validateMove(arrChessman, pos[i], attackedPos))
        {
            return true;
        }
    }

    return false;
}

bool MoveGenerator::attackedByGeneral(const char *arrChessman, int attackedPos, char attackingGeneral)
{
    int pos[5];
    getChessmanPos(arrChessman, attackingGeneral, pos);
    if (!IN_FORT(pos[0]))
    {
        return false;
    }

    if (!SAME_FILE(attackedPos, pos[0]))
    {
        return false;
    }

    int delta = attackedPos > pos[0] ? 16 : -16;
    int sqPin = pos[0] + delta;

    while (sqPin != attackedPos && arrChessman[sqPin] == 0)
    {
        sqPin += delta;
    }

    return sqPin == attackedPos;
}

int MoveGenerator::getChessmanPos(const char *arrChessman, char chessman, int pos[])
{
    int posCount = 0;
    for (int i = FILE_LEFT; i <= FILE_RIGHT; ++i)
    {
        for (int j = RANK_TOP; j <= RANK_BOTTOM; ++j)
        {
            int sq = COORD_XY(i, j);
            if (arrChessman[sq] == chessman)
            {
                pos[posCount] = sq;
                posCount++;
            }
        }
    }

    return posCount;
}

QString MoveGenerator::digitToChinese(const QChar &digit, bool black)
{
    QString strDigit;
    if (black)
    {
        switch(digit.toLatin1())
        {
        case '1':
            strDigit = "1";
            break;
        case '2':
            strDigit = "2";
            break;
        case '3':
            strDigit = "3";
            break;
        case '4':
            strDigit = "4";
            break;
        case '5':
            strDigit = "5";
            break;
        case '6':
            strDigit = "6";
            break;
        case '7':
            strDigit = "7";
            break;
        case '8':
            strDigit = "8";
            break;
        case '9':
            strDigit = "9";
            break;
        case 'a':
            strDigit = "前";
            break;
        case 'b':
            strDigit = "后";
            break;
        case 'c':
            strDigit = "一";
            break;
        case 'd':
            strDigit = "二";
            break;
        case 'e':
            strDigit = "三";
            break;
        case 'f':
            strDigit = "四";
            break;
        case 'g':
            strDigit = "五";
            break;
        case 'q':
            strDigit = "前";
            break;
        case 'z':
            strDigit = "中";
            break;
        case 'h':
            strDigit = "后";
            break;
        default:
            break;
        }
    }
    else
    {
        switch(digit.toLatin1())
        {
        case '1':
            strDigit = "一";
            break;
        case '2':
            strDigit = "二";
            break;
        case '3':
            strDigit = "三";
            break;
        case '4':
            strDigit = "四";
            break;
        case '5':
            strDigit = "五";
            break;
        case '6':
            strDigit = "六";
            break;
        case '7':
            strDigit = "七";
            break;
        case '8':
            strDigit = "八";
            break;
        case '9':
            strDigit = "九";
            break;
        case 'a':
            strDigit = "前";
            break;
        case 'b':
            strDigit = "后";
            break;
        case 'c':
            strDigit = "一";
            break;
        case 'd':
            strDigit = "二";
            break;
        case 'e':
            strDigit = "三";
            break;
        case 'f':
            strDigit = "四";
            break;
        case 'g':
            strDigit = "五";
            break;
        case 'q':
            strDigit = "前";
            break;
        case 'z':
            strDigit = "中";
            break;
        case 'h':
            strDigit = "后";
            break;
        default:
            break;
        }
    }

    return strDigit;
}

QString MoveGenerator::chessManCodeToChinese(const QChar &code)
{
    QString strCode;
    switch(code.toLatin1())
    {
    case 'k':
        strCode = "将";
        break;
    case 'r':
        strCode = "车";
        break;
    case 'n':
        strCode = "马";
        break;
    case 'c':
        strCode = "炮";
        break;
    case 'a':
        strCode = "士";
        break;
    case 'b':
        strCode = "象";
        break;
    case 'p':
        strCode = "卒";
        break;
    case 'K':
        strCode = "帅";
        break;
    case 'R':
        strCode = "车";
        break;
    case 'N':
        strCode = "马";
        break;
    case 'C':
        strCode = "炮";
        break;
    case 'A':
        strCode = "仕";
        break;
    case 'B':
        strCode = "相";
        break;
    case 'P':
        strCode = "兵";
        break;
    default:
        break;
    }

    return strCode;
}

QString MoveGenerator::actionToChinese(const QChar &action)
{
    QString strAction;
    switch(action.toLatin1())
    {
    case '.':
        strAction = "平";
        break;
    case '+':
        strAction = "进";
        break;
    case '-':
        strAction = "退";
        break;
    default:
        break;
    }

    return strAction;
}

void MoveGenerator::alphaFmtToChiness(const QString &alphaFmt, QString &chineseFmt, bool black)
{
    if (alphaFmt[1] >= 'a')
    {
        chineseFmt = digitToChinese(alphaFmt[1], black) +
                 chessManCodeToChinese(alphaFmt[0]) +
                 actionToChinese(alphaFmt[2]) +
                 digitToChinese(alphaFmt[3], black);
    }
    else
    {
        chineseFmt = chessManCodeToChinese(alphaFmt[0]) +
                 digitToChinese(alphaFmt[1], black) +
                 actionToChinese(alphaFmt[2]) +
                 digitToChinese(alphaFmt[3], black);
    }
}

