﻿#include <QtGui/QApplication>
#include "mainwindow.h"
#include "fileselector.h"
/**
 * Входная точка в программу.
 */
int main(int argc, char *argv[])
{
    int start_x = 150;
    int start_y = 100;

    QApplication a(argc, argv);
    MainWindow w;
    w.move(QPoint(start_x, start_y));
    w.show();

    FileSelector f;
    f.setMainWindow(&w);
    f.move(QPoint(start_x + w.width() + 30, start_y));
    f.show();
    
    return a.exec();
}
