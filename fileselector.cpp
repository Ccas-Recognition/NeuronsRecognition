#include "fileselector.h"
#include "mainwindow.h"
#include "ui_fileselector.h"
#include <QFileDialog>
#include <QListWidgetItem>

FileSelector::FileSelector(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileSelector)
{
    ui->setupUi(this);

    QString q = "ImageExamples/main_image.bmp";

    addFile(q);
    processFile(q);
}

FileSelector::~FileSelector()
{
    delete ui;
}

void FileSelector::setMainWindow(MainWindow * w) {
    mainWindow = w;
}

void FileSelector::on_actionOpen_Folder_triggered()
{
    QString dirname = QFileDialog::getExistingDirectory(
            this,
            tr("Select a Directory"),
            QDir::currentPath() );

    if( !dirname.isNull() )
    {
      qDebug( dirname.toAscii() );
    }
}

void FileSelector::on_actionOpen_Files_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
           this,
           tr("Open Images"),
           QDir::currentPath(),
           tr("Image Files (*.png *.bmp *.jpg)") );

   if( !filenames.isEmpty() )
   {
     qDebug( filenames.join(",").toAscii() );
     for (int i=0; i< filenames.length(); i+=1) {
        addFile(filenames[i]);
     }
   }
}

void FileSelector::on_actionChoose_file_triggered()
{
    QFileDialog* lFileDlg  = new QFileDialog (NULL);
    lFileDlg->setOptions(0);
    lFileDlg->setWindowTitle(tr("Open image"));
    lFileDlg->setFilter(tr("Image Files (*.png *.bmp *.jpg)"));
    if (!lFileDlg->exec() || !lFileDlg->selectedFiles().size())
    {
        delete lFileDlg;
        return;
    }
    QString filepath = lFileDlg->selectedFiles().at(0);
    delete lFileDlg;

    addFile(filepath);
}

void FileSelector::on_listWidget_itemActivated(QListWidgetItem *item)
{
    // TODO add all images in folder
    // TODO add file for each image
    qDebug("item");
}

void FileSelector::processFile(QString path) {
    if (mainWindow) {
        mainWindow->setDetectionData(& (detectionMap[path.toStdString()]));
        mainWindow->process_file(path);
    }
}

void FileSelector::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    processFile(current->text());
}

void FileSelector::addFile(QString path)
{
    ui->listWidget->addItem(path);
    detectionMap[path.toStdString()] = DetectionData();
}
