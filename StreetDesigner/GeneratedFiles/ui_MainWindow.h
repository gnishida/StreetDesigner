/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Mon Mar 24 09:40:57 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewTerrain;
    QAction *actionOpenTerrain;
    QAction *actionExit;
    QAction *actionControlWidget;
    QAction *actionDeleteEdge;
    QAction *actionSaveTerrain;
    QAction *actionUndo;
    QAction *actionSelectAll;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionOpenToAdd;
    QAction *actionVoronoi;
    QAction *actionVoronoiCut;
    QAction *actionModeBasic;
    QAction *actionModeLayer;
    QAction *actionModeSketch;
    QAction *actionShowArea;
    QAction *actionAreaSelect;
    QAction *actionAreaCreate;
    QAction *actionHighwaySketch;
    QAction *actionSave_2;
    QAction *actionSaveRoads;
    QAction *actionLoad_Highways;
    QAction *actionSave_Highways;
    QAction *actionBoulevardSketch;
    QAction *actionPropertyWidget;
    QAction *actionDebug;
    QAction *action3DView;
    QAction *actionTerrain;
    QAction *actionLoadRoads;
    QAction *actionLoadAreas;
    QAction *actionSaveAreas;
    QAction *actionGenerateBlocks;
    QAction *actionGenerateParcels;
    QAction *actionGenerateBuildings;
    QAction *actionAreaDelete;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuWindow;
    QMenu *menuArea;
    QMenu *menuPM;
    QToolBar *fileToolBar;
    QToolBar *areaToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1006, 800);
        MainWindow->setMinimumSize(QSize(1000, 800));
        actionNewTerrain = new QAction(MainWindow);
        actionNewTerrain->setObjectName(QString::fromUtf8("actionNewTerrain"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MainWindow/Resources/New_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewTerrain->setIcon(icon);
        actionOpenTerrain = new QAction(MainWindow);
        actionOpenTerrain->setObjectName(QString::fromUtf8("actionOpenTerrain"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/MainWindow/Resources/Open_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenTerrain->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionControlWidget = new QAction(MainWindow);
        actionControlWidget->setObjectName(QString::fromUtf8("actionControlWidget"));
        actionDeleteEdge = new QAction(MainWindow);
        actionDeleteEdge->setObjectName(QString::fromUtf8("actionDeleteEdge"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/MainWindow/Resources/Delete_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDeleteEdge->setIcon(icon2);
        actionSaveTerrain = new QAction(MainWindow);
        actionSaveTerrain->setObjectName(QString::fromUtf8("actionSaveTerrain"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/MainWindow/Resources/Save_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveTerrain->setIcon(icon3);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/MainWindow/Resources/Undo_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon4);
        actionSelectAll = new QAction(MainWindow);
        actionSelectAll->setObjectName(QString::fromUtf8("actionSelectAll"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/MainWindow/Resources/Cut_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon5);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/MainWindow/Resources/Copy_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon6);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/MainWindow/Resources/Paste_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon7);
        actionOpenToAdd = new QAction(MainWindow);
        actionOpenToAdd->setObjectName(QString::fromUtf8("actionOpenToAdd"));
        actionVoronoi = new QAction(MainWindow);
        actionVoronoi->setObjectName(QString::fromUtf8("actionVoronoi"));
        actionVoronoiCut = new QAction(MainWindow);
        actionVoronoiCut->setObjectName(QString::fromUtf8("actionVoronoiCut"));
        actionModeBasic = new QAction(MainWindow);
        actionModeBasic->setObjectName(QString::fromUtf8("actionModeBasic"));
        actionModeBasic->setCheckable(true);
        actionModeLayer = new QAction(MainWindow);
        actionModeLayer->setObjectName(QString::fromUtf8("actionModeLayer"));
        actionModeLayer->setCheckable(true);
        actionModeSketch = new QAction(MainWindow);
        actionModeSketch->setObjectName(QString::fromUtf8("actionModeSketch"));
        actionModeSketch->setCheckable(true);
        actionShowArea = new QAction(MainWindow);
        actionShowArea->setObjectName(QString::fromUtf8("actionShowArea"));
        actionAreaSelect = new QAction(MainWindow);
        actionAreaSelect->setObjectName(QString::fromUtf8("actionAreaSelect"));
        actionAreaSelect->setCheckable(true);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/MainWindow/Resources/Select_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAreaSelect->setIcon(icon8);
        actionAreaCreate = new QAction(MainWindow);
        actionAreaCreate->setObjectName(QString::fromUtf8("actionAreaCreate"));
        actionAreaCreate->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/MainWindow/Resources/Create_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAreaCreate->setIcon(icon9);
        actionHighwaySketch = new QAction(MainWindow);
        actionHighwaySketch->setObjectName(QString::fromUtf8("actionHighwaySketch"));
        actionHighwaySketch->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8("Resources/SketchH_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHighwaySketch->setIcon(icon10);
        actionSave_2 = new QAction(MainWindow);
        actionSave_2->setObjectName(QString::fromUtf8("actionSave_2"));
        actionSaveRoads = new QAction(MainWindow);
        actionSaveRoads->setObjectName(QString::fromUtf8("actionSaveRoads"));
        actionLoad_Highways = new QAction(MainWindow);
        actionLoad_Highways->setObjectName(QString::fromUtf8("actionLoad_Highways"));
        actionSave_Highways = new QAction(MainWindow);
        actionSave_Highways->setObjectName(QString::fromUtf8("actionSave_Highways"));
        actionBoulevardSketch = new QAction(MainWindow);
        actionBoulevardSketch->setObjectName(QString::fromUtf8("actionBoulevardSketch"));
        actionBoulevardSketch->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/MainWindow/Resources/SketchB_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBoulevardSketch->setIcon(icon11);
        actionPropertyWidget = new QAction(MainWindow);
        actionPropertyWidget->setObjectName(QString::fromUtf8("actionPropertyWidget"));
        actionDebug = new QAction(MainWindow);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        actionDebug->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/MainWindow/Resources/Debug_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDebug->setIcon(icon12);
        action3DView = new QAction(MainWindow);
        action3DView->setObjectName(QString::fromUtf8("action3DView"));
        action3DView->setCheckable(true);
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/MainWindow/Resources/3DView_24x24.png"), QSize(), QIcon::Normal, QIcon::Off);
        action3DView->setIcon(icon13);
        actionTerrain = new QAction(MainWindow);
        actionTerrain->setObjectName(QString::fromUtf8("actionTerrain"));
        actionTerrain->setCheckable(true);
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/MainWindow/Resources/Terrain_32x32.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTerrain->setIcon(icon14);
        actionLoadRoads = new QAction(MainWindow);
        actionLoadRoads->setObjectName(QString::fromUtf8("actionLoadRoads"));
        actionLoadAreas = new QAction(MainWindow);
        actionLoadAreas->setObjectName(QString::fromUtf8("actionLoadAreas"));
        actionSaveAreas = new QAction(MainWindow);
        actionSaveAreas->setObjectName(QString::fromUtf8("actionSaveAreas"));
        actionGenerateBlocks = new QAction(MainWindow);
        actionGenerateBlocks->setObjectName(QString::fromUtf8("actionGenerateBlocks"));
        actionGenerateParcels = new QAction(MainWindow);
        actionGenerateParcels->setObjectName(QString::fromUtf8("actionGenerateParcels"));
        actionGenerateBuildings = new QAction(MainWindow);
        actionGenerateBuildings->setObjectName(QString::fromUtf8("actionGenerateBuildings"));
        actionAreaDelete = new QAction(MainWindow);
        actionAreaDelete->setObjectName(QString::fromUtf8("actionAreaDelete"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1006, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuWindow = new QMenu(menuBar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuArea = new QMenu(menuBar);
        menuArea->setObjectName(QString::fromUtf8("menuArea"));
        menuPM = new QMenu(menuBar);
        menuPM->setObjectName(QString::fromUtf8("menuPM"));
        MainWindow->setMenuBar(menuBar);
        fileToolBar = new QToolBar(MainWindow);
        fileToolBar->setObjectName(QString::fromUtf8("fileToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, fileToolBar);
        areaToolBar = new QToolBar(MainWindow);
        areaToolBar->setObjectName(QString::fromUtf8("areaToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, areaToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuArea->menuAction());
        menuBar->addAction(menuPM->menuAction());
        menuBar->addAction(menuWindow->menuAction());
        menuFile->addAction(actionNewTerrain);
        menuFile->addAction(actionOpenTerrain);
        menuFile->addAction(actionSaveTerrain);
        menuFile->addSeparator();
        menuFile->addAction(actionLoadRoads);
        menuFile->addAction(actionSaveRoads);
        menuFile->addSeparator();
        menuFile->addAction(actionLoadAreas);
        menuFile->addAction(actionSaveAreas);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuWindow->addAction(actionControlWidget);
        menuWindow->addAction(actionPropertyWidget);
        menuArea->addAction(actionAreaSelect);
        menuArea->addAction(actionAreaCreate);
        menuArea->addAction(actionAreaDelete);
        menuArea->addSeparator();
        menuArea->addAction(actionHighwaySketch);
        menuArea->addAction(actionBoulevardSketch);
        menuArea->addSeparator();
        menuArea->addAction(actionDebug);
        menuPM->addAction(actionGenerateBlocks);
        menuPM->addAction(actionGenerateParcels);
        menuPM->addAction(actionGenerateBuildings);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Road Designer", 0, QApplication::UnicodeUTF8));
        actionNewTerrain->setText(QApplication::translate("MainWindow", "New Terrain", 0, QApplication::UnicodeUTF8));
        actionNewTerrain->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionOpenTerrain->setText(QApplication::translate("MainWindow", "Open Terrain", 0, QApplication::UnicodeUTF8));
        actionOpenTerrain->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionControlWidget->setText(QApplication::translate("MainWindow", "Control Widget", 0, QApplication::UnicodeUTF8));
        actionDeleteEdge->setText(QApplication::translate("MainWindow", "Delete Edge", 0, QApplication::UnicodeUTF8));
        actionDeleteEdge->setShortcut(QApplication::translate("MainWindow", "Del", 0, QApplication::UnicodeUTF8));
        actionSaveTerrain->setText(QApplication::translate("MainWindow", "Save Terrain", 0, QApplication::UnicodeUTF8));
        actionSaveTerrain->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        actionUndo->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionSelectAll->setText(QApplication::translate("MainWindow", "Select All", 0, QApplication::UnicodeUTF8));
        actionSelectAll->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionOpenToAdd->setText(QApplication::translate("MainWindow", "Open To Add", 0, QApplication::UnicodeUTF8));
        actionVoronoi->setText(QApplication::translate("MainWindow", "Voronoi", 0, QApplication::UnicodeUTF8));
        actionVoronoiCut->setText(QApplication::translate("MainWindow", "Voronoi Cut", 0, QApplication::UnicodeUTF8));
        actionModeBasic->setText(QApplication::translate("MainWindow", "Basic", 0, QApplication::UnicodeUTF8));
        actionModeLayer->setText(QApplication::translate("MainWindow", "Graph", 0, QApplication::UnicodeUTF8));
        actionModeSketch->setText(QApplication::translate("MainWindow", "Sketch", 0, QApplication::UnicodeUTF8));
        actionShowArea->setText(QApplication::translate("MainWindow", "Show Area", 0, QApplication::UnicodeUTF8));
        actionAreaSelect->setText(QApplication::translate("MainWindow", "Select", 0, QApplication::UnicodeUTF8));
        actionAreaCreate->setText(QApplication::translate("MainWindow", "Create", 0, QApplication::UnicodeUTF8));
        actionHighwaySketch->setText(QApplication::translate("MainWindow", "Highway Sketch", 0, QApplication::UnicodeUTF8));
        actionSave_2->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
        actionSaveRoads->setText(QApplication::translate("MainWindow", "Save Roads", 0, QApplication::UnicodeUTF8));
        actionLoad_Highways->setText(QApplication::translate("MainWindow", "Load Highways", 0, QApplication::UnicodeUTF8));
        actionSave_Highways->setText(QApplication::translate("MainWindow", "Save Highways", 0, QApplication::UnicodeUTF8));
        actionBoulevardSketch->setText(QApplication::translate("MainWindow", "Boulevard Sketch", 0, QApplication::UnicodeUTF8));
        actionPropertyWidget->setText(QApplication::translate("MainWindow", "Property Widget", 0, QApplication::UnicodeUTF8));
        actionDebug->setText(QApplication::translate("MainWindow", "Debug", 0, QApplication::UnicodeUTF8));
        action3DView->setText(QApplication::translate("MainWindow", "3D", 0, QApplication::UnicodeUTF8));
        actionTerrain->setText(QApplication::translate("MainWindow", "Terrain", 0, QApplication::UnicodeUTF8));
        actionLoadRoads->setText(QApplication::translate("MainWindow", "Load Roads", 0, QApplication::UnicodeUTF8));
        actionLoadAreas->setText(QApplication::translate("MainWindow", "Load Areas", 0, QApplication::UnicodeUTF8));
        actionSaveAreas->setText(QApplication::translate("MainWindow", "Save Areas", 0, QApplication::UnicodeUTF8));
        actionGenerateBlocks->setText(QApplication::translate("MainWindow", "Generate Blocks", 0, QApplication::UnicodeUTF8));
        actionGenerateParcels->setText(QApplication::translate("MainWindow", "Generate Parcels", 0, QApplication::UnicodeUTF8));
        actionGenerateBuildings->setText(QApplication::translate("MainWindow", "Generate Buildings", 0, QApplication::UnicodeUTF8));
        actionAreaDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuWindow->setTitle(QApplication::translate("MainWindow", "Window", 0, QApplication::UnicodeUTF8));
        menuArea->setTitle(QApplication::translate("MainWindow", "Area", 0, QApplication::UnicodeUTF8));
        menuPM->setTitle(QApplication::translate("MainWindow", "PM", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
