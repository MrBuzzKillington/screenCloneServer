#include "servernetworkmodule.h"
#include <QImage>

ServerNetworkModule::ServerNetworkModule(QHostAddress addr, int port):
    socketPtr_(),
    imageSeq_(0)
{

    clientAddr_ = addr;
    clientPort_ = port;

    socketPtr_.reset( new QUdpSocket() );

}
ServerNetworkModule::~ServerNetworkModule()
{

}

void ServerNetworkModule::sendImage( QImage imgToSend )
{
    qDebug() << "send image";
    QByteArray imageQBA = QByteArray::fromRawData( (const char*)imgToSend.bits(), imgToSend.sizeInBytes() );
     imageQBA.append( imgToSend.height() );
     imageQBA.append( imgToSend.width() );
     imageQBA.append( imageSeq_ );
     imageSeq_++;

     //TODO check bound state first
     socketPtr_->writeDatagram(imageQBA, clientAddr_, clientPort_);

}
