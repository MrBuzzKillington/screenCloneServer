#include "capturemodule.h"

#include<QScreen>

int CaptureModule::currentId_ = 0;

CaptureModule::CaptureModule(QWidget *parent)
    : QWidget{parent},
      id_(0),
      posX_(0),
      posY_(0),
      width_(0),
      height_(0),
      capturedBitmap_()
{


    id_ = (currentId_++); //increment the id for each time one is created

    if ( !SetProcessDPIAware() ) // This is required to get the full screen
    {
        //cout << "Failed to get DPI aware" << std::endl;
    }

    //cout << "Created Module" << id_ << std::endl;
}

//Destructor
CaptureModule::~CaptureModule()
{
    DeleteObject(capturedBitmap_); // delete the bitmap once it's no longer needed
    //cout << "destroyed moduled\n";

}

//Set the width and height to full screen
//Note this only takes the primary display
void CaptureModule::setFullScreen()
{
    //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsystemmetrics
    width_ = GetSystemMetrics(SM_CXSCREEN); // Screen width and height in pixels of the primary display
    height_ = GetSystemMetrics(SM_CYSCREEN);
    capturedBitmap_ = CreateBitmap(width_, height_, 1, 32, NULL); // Create an empty bitmap to copy image to

}

//init the capture;
void CaptureModule::initCapture(int posX, int posY, int width, int height)
{
    posX_ = posX;
    posY_ = posY;
    width_ = width;
    height_ = height;
    capturedBitmap_ = CreateBitmap(width_, height_, 1, 32, NULL); // Create an empty bitmap to copy image to

    //cout << "Capture Module initilized " << posX << " " << posY << " " << width << " " << height << std::endl;

}

HBITMAP CaptureModule::captureImage()
{
    //Maybe move these to init
    HDC Desktop = GetDC(HWND_DESKTOP);
    HDC BitmapDC = CreateCompatibleDC(Desktop);
    SelectObject(BitmapDC, capturedBitmap_);
    BitBlt(BitmapDC, 0, 0, width_, height_, Desktop, posX_, posY_, SRCCOPY); // Copy data from desktop to new bitmap
    DeleteDC(Desktop); // Free up memory
    DeleteDC(BitmapDC);
    return capturedBitmap_;
}



