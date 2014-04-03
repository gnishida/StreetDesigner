#include "MainWindow.h"
#include "TerrainSizeInputDialog.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// setup the docking widgets
	controlWidget = new ControlWidget(this);

	// setup the toolbar
	ui.fileToolBar->addAction(ui.actionNew);
	ui.fileToolBar->addAction(ui.actionOpen);
	ui.areaToolBar->addAction(ui.actionAreaCreate);

	ui.actionAreaSelect->setChecked(true);

	// register the menu's action handlers
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onLoad()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionAreaCreate, SIGNAL(triggered()), this, SLOT(onAreaCreate()));
	connect(ui.actionControlWidget, SIGNAL(triggered()), this, SLOT(onShowControlWidget()));

	// setup the GL widget
	glWidget = new GLWidget3D(this);
	setCentralWidget(glWidget);

	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);

	urbanGeometry = new UrbanGeometry(this);

	mode = MODE_DEFAULT;
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

void MainWindow::onNew() {
	TerrainSizeInputDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		urbanGeometry->newTerrain(dlg.width, dlg.depth, dlg.cellLength);
		glWidget->updateGL();
	}
}

void MainWindow::onLoad() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Terrain file..."), "", tr("Terrain Files (*.trn)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->loadTerrain(filename);

	glWidget->updateGL();
}

void MainWindow::onSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Terrain file..."), "", tr("Terrain Files (*.trn)"));

	if (filename.isEmpty()) {
		return;
	}

	urbanGeometry->saveTerrain(filename);
}

void MainWindow::onAreaCreate() {
	mode = MODE_AREA_CREATE;
}

void MainWindow::onShowControlWidget() {
	//pmcontrolWidget->hide();
	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);
}

