#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <QMainWindow>
#include "mainwindow.h"
#include "DetectionData.h"
#include <QListWidgetItem>
#include <vector>

using std::map;

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

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    void addFile(QString path);

private:
    Ui::FileSelector *ui;
    MainWindow * mainWindow;
    map<string, DetectionData> detectionMap;

    void processFile(QString path);
};

#endif // FILESELECTOR_H
