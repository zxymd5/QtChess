#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QMessageBox>
#include "servernetwork.h"

ServerNetwork::ServerNetwork(QObject *parent) : QObject(parent)
{
    serverSocket = NULL;
}

ServerNetwork::~ServerNetwork()
{
    if (server != NULL)
    {
        delete server;
        server = NULL;
    }

    if (serverSocket != NULL)
    {
        delete serverSocket;
        serverSocket = NULL;
    }

}

void ServerNetwork::initServer(int port)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any, port))
    {
        QMessageBox::critical(NULL, tr("错误信息"), tr("监听端口失败！"));
    }
}

int ServerNetwork::sendMsg(char *msg, int len)
{
    return serverSocket->write(msg, len);
}

void ServerNetwork::newConnection()
{
    serverSocket = server->nextPendingConnection();
    connect(serverSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(serverSocket, SIGNAL(disconnected()), serverSocket, SLOT(deleteLater()));
    connect(serverSocket, SIGNAL(readyRead()), this, SLOT(readData()));

    //QMessageBox::about(NULL, tr("提示信息"), tr("已成功连接上客户端！"));
    emit connStateChanged(true);
}

void ServerNetwork::disconnected()
{
    emit connStateChanged(false);
    qDebug() << "disconnected";
}

void ServerNetwork::readData()
{
     QByteArray data;
     QString strData;

    while (serverSocket->bytesAvailable()) {
        data = serverSocket->readLine();
        strData.append(data);
    }

    emit processMessage(strData, strData.length());
//    qDebug() << "ReadData:" << strData;
//    serverSocket->write(strData.toLatin1(), strData.length());
}

