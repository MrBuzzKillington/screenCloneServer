#ifndef SCREENCLONESERVERWINDOW_H
#define SCREENCLONESERVERWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "servernetworkmodule.h"


QT_BEGIN_NAMESPACE
namespace Ui { class ScreenCloneServerWindow; }
QT_END_NAMESPACE

class ScreenCloneServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ScreenCloneServerWindow(QWidget *parent = nullptr);
    ~ScreenCloneServerWindow();


private:
    Ui::ScreenCloneServerWindow *ui;
    std::unique_ptr<ServerNetworkModule> netModPtr_;
    std::unique_ptr<QTimer> sendTimer_;
    std::unique_ptr<ServerNetworkModule> Server_;
    std::unique_ptr<QGraphicsScene> scene_;
    int capX_;
    int capY_;
    int capWidth_;
    int capHeight_;
    int totalWidth_;
    int totalHeight_;

    QImage getImage(int &x, int &y, int &width, int &height);
    void processImageEvent(bool preview);
    void getMaxScreenParams();
    void updateImageParams();

private slots:
  void handleCaptureButton();
  void handleStreamButton();
  void handleTimerEvent();
};
#endif // SCREENCLONESERVERWINDOW_H
