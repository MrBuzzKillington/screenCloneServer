#ifndef SCREENCLONESERVERWINDOW_H
#define SCREENCLONESERVERWINDOW_H

#include <QMainWindow>
#include "capturemodule.h"
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
    CaptureModule capMod_;
    std::unique_ptr<ServerNetworkModule> netModPtr_;
    std::unique_ptr<QTimer> sendTimer_;

    QImage getImage(int &x, int &y, int &width, int &height);
    void processImageEvent(bool preview);

private slots:
  void handleCaptureButton();
  void handleStreamButton();
  void handleTimerEvent();
};
#endif // SCREENCLONESERVERWINDOW_H
