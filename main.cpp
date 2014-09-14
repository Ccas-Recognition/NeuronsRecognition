#include <QtGui/QApplication>
#include "mainwindow.h"
/**
 * Входная точка в программу.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
