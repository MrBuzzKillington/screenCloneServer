#ifndef CAPTUREMODULE_H
#define CAPTUREMODULE_H

#include <QMainWindow>
#include <QWidget>
#include <Windows.h>

class CaptureModule : public QWidget
{
    Q_OBJECT
public:
    explicit CaptureModule(QWidget *parent = nullptr);
    ~CaptureModule();

    void initCapture(int posX, int posY, int width, int height);
    HBITMAP captureImage();
    void setFullScreen();

private:
    static int currentId_;
    int id_;
    int posX_;
    int posY_;
    int width_;
    int height_;

    HBITMAP capturedBitmap_; // Image to send/recieve

signals:

};

#endif // CAPTUREMODULE_H
