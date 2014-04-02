#pragma once

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"
#include "ControlWidget.h"
#include "GLWidget3D.h"
#include "UrbanGeometry.h"
#include <QToolBar>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	static enum { MODE_DEFAULT = 0, MODE_AREA_CREATE };

public:
	Ui::MainWindow ui;
	ControlWidget* controlWidget;
	GLWidget3D* glWidget;
	UrbanGeometry* urbanGeometry;

	int mode;

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();

protected:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

public slots:
	void onNew();
	void onLoad();
	void onSave();
	void onShowControlWidget();
};

