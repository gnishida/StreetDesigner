#include "ControlWidget.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "GLWidget3D.h"
#include <common/global.h>
#include <road/GraphUtil.h>
#include <road/generator/ExRoadGenerator.h>
#include <road/feature/ExFeature.h>


ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.lineEditElevation->setText("0");

	// register the event handlers
	connect(ui.pushButtonApply, SIGNAL(clicked()), this, SLOT(apply()));
	/*
	connect(ui.pushButtonPerturb, SIGNAL(clicked()), this, SLOT(perturb()));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectRoads()));
	connect(ui.pushButtonMerge, SIGNAL(clicked()), this, SLOT(mergeRoads()));
	*/

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void ControlWidget::apply() {
	float z = ui.lineEditElevation->text().toFloat();

	mainWin->urbanGeometry->setElevation(z);
	mainWin->glWidget->updateGL();
}


