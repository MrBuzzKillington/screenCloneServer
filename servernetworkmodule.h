#ifndef SERVERNETWORKMODULE_H
#define SERVERNETWORKMODULE_H

#include<QtNetwork/QUdpSocket>
#include<QtNetwork/QHostAddress>
#include<QtNetwork/QLocalSocket>

class ServerNetworkModule
{
public:
    ServerNetworkModule(); //QHostAddress addr, quint16 port



private:
    //QUdpSocket* socketPtr_;
    int imageSeq_;
    QHostAddress clientAddr_;
    int clientPort_;

};

#endif // SERVERNETWORKMODULE_H
