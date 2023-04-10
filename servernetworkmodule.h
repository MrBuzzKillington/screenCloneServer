#ifndef SERVERNETWORKMODULE_H
#define SERVERNETWORKMODULE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "myclient.h"

class ServerNetworkModule : public QTcpServer
{    
    Q_OBJECT

public:

    explicit ServerNetworkModule(QObject *parent = 0, int port = 1234); //QHostAddress addr, quint16 port
    ~ServerNetworkModule();
    void StartServer();

    void sendImage( QImage imgToSend );
    int getClientCount();

protected:
    void incomingConnection(qintptr socketId);

private:
    int imageSeq_;
    int clientPort_;
    std::vector<MyClient*> clientList_;
};

#endif // SERVERNETWORKMODULE_H
