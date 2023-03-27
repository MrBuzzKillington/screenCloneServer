#include "screencloneserverwindow.h"
#include "./ui_screencloneserverwindow.h"
#include<QPixmap>
#include<QImage>
#include<QScreen>
#include "servernetworkmodule.h"

ScreenCloneServerWindow::ScreenCloneServerWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneServerWindow),
    capMod_(),
    scene_(),
    netMod_()
{
    ui->setupUi(this);
    int capX = ui->xSpinBox_->value();
    int capY = ui->ySpinBox_->value();
    int capWidth = ui->widthSpinBox_->value();
    int capHeight = ui->heightSpinBox_->value();
    capMod_.initCapture(capX, capY, capWidth, capHeight);


    //        = new ServerNetworkModule(QHostAddress::LocalHost, 1234);

    //connect signals
    connect( ui->captureBtn_, &QPushButton::released, this, &ScreenCloneServerWindow::handleCaptureButton);
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
void ScreenCloneServerWindow::handleCaptureButton()
{

       //qDebug() << "Capture Button pressed";
        int capX = ui->xSpinBox_->value();
       int capY = ui->ySpinBox_->value();
       int capWidth = ui->widthSpinBox_->value();
       int capHeight = ui->heightSpinBox_->value();

       QImage image = getImage(capX, capY, capWidth, capHeight);

      //update the capture window
       //capMod_.initCapture(capX, capY, capWidth, capHeight);
       //capturedBitmap = capMod_.captureImage(); //Capture an image
       //QImage image(QImage::fromHBITMAP(capturedBitmap));

       //Scale the window size and show the image
       int widgetWidth = image.width();
       if (widgetWidth < 640)
           widgetWidth = 640;
       int widgetHeight = image.height()+30;//need to add room for the text boxes
        //Resize the window
       this->resize( widgetWidth, widgetHeight );

       //know convert that image to a pixel map and set the image widget
       scene_ = new QGraphicsScene(this);
       scene_->addPixmap( QPixmap::fromImage( image ) );
       scene_->setSceneRect(image.rect());

       //Scal the size of the imageview widget
       QRect viewSize = ui->captureImgView_->geometry();
       viewSize.setWidth(image.width());
       viewSize.setHeight(image.height());
       ui->captureImgView_->setGeometry(viewSize);
       ui->captureImgView_->setScene(scene_);

}

//
//  check that the size is ok fix if not
//
QImage ScreenCloneServerWindow::getImage(int &x, int &y, int &width, int &height)
{

    HBITMAP capturedBitmap;
    QList <QScreen*> screens = QGuiApplication::screens();
    int totalWidth = 0;
    int totalHeight = 0;

    int screenNum = 0;
    QRect  screenGeometry;
    //QScreen *screen = screens.front();//QGuiApplication::primaryScreen();
    for(auto itr : screens)
    {
       screenGeometry = itr->geometry();
       qDebug() << "index" << screenNum << "X" << screenGeometry.x() << " y " << screenGeometry.y() << " W " << screenGeometry.width() << " H " << screenGeometry.height();

       if (screenGeometry.x() + screenGeometry.width() > totalWidth)
           totalWidth = screenGeometry.x() + screenGeometry.width();

       if (screenGeometry.y() + screenGeometry.height() > totalHeight)
           totalHeight = screenGeometry.y() + screenGeometry.height();
       screenNum++;
    }

    qDebug() << "Total Res = " << totalWidth << " height " << totalHeight;

    if (x > totalWidth)
    {
        x = totalWidth - 10;
        qWarning() << "X greater than total screen width";
    }

    if (x + width > totalWidth)
    {
        width = totalWidth - x;
        qWarning() << "Width greater than total screen width";
    }

    if (y > totalHeight)
    {
        y = totalHeight - 10;
        qWarning() << "Y greater than total screen height";
    }

    if (y + height > totalHeight)
    {
        height = totalHeight - y;
        qWarning() << "Width greater than total screen width";
    }

    capMod_.initCapture(x, y, width, height);
    capturedBitmap = capMod_.captureImage(); //Capture an image
    QImage image(QImage::fromHBITMAP(capturedBitmap));

    return image;
}
