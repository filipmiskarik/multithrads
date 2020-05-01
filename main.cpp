#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    //w.operate(47000);
    //w.operateEratos(100);
    return a.exec();
}
