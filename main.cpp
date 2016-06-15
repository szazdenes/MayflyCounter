#include "mayflycounter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MayflyCounter w;
    w.showMaximized();

    return a.exec();
}
