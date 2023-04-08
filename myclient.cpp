#include "myclient.h"

MyClient::MyClient(QObject *parent) :
    QObject(parent)
{
   // QThreadPool::globalInstance()->setMaxThreadCount(5);
}

void MyClient::SetSocket(qintptr Descriptor)
{
    // make a new socket
    socketOpen_ = false;
    socket = new QTcpSocket(this);

    qDebug() << "A new socket created!";

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    socket->setSocketDescriptor(Descriptor);
    socketOpen_ = true;

    qDebug() << " Client connected at " << Descriptor;
}


// asynchronous - runs separately from the thread we created
void MyClient::connected()
{
    qDebug() << "Client connected event";

}

// asynchronous
void MyClient::disconnected()
{
    qDebug() << "Client disconnected";
    socketOpen_ = false;
}

// Our main thread of execution
// This happening via main thread
// Our code running in our thread not in Qthread
// That's why we're getting the thread from the pool

void MyClient::readyRead()
{
    //qDebug() << "MyClient::readyRead()";
    qDebug() << socket->readAll();

    // Time consumer
   // MyTask *mytask = new MyTask();
   // mytask->setAutoDelete(true);
   // connect(mytask, SIGNAL(Result(int)), this, SLOT(TaskResult(int)), Qt::QueuedConnection);

   // qDebug() << "Starting a new task using a thread from the QThreadPool";
  //  QThreadPool::globalInstance()->start(mytask);

}

// After a task performed a time consuming task.
// We grab the result here, and send it to client
void MyClient::TaskResult(int Number)
{
    QByteArray Buffer;
    Buffer.append("\r\nTask result = ");
    Buffer.append(QString::number(Number).toUtf8());

    socket->write(Buffer);
}


void MyClient::SendImage(QByteArray imgMsg)
{
    if (imgMsg.size() == 0)
    {
        qDebug() << "image frame to send is 0 bytes";
        return;
    }
    if (socketOpen_ == false)
    {
        qDebug() << "tried to write to closed socket";
        return;
    }
qDebug() << "wrote:" << imgMsg.size();
    socket->write(imgMsg);
}
