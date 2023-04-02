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
   // qDebug() << "send image";

    QByteArray imageQBA;
    bool flag{1};
    QDataStream out(&imageQBA,QIODevice::ReadWrite);
    out << imgToSend << flag;


     //qDebug() << "image size:" << imageQBA.size();
     //TODO check bound state first
     int maxPayload = 1200;
     int fragment = 0;
     int bytesSent = 0;
     int payloadSize = 0;
     quint8 packetType = 0;//0==first,1==mid,2==last

   //Generate the message
     QByteArray buf;
     QDataStream sBuff(&buf, QIODevice::WriteOnly);

     bool messageDone = false;
     while (messageDone == false )
     {
        //break the image up into paylaod sized messages
        if (imageQBA.size() > maxPayload)
        {
            payloadSize = maxPayload;
        } else {
            payloadSize = imageQBA.size();
            messageDone=true;
            packetType = 2;
        }
        //reset the buffer
        buf.clear();
        QDataStream sBuff(&buf, QIODevice::WriteOnly);
        //qDebug() << "buff size after clear" << buf.size();
        //Push on the values
        //int sequence,frag#,PayloadSize,payload
        sBuff << (quint8) packetType;
        sBuff << (qint32)imageSeq_;
        //qDebug() << "buff size with seq:" << buf.size();
        sBuff << (qint32)fragment;
        //qDebug() << "buff size with seq/frag:" << buf.size();
        sBuff << (qint32)payloadSize;
            //qDebug() << "buff size with seq/frag/pay:" << buf.size();
        sBuff << imageQBA.first(payloadSize);
        imageQBA.remove(0,payloadSize);
            //qDebug() << "buff size" << buf.size() << " payload" << payloadSize << " imageSize" << imageQBA.size();
        //Send the data
         socketPtr_->writeDatagram(buf, clientAddr_, clientPort_);
         fragment++;
         packetType = 1;
         bytesSent = bytesSent + buf.size();
         //qDebug()<< "Sent" << buf.size() << " bytes for seq:" << imageSeq_;
     }  //q8(type),Q32(seq),q32(frag),q32(payloadsize),qba(image payload)

    imageSeq_++;
}
