#include "MainWindow.h"
#include <road/GraphUtil.h>

MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	statusLebelL = new QLabel();
	statusLabelR = new QLabel();

	// setup UI
	ui.statusBar->addWidget(statusLebelL, 1);
	ui.statusBar->addWidget(statusLabelR, 0);

	// setup the docking widgets
	controlWidget = new ControlWidget(this);

	// register the menu's "AboutToShow" handlers
	connect(ui.menuEdit, SIGNAL(aboutToShow()), this, SLOT(onMenuEdit()));

	// register the menu's action handlers
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(onNew()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionSelectAll, SIGNAL(triggered()), this, SLOT(onSelectAll()));
	connect(ui.actionUndo, SIGNAL(triggered()), this, SLOT(onUndo()));
	connect(ui.actionCut, SIGNAL(triggered()), this, SLOT(onCut()));
	connect(ui.actionDeleteEdge, SIGNAL(triggered()), this, SLOT(onDeleteEdge()));
	connect(ui.actionControlWidget, SIGNAL(triggered()), this, SLOT(onShowControlWidget()));

	// setup the GL widget
	glWidget = new GLWidget(this);
	setCentralWidget(glWidget);

	// setup the event filter
	glWidget->installEventFilter(this);
	controlWidget->installEventFilter(this);
}

MainWindow::~MainWindow() {
}

void MainWindow::setStatusMessage(QString message1, QString message2) {
	statusLebelL->setText(message1);
	statusLabelR->setText(message2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void MainWindow::keyPressEvent(QKeyEvent* e) {
	glWidget->keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent* e) {
	glWidget->keyReleaseEvent(e);
}

void MainWindow::onMenuEdit() {
	ui.actionUndo->setDisabled(glWidget->editor->history.empty());
	ui.actionCut->setEnabled(glWidget->editor->mode == RoadGraphEditor::MODE_AREA_SELECTED);
	ui.actionCopy->setEnabled(glWidget->editor->mode == RoadGraphEditor::MODE_AREA_SELECTED);
	ui.actionPaste->setDisabled(glWidget->editor->clipBoard.empty());
	ui.actionDeleteEdge->setDisabled(glWidget->editor->selectedEdge == NULL);
}

void MainWindow::onNew() {
	glWidget->editor->clear();
	glWidget->updateGL();
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open StreetMap file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->editor->openRoad(filename);
	glWidget->updateGL();
	QApplication::restoreOverrideCursor();
}

void MainWindow::onSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save StreetMap file..."), "", tr("StreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->editor->saveRoad(filename);
	QApplication::restoreOverrideCursor();
}

void MainWindow::onSelectAll() {
	glWidget->editor->selectAll();
	glWidget->updateGL();
}

void MainWindow::onUndo() {
	glWidget->editor->undo();
	glWidget->updateGL();
}

void MainWindow::onCut() {
	glWidget->editor->cut();
	glWidget->updateGL();
}

void MainWindow::onDeleteEdge() {
	qDebug() << "onDeleteEdge";
	glWidget->editor->deleteEdge();
	glWidget->updateGL();
}

void MainWindow::onShowControlWidget() {
	controlWidget->show();
	addDockWidget(Qt::LeftDockWidgetArea, controlWidget);
}
