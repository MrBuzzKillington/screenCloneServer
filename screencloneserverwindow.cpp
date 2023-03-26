#include "screencloneserverwindow.h"
#include "./ui_screencloneserverwindow.h"

ScreenCloneServerWindow::ScreenCloneServerWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::ScreenCloneServerWindow),
    capMod_()
{
    ui->setupUi(this);

    HBITMAP capturedBitmap;
    capMod_.initCapture(2126, 112, 300, 400);

    capturedBitmap = capMod_.captureImage(); //Capture an image
}

ScreenCloneServerWindow::~ScreenCloneServerWindow()
{
    delete ui;
}

