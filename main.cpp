#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fileselector.h"
/**
 * Входная точка в программу.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    FileSelector f;
    f.setMainWindow(&w);
    f.show();
    
    return a.exec();
}
