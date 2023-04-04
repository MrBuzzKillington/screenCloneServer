#include "screencloneserverwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenCloneServerWindow w;
    w.show();

    return a.exec();
}
