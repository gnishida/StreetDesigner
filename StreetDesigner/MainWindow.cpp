#include <road/GraphUtil.h>
#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// setup the docking widgets
	controlWidget = new ControlWidget(this);
	propertyWidget = new PropertyWidget(this);

	// setup the toolbar
	ui.fileToolBar->addAction(ui.actionNewTerrain);
	ui.fileToolBar->addAction(ui.actionOpenTerrain);
	ui.areaToolBar->addAction(ui.actionAreaSelect);
	ui.areaToolBar->addAction(ui.actionAreaCreate);
	ui.areaToolBar->addAction(ui.actionHighwaySketch);
	ui.areaToolBar->addAction(ui.actionBoulevardSketch);
	ui.areaToolBar->addAction(ui.actionDebug);

	ui.actionAreaSelect->setChecked(true);

	// register the menu's action handlers
	connect(ui.actionNewTerrain, SIGNAL(triggered()), this, SLOT(onNewTerrain()));
	connect(ui.actionOpenTerrain, SIGNAL(triggered()), this, SLOT(onLoadTerrain()));
	connect(ui.actionSaveTerrain, SIGNAL(triggered()), this, SLOT(onSaveTerrain()));
	connect(ui.actionLoadRoads, SIGNAL(triggered()), this, SLOT(onLoadRoads()));
	connect(ui.actionSaveRoads, SIGNAL(triggered()), this, SLOT(onSaveRoads()));
	connect(ui.actionLoadAreas, SIGNAL(triggered()), this, SLOT(onLoadAreas()));
	connect(ui.actionSaveAreas, SIGNAL(triggered()), this, SLOT(onSaveAreas()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.menuArea, SIGNAL(aboutToShow()), this, SLOT(onAreaMenu()));
	connect(ui.actionAreaSelect, SIGNAL(triggered()), this, SLOT(onAreaSelect()));
	connect(ui.actionAreaCreate, SIGNAL(triggered()), this, SLOT(onAreaCreate()));
	connect(ui.actionHighwaySketch, SIGNAL(triggered()), this, SLOT(onHighwaySketch()));
	//connect(ui.actionBoulevardSketch, SIGNAL(triggered()), this, SLOT(onBoulevardSketch()));
	connect(ui.actionDebug, SIGNAL(triggered()), this, SLOT(onDebug()));
	connect(ui.actionGenerateBlocks, SIGNAL(triggered()), this, SLOT(onGenerateBlocks()));
	connect(ui.actionGenerateParcels, SIGNAL(triggered()), this, SLOT(onGenerateParcels()));
	connect(ui.actionControlWidget, SIGNAL(triggered()), this, SLOT(onShowControlWidget()));
	connect(ui.actionPropertyWidget, SIGNAL(triggered()), this, SLOT(onShowPropertyWidget()));

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);

	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);

	//propertyWidget->show();
	//addDockWidget(Qt::RightDockWidgetArea, propertyWidget);

	urbanGeometry = new MyUrbanGeometry(this, 3000, 3000);

	mode = MODE_AREA_SELECT;
}

MainWindow::~MainWindow() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void MainWindow::keyPressEvent(QKeyEvent* e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onNewTerrain() {
	urbanGeometry->newTerrain(6000, 6000, 100);

	glWidget->updateGL();
}

void MainWindow::onLoadTerrain() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Terrain file..."), "", tr("Terrain Files (*.trn)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->loadTerrain(filename);

	glWidget->updateGL();
}

void MainWindow::onSaveTerrain() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Terrain file..."), "", tr("Terrain Files (*.trn)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->saveTerrain(filename);
}

void MainWindow::onLoadRoads() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Street Map file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->loadRoads(filename);

	glWidget->updateGL();
}

void MainWindow::onSaveRoads() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Street Map file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);

	urbanGeometry->saveRoads(filename);

	QApplication::restoreOverrideCursor();
}

void MainWindow::onLoadAreas() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Area file..."), "", tr("Area Files (*.area)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->loadAreas(filename);

	glWidget->updateGL();
}

void MainWindow::onSaveAreas() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Area Map file..."), "", tr("Area Files (*.area)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);

	urbanGeometry->saveAreas(filename);

	QApplication::restoreOverrideCursor();
}

void MainWindow::onAreaMenu() {
	ui.actionAreaSelect->setChecked(mode == MODE_AREA_SELECT);
	ui.actionAreaCreate->setChecked(mode == MODE_AREA_CREATE);
	ui.actionHighwaySketch->setChecked(mode == MODE_HIGHWAY_SKETCH);
	ui.actionBoulevardSketch->setChecked(mode == MODE_BOULEVARD_SKETCH);
	ui.action3DView->setChecked(mode == MODE_3DVIEW);
	ui.actionTerrain->setChecked(mode == MODE_TERRAIN);
	ui.actionDebug->setChecked(mode == MODE_DEBUG);
}

void MainWindow::onAreaSelect() {
	mode = MODE_AREA_SELECT;
	ui.actionAreaCreate->setChecked(false);
	ui.actionHighwaySketch->setChecked(false);
	ui.actionBoulevardSketch->setChecked(false);
	ui.action3DView->setChecked(false);
	ui.actionTerrain->setChecked(false);
	ui.actionDebug->setChecked(false);
}

void MainWindow::onAreaCreate() {
	mode = MODE_AREA_CREATE;
	ui.actionAreaSelect->setChecked(false);
	ui.actionHighwaySketch->setChecked(false);
	ui.actionBoulevardSketch->setChecked(false);
	ui.action3DView->setChecked(false);
	ui.actionTerrain->setChecked(false);
	ui.actionDebug->setChecked(false);
}

void MainWindow::onHighwaySketch() {
	mode = MODE_HIGHWAY_SKETCH;
	ui.actionAreaCreate->setChecked(false);
	ui.actionAreaSelect->setChecked(false);
	ui.actionBoulevardSketch->setChecked(false);
	ui.action3DView->setChecked(false);
	ui.actionTerrain->setChecked(false);
	ui.actionDebug->setChecked(false);
}

/*
void MainWindow::onBoulevardSketch() {
	mode = MODE_BOULEVARD_SKETCH;
	ui.actionAreaCreate->setChecked(false);
	ui.actionAreaSelect->setChecked(false);
	ui.actionHighwaySketch->setChecked(false);
	ui.action3DView->setChecked(false);
	ui.actionTerrain->setChecked(false);
	ui.actionDebug->setChecked(false);
}
*/

void MainWindow::onDebug() {
	mode = MODE_DEBUG;
	ui.actionAreaSelect->setChecked(false);
	ui.actionAreaCreate->setChecked(false);
	ui.actionHighwaySketch->setChecked(false);
	ui.actionBoulevardSketch->setChecked(false);
	ui.action3DView->setChecked(false);
	ui.actionTerrain->setChecked(false);
}

void MainWindow::onGenerateBlocks() {
	urbanGeometry->generateBlocks();
}

void MainWindow::onGenerateParcels() {
}

void MainWindow::onShowControlWidget() {
	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);
}

void MainWindow::onShowPropertyWidget() {
	propertyWidget->show();
	addDockWidget(Qt::RightDockWidgetArea, propertyWidget);
}

