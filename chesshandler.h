#ifndef CHESSHANDLER_H
#define CHESSHANDLER_H

#include <QObject>

class ChessHandler : public QObject
{
    Q_OBJECT
public:
    explicit ChessHandler(QObject *parent = 0);
    ~ChessHandler();

signals:

public slots:
};

#endif // CHESSHANDLER_H
