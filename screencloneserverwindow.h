#ifndef SCREENCLONESERVERWINDOW_H
#define SCREENCLONESERVERWINDOW_H

#include <QMainWindow>
#include "capturemodule.h"

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
};
#endif // SCREENCLONESERVERWINDOW_H
