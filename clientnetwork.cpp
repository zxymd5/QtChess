#include "clientnetwork.h"
#include "networkmsgdef.h"
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QHostAddress>
#include <QMessageBox>

ClientNetwork::ClientNetwork(QObject *parent) : QObject(parent)
{
}

ClientNetwork::~ClientNetwork()
{

}

void ClientNetwork::initClient(QString serverIP, int port)
{
    clientSocket = new QTcpSocket();
    connect(clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(clientSocket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    QHostAddress addr(serverIP);
    clientSocket->connectToHost(addr, port);
}

int ClientNetwork::sendMsg(char *msg, int len)
{
    return clientSocket->write(msg, len);
}

void ClientNetwork::connected()
{
//    qDebug() << "connected";
//    QString strToSend;
//    strToSend.append("GET HTTP/1.1\r\n");
//    strToSend.append("Host: 180.97.33.108\r\n");
//    strToSend.append("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0\r\n");
//    strToSend.append("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
//    strToSend.append("Connection: keep-alive\r\n");
//    clientSocket->write(strToSend.toLatin1(), strToSend.length());
    //QMessageBox::about(NULL, tr("提示信息"), tr("已成功连接上服务器！"));
    emit connStateChanged(true);
    MsgReqGameInfo msg;
    clientSocket->write((char *)&msg, sizeof(msg));
}

void ClientNetwork::disconnected()
{
    emit connStateChanged(false);
    qDebug() << "disconnected";
}

void ClientNetwork::readData()
{
    qDebug() << "readData";
    QByteArray data;
    QString strData;
    char msg[1024];
    memset(msg, 0, 1024);
    int len = clientSocket->read(msg, 1024);
//    while (clientSocket->bytesAvailable()) {
//        data = clientSocket->readLine();
//        strData.append(data);
//    }

//    qDebug() << strData;
    emit processMessage(msg, len);
}

void ClientNetwork::displayError(QAbstractSocket::SocketError err)
{
    qDebug() << err;
}

