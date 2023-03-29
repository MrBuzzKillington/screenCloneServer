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
    QGraphicsScene *scene_;
    std::unique_ptr<ServerNetworkModule> netModPtr_;

    QImage getImage(int &x, int &y, int &width, int &height);

private slots:
  void handleCaptureButton();
};
#endif // SCREENCLONESERVERWINDOW_H
