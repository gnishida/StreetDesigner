#pragma once

#include <QDockWidget>
#include "ui_PMControlWidget.h"

class MainWindow;

class PMControlWidget : public QDockWidget {
Q_OBJECT

private:
	Ui::PMControlWidget ui;
	MainWindow* mainWin;

public:
	PMControlWidget(MainWindow* mainWin);

public slots:
	void generate();
	void clear();
	void merge();
};

