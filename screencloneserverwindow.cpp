#include "screencloneserverwindow.h"
#include "./ui_screencloneserverwindow.h"
#include<QPixmap>
#include<QImage>
#include<QScreen>
#include <QTimer>
#include <QDateTime>

ScreenCloneServerWindow::ScreenCloneServerWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneServerWindow),
    netModPtr_(),
    sendTimer_(),
    maintinanceTimer_(),
    Server_(),
    scene_(),
    capX_(10),
    capY_(10),
    capWidth_(1800),
    capHeight_(1024),
    lastTime_(0)
{
    ui->setupUi(this);

    getMaxScreenParams();

    //connect signals
    connect( ui->captureBtn_, &QPushButton::released, this, &ScreenCloneServerWindow::handleCaptureButton);

    connect( ui->streamCkBx_, &QCheckBox::released, this, &ScreenCloneServerWindow::handleStreamButton);

    sendTimer_.reset(new QTimer(this));
    connect(sendTimer_.get(), &QTimer::timeout, this, &ScreenCloneServerWindow::handleTimerEvent);

    maintinanceTimer_.reset(new QTimer(this));
    connect(maintinanceTimer_.get(), &QTimer::timeout, this, &ScreenCloneServerWindow::handleMaintTimerEvent);
    maintinanceTimer_->start(1000);


    Server_.reset(new ServerNetworkModule(this,1234));
    Server_->StartServer();

}

ScreenCloneServerWindow::~ScreenCloneServerWindow()
{
    delete ui;
}

/*
 *
 * Get a screen capture
 *
 */


//Function to do the work of sending an image
void ScreenCloneServerWindow::processImageEvent(bool preview)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap originalPixmap = screen->grabWindow(0,capX_,capY_,capWidth_,capHeight_);
    QImage image(originalPixmap.toImage());
    //test
    //QImage image(capWidth_,capWidth_,QImage::Format_RGB16);

    if (preview)
    {
         //Scale the window size and show the image
         int widgetWidth = image.width();
         if (widgetWidth < 640)
             widgetWidth = 640;
         int widgetHeight = image.height()+80;//need to add room for the text boxes
          //Resize the window
         this->resize( widgetWidth, widgetHeight );

         //know convert that image to a pixel map and set the image widget
         scene_.reset(new QGraphicsScene(this));
         scene_->addPixmap( QPixmap::fromImage( image ) );
         scene_->setSceneRect(image.rect());
         //Scal the size of the imageview widget
         QRect viewSize = ui->captureImgView_->geometry();
         viewSize.setWidth(image.width());
         viewSize.setHeight(image.height());
         ui->captureImgView_->setGeometry(viewSize);
         ui->captureImgView_->setScene(scene_.get());

    }
    image.convertTo(QImage::Format_RGB16);
    Server_->sendImage(image);

}


//Slots
void ScreenCloneServerWindow::handleCaptureButton()
{
        updateImageParams();
        processImageEvent(true);
}

//Slot to handle a user press of stream button
void ScreenCloneServerWindow::handleStreamButton()
{
    int rate = ui->fpsSpinBox_->value();
    double sleepTime = (1 / (double)rate) * 1000;

    qDebug() << "Time event butn:" << ui->streamCkBx_->checkState() << " sleep:" << round(sleepTime);
    if (ui->streamCkBx_->checkState() == Qt::Checked)
    {
        updateImageParams();
        sendTimer_->start( round(sleepTime) ); //ms
        ui->fpsSpinBox_->setEnabled(false);
        ui->xSpinBox_->setEnabled(false);
        ui->ySpinBox_->setEnabled(false);
        ui->widthSpinBox_->setEnabled(false);
        ui->heightSpinBox_->setEnabled(false);
        ui->captureBtn_->setEnabled(false);
    } else {
        sendTimer_->stop();
        ui->fpsSpinBox_->setEnabled(true);
        ui->xSpinBox_->setEnabled(true);
        ui->ySpinBox_->setEnabled(true);
        ui->widthSpinBox_->setEnabled(true);
        ui->heightSpinBox_->setEnabled(true);
        ui->captureBtn_->setEnabled(true);
    }
}


//Slot to handle a send message event
void ScreenCloneServerWindow::handleTimerEvent()
{
    qint64 nowTime = QDateTime::currentMSecsSinceEpoch();
    if (lastTime_>0)
    {
        qDebug() << "Sleep Time:" << nowTime - lastTime_;
    }

    lastTime_ = nowTime;

   // qDebug() << "Handle Timer event";
   processImageEvent(false);
}


//set the limites of the screen
void ScreenCloneServerWindow::getMaxScreenParams()
{
    QList <QScreen*> screens = QGuiApplication::screens();
    totalWidth_ = 0;
    totalHeight_ = 0;

    int screenNum = 0;
    QRect  screenGeometry;
    //QScreen *screen = screens.front();//QGuiApplication::primaryScreen();
    for(auto itr : screens)
    {
       screenGeometry = itr->geometry();
       //qDebug() << "index" << screenNum << "X" << screenGeometry.x() << " y " << screenGeometry.y() << " W " << screenGeometry.width() << " H " << screenGeometry.height();

       if (screenGeometry.x() + screenGeometry.width() > totalWidth_)
           totalWidth_ = screenGeometry.x() + screenGeometry.width();

       if (screenGeometry.y() + screenGeometry.height() > totalHeight_)
           totalHeight_ = screenGeometry.y() + screenGeometry.height();
       screenNum++;
    }
}

//update the params of the image
void ScreenCloneServerWindow::updateImageParams()
{

    capX_ = ui->xSpinBox_->value();
    capY_ = ui->ySpinBox_->value();
    capWidth_ = ui->widthSpinBox_->value();
    capHeight_ = ui->heightSpinBox_->value();
    if (capX_ > totalWidth_)
    {
        capX_ = totalWidth_ - 10;
        qWarning() << "X greater than total screen width";
    }

    if (capX_ + capWidth_ > totalWidth_)
    {
        capWidth_ = totalWidth_ - capX_;
        qWarning() << "Width greater than total screen width";
    }

    if (capY_ > totalHeight_)
    {
        capY_ = totalHeight_ - 10;
        qWarning() << "Y greater than total screen height";
    }

    if (capY_ + capHeight_ > totalHeight_)
    {
        capHeight_ = totalHeight_ - capY_;
        qWarning() << "Width greater than total screen width";
    }
}


//Timer event to do maintiance things
void ScreenCloneServerWindow::handleMaintTimerEvent()
{
    ui->connectedClients_->setText(QString::number( Server_->getClientCount() ) );
}
