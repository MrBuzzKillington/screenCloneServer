#include "screencloneserverwindow.h"
#include "./ui_screencloneserverwindow.h"
#include<QPixmap>
#include<QImage>

ScreenCloneServerWindow::ScreenCloneServerWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneServerWindow),
    capMod_(),
    scene_()
{
    ui->setupUi(this);

    int capX = 2740;
    int capY = 305;
    int capWidth = 450;
    int capHeight = 100;


    capMod_.initCapture(capX, capY, capWidth, capHeight);

    connect( ui->captureBtn_, &QPushButton::released, this, &ScreenCloneServerWindow::handleCaptureButton);

}

ScreenCloneServerWindow::~ScreenCloneServerWindow()
{
    delete ui;
}

void ScreenCloneServerWindow::handleCaptureButton()
{

       qDebug() << "Capture Button pressed";
       HBITMAP capturedBitmap;
       capturedBitmap = capMod_.captureImage(); //Capture an image

       QImage image(QImage::fromHBITMAP(capturedBitmap));
       scene_ = new QGraphicsScene(this);
       scene_->addPixmap( QPixmap::fromImage( image ) );
       scene_->setSceneRect(image.rect());


       qDebug() << image.width() << ",  " << image.height() ;

       QRect viewSize = ui->captureImgView_->geometry();
       viewSize.setWidth(image.width());
       viewSize.setHeight(image.height());
       ui->captureImgView_->setGeometry(viewSize);
       ui->captureImgView_->setScene(scene_);

}
