#include "PMControlWidget.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "GLWidget3D.h"
#include <common/global.h>
#include <road/GraphUtil.h>
#include <road/generator/RoadGenerator.h>
#include <road/generator/KDERoadGenerator.h>


PMControlWidget::PMControlWidget(MainWindow* mainWin) : QDockWidget("PM Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.lineEditNumIterations->setText("1000");
	ui.lineEditOrganicFactor->setText("0.1");
	ui.lineEditSnapFactor->setText("0.7");
	ui.lineEditMinAngle->setText("1.2566");

	// register the event handlers
	connect(ui.pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generate()));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonMerge, SIGNAL(clicked()), this, SLOT(merge()));

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

/**
 * Event handler for button [Generate]
 */
void PMControlWidget::generate() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	G::global()["numIterations"] = ui.lineEditNumIterations->text().toInt();
	G::global()["organicFactor"] = ui.lineEditOrganicFactor->text().toFloat();
	G::global()["snapFactor"] = ui.lineEditSnapFactor->text().toFloat();
	G::global()["minAngle"] = ui.lineEditMinAngle->text().toFloat();

	mainWin->urbanGeometry->generatePMRoads();

	mainWin->glWidget->updateGL();
}


void PMControlWidget::clear() {
	if (mainWin->urbanGeometry->areas.selectedIndex >= 0) {
		mainWin->urbanGeometry->areas.selectedArea()->clear();
	}

	mainWin->glWidget->updateGL();
}

void PMControlWidget::merge() {
	mainWin->urbanGeometry->mergeRoads();

	mainWin->glWidget->updateGL();
}

