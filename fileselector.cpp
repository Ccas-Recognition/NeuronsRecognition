#include "fileselector.h"
#include "mainwindow.h"
#include "ui_fileselector.h"
#include <QFileDialog>

FileSelector::FileSelector(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileSelector)
{
    ui->setupUi(this);
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

    if (mainWindow) mainWindow->process_file( filepath );
}
