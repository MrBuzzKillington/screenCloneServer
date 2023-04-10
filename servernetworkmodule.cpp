#include "servernetworkmodule.h"
#include <QImage>
#include <QBuffer>

ServerNetworkModule::ServerNetworkModule(QObject *parent, int port):
    QTcpServer(parent),
    imageSeq_(0),
    clientList_()
{

    clientPort_ = port;
}

ServerNetworkModule::~ServerNetworkModule()
{
    qDebug() << "server removed";
}


void ServerNetworkModule::StartServer()
{
    if(listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server: started";
    }
    else
    {
        qDebug() << "Server: not started!";
    }
}

void ServerNetworkModule::incomingConnection(qintptr socketId)
{
    // at the incoming connection, make a client
    qDebug() << "New client connection";
    MyClient *client = new MyClient(this);
    client->SetSocket(socketId);
    clientList_.push_back(client);
}




void ServerNetworkModule::sendImage( QImage imgToSend )
{
    qDebug() << "send image size:" << imgToSend.sizeInBytes();
    QByteArray imageQBA;
    QBuffer buffer(&imageQBA);
    buffer.open(QIODevice::WriteOnly);
    imgToSend.save(&buffer, "BMP"); // writes image into ba in PNG format

    //QByteArray imageQBA;
    //bool flag{1};
    //QDataStream out(&imageQBA,QIODevice::ReadWrite);
    //out << imgToSend << flag;


     //qDebug() << "image size:" << imageQBA.size();
     //TODO check bound state first
     int maxPayload = 1200;
     int fragment = 0;
     int payloadSize = 0;
     quint8 packetType = 0;//0==first,1==mid,2==last

   //Generate the message
     QByteArray buf;
     QDataStream sBuff(&buf, QIODevice::WriteOnly);

     payloadSize = imageQBA.size();

     //reset the buffer
     buf.clear();
     sBuff << (quint16) 0x5C5C;
     sBuff << (qint32) imageSeq_;
     sBuff << (qint32) payloadSize;
     qDebug() << "sizewas:" << buf.size() << " image:" << imageQBA.size();
     sBuff << imageQBA;

     for (int i=0;i<clientList_.size();i++)
     {
         MyClient* tempClient = clientList_.at(0); //get the element
         clientList_.erase(clientList_.begin()); //remove it from the front
         if (tempClient->isConnected())
         {
             tempClient->SendImage(buf);
             clientList_.push_back(tempClient);
             //qDebug() << "Sent image. ClientList :" << clientList_.size()  << "bytes Sent " << bytesSent;
         }
     }

    imageSeq_++;
}

  int ServerNetworkModule::getClientCount()
  {
      //Purge the client list
      for (int i=0;i<clientList_.size();i++)
      {
          MyClient* tempClient = clientList_.at(0); //get the element
          clientList_.erase(clientList_.begin()); //remove it from the front
          if (tempClient->isConnected())
          {
              clientList_.push_back(tempClient);
          }
      }
      return clientList_.size();
  }
