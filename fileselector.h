#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class FileSelector;
}

class FileSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileSelector(QWidget *parent = 0);
    void setMainWindow(MainWindow * w);
    ~FileSelector();

private slots:
    void on_actionOpen_Folder_triggered();

    void on_actionOpen_Files_triggered();

    void on_actionChoose_file_triggered();

private:
    Ui::FileSelector *ui;
    MainWindow * mainWindow;
};

#endif // FILESELECTOR_H
