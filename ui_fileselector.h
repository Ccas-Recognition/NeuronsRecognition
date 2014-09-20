/********************************************************************************
** Form generated from reading UI file 'fileselector.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILESELECTOR_H
#define UI_FILESELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileSelector
{
public:
    QAction *actionOpen_Folder;
    QAction *actionOpen_Files;
    QAction *actionChoose_file;
    QWidget *centralwidget;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FileSelector)
    {
        if (FileSelector->objectName().isEmpty())
            FileSelector->setObjectName(QString::fromUtf8("FileSelector"));
        FileSelector->resize(800, 600);
        actionOpen_Folder = new QAction(FileSelector);
        actionOpen_Folder->setObjectName(QString::fromUtf8("actionOpen_Folder"));
        actionOpen_Files = new QAction(FileSelector);
        actionOpen_Files->setObjectName(QString::fromUtf8("actionOpen_Files"));
        actionChoose_file = new QAction(FileSelector);
        actionChoose_file->setObjectName(QString::fromUtf8("actionChoose_file"));
        centralwidget = new QWidget(FileSelector);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 10, 431, 461));
        FileSelector->setCentralWidget(centralwidget);
        menubar = new QMenuBar(FileSelector);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        FileSelector->setMenuBar(menubar);
        statusbar = new QStatusBar(FileSelector);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        FileSelector->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionChoose_file);
        menuFile->addAction(actionOpen_Files);
        menuFile->addAction(actionOpen_Folder);

        retranslateUi(FileSelector);

        QMetaObject::connectSlotsByName(FileSelector);
    } // setupUi

    void retranslateUi(QMainWindow *FileSelector)
    {
        FileSelector->setWindowTitle(QApplication::translate("FileSelector", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen_Folder->setText(QApplication::translate("FileSelector", "Open Folder", 0, QApplication::UnicodeUTF8));
        actionOpen_Files->setText(QApplication::translate("FileSelector", "Choose files", 0, QApplication::UnicodeUTF8));
        actionChoose_file->setText(QApplication::translate("FileSelector", "Choose file", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("FileSelector", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileSelector: public Ui_FileSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILESELECTOR_H
