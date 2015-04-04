/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_File;
    QAction *actionSave_File;
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionRun;
    QAction *actionPaint_Objects;
    QAction *actionRubber;
    QAction *actionRun_thres;
    QAction *actionCuculate_thres;
    QAction *actionCulc_Stat;
    QAction *actionSdasad;
    QAction *actionChange_view;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QWidget *imageWidget;
    QListWidget *listWidget;
    QMenuBar *menuBar;
    QMenu *menuOpen_File;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(822, 658);
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/Icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_File->setIcon(icon);
        actionSave_File = new QAction(MainWindow);
        actionSave_File->setObjectName(QString::fromUtf8("actionSave_File"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/Icons/filesave-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_File->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/Icons/Gnome-Application-Exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/Icons/Help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon3);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/Icons/run-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon4);
        actionPaint_Objects = new QAction(MainWindow);
        actionPaint_Objects->setObjectName(QString::fromUtf8("actionPaint_Objects"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/Icons/Ballon-white.ico"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaint_Objects->setIcon(icon5);
        actionRubber = new QAction(MainWindow);
        actionRubber->setObjectName(QString::fromUtf8("actionRubber"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/Icons/Eraser-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRubber->setIcon(icon6);
        actionRun_thres = new QAction(MainWindow);
        actionRun_thres->setObjectName(QString::fromUtf8("actionRun_thres"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix1/Icons/blue-rectangle-icone-8818-128.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun_thres->setIcon(icon7);
        actionCuculate_thres = new QAction(MainWindow);
        actionCuculate_thres->setObjectName(QString::fromUtf8("actionCuculate_thres"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/prefix1/Icons/128x128-18-2120.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCuculate_thres->setIcon(icon8);
        actionCulc_Stat = new QAction(MainWindow);
        actionCulc_Stat->setObjectName(QString::fromUtf8("actionCulc_Stat"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/prefix1/Icons/Box habanos open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCulc_Stat->setIcon(icon9);
        actionSdasad = new QAction(MainWindow);
        actionSdasad->setObjectName(QString::fromUtf8("actionSdasad"));
        actionChange_view = new QAction(MainWindow);
        actionChange_view->setObjectName(QString::fromUtf8("actionChange_view"));
        actionChange_view->setIcon(icon7);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setEnabled(true);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setOrientation(Qt::Horizontal);
        imageWidget = new QWidget(splitter);
        imageWidget->setObjectName(QString::fromUtf8("imageWidget"));
        sizePolicy.setHeightForWidth(imageWidget->sizePolicy().hasHeightForWidth());
        imageWidget->setSizePolicy(sizePolicy);
        splitter->addWidget(imageWidget);
        listWidget = new QListWidget(splitter);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        splitter->addWidget(listWidget);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 822, 22));
        menuOpen_File = new QMenu(menuBar);
        menuOpen_File->setObjectName(QString::fromUtf8("menuOpen_File"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuOpen_File->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuOpen_File->addAction(actionOpen_File);
        menuOpen_File->addAction(actionSave_File);
        menuOpen_File->addSeparator();
        menuOpen_File->addAction(actionExit);
        menuEdit->addAction(actionRun);
        menuEdit->addAction(actionCulc_Stat);
        menuEdit->addAction(actionChange_view);
        mainToolBar->addAction(actionOpen_File);
        mainToolBar->addAction(actionSave_File);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRun);
        mainToolBar->addAction(actionCulc_Stat);
        mainToolBar->addAction(actionChange_view);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionHelp);
        mainToolBar->addAction(actionExit);

        retranslateUi(MainWindow);
        QObject::connect(actionExit, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Create Samples", 0, QApplication::UnicodeUTF8));
        actionOpen_File->setText(QApplication::translate("MainWindow", "Open File", 0, QApplication::UnicodeUTF8));
        actionSave_File->setText(QApplication::translate("MainWindow", "Save File", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionHelp->setToolTip(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRun->setText(QApplication::translate("MainWindow", "Detection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRun->setToolTip(QApplication::translate("MainWindow", "Run Classifier", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPaint_Objects->setText(QApplication::translate("MainWindow", "Paint Objects", 0, QApplication::UnicodeUTF8));
        actionRubber->setText(QApplication::translate("MainWindow", "Rubber", 0, QApplication::UnicodeUTF8));
        actionRun_thres->setText(QApplication::translate("MainWindow", "Run Threshold Classification", 0, QApplication::UnicodeUTF8));
        actionCuculate_thres->setText(QApplication::translate("MainWindow", "Cuculate Threshold", 0, QApplication::UnicodeUTF8));
        actionCulc_Stat->setText(QApplication::translate("MainWindow", "Select Area", 0, QApplication::UnicodeUTF8));
        actionSdasad->setText(QApplication::translate("MainWindow", "sdasad", 0, QApplication::UnicodeUTF8));
        actionChange_view->setText(QApplication::translate("MainWindow", "Change_view", 0, QApplication::UnicodeUTF8));
        menuOpen_File->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
