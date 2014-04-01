#include "ControlWidget.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "GLWidget3D.h"
#include <common/global.h>
#include <road/GraphUtil.h>
//#include <road/feature/GridFeature.h>
//#include <road/feature/RadialFeature.h>
//#include <road/feature/GenericFeature.h>
#include <road/generator/RoadGenerator.h>
#include <road/generator/KDERoadGenerator.h>


ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.lineEditNumIterations->setText("5000");
	ui.lineEditOrganicFactor->setText("0.1");
	ui.horizontalSliderExactSimilarityFactor->setMinimum(0);
	ui.horizontalSliderExactSimilarityFactor->setMaximum(100);
	ui.checkBoxLocalStreets->setChecked(false);
	ui.radioButtonMultiSeeds->setChecked(true);
	ui.checkBoxCropping->setChecked(false);
	ui.radioButtonCartesianCoordinate->setChecked(true);
	ui.lineEditPerturbationFactor->setText("0.1");

	// register the event handlers
	connect(ui.pushButtonGenerateKDE, SIGNAL(clicked()), this, SLOT(generateKDE()));
	connect(ui.pushButtonPerturb, SIGNAL(clicked()), this, SLOT(perturb()));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectRoads()));
	connect(ui.pushButtonMerge, SIGNAL(clicked()), this, SLOT(mergeRoads()));

	connect(ui.pushButtonGenerateUShape, SIGNAL(clicked()), this, SLOT(generateUShape()));

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

/**
 * Event handler for button [Generate KDE-base]
 */
void ControlWidget::generateKDE() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));

	if (filename.isEmpty()) return;

	G::global()["numIterations"] = ui.lineEditNumIterations->text().toInt();
	G::global()["roadOrganicFactor"] = ui.lineEditOrganicFactor->text().toFloat();
	G::global()["roadExactSimilarityFactor"] = ui.horizontalSliderExactSimilarityFactor->value() * 0.01f;
	G::global()["addAvenuesOnBoundary"] = ui.checkBoxAddAvenuesOnBoundary->isChecked();
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();

	G::global()["multiSeeds"] = ui.radioButtonMultiSeeds->isChecked();
	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["areaScaling"] = ui.checkBoxAreaScaling->isChecked();

	G::global()["coordiniates"] = ui.radioButtonCartesianCoordinate->isChecked() ? "cartesian" : "polar";

	int orientation = ui.dialOrientation->value() - 180;
	bool areaScaling = ui.checkBoxAreaScaling->isChecked();

	RoadFeature rf;
	rf.load(filename);

	if (orientation != 0) {
		rf.rotate(orientation);
	}

	if (areaScaling) {
		rf.scale(mainWin->urbanGeometry->areas.selectedArea()->area);
	}

	mainWin->urbanGeometry->generateRoads(rf);

	mainWin->glWidget->updateGL();
}

void ControlWidget::generateUShape() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));

	if (filename.isEmpty()) return;

	G::global()["numIterations"] = ui.lineEditNumIterations->text().toInt();
	G::global()["roadOrganicFactor"] = ui.lineEditOrganicFactor->text().toFloat();
	G::global()["roadExactSimilarityFactor"] = ui.horizontalSliderExactSimilarityFactor->value() * 0.01f;
	G::global()["addAvenuesOnBoundary"] = ui.checkBoxAddAvenuesOnBoundary->isChecked();
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();

	G::global()["multiSeeds"] = ui.radioButtonMultiSeeds->isChecked();
	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["areaScaling"] = ui.checkBoxAreaScaling->isChecked();

	G::global()["coordiniates"] = ui.radioButtonCartesianCoordinate->isChecked() ? "cartesian" : "polar";

	int orientation = ui.dialOrientation->value() - 180;
	bool areaScaling = ui.checkBoxAreaScaling->isChecked();

	RoadFeature rf;
	rf.load(filename);

	if (orientation != 0) {
		rf.rotate(orientation);
	}

	if (areaScaling) {
		rf.scale(mainWin->urbanGeometry->areas.selectedArea()->area);
	}

	mainWin->urbanGeometry->generateRoads(rf);

	mainWin->glWidget->updateGL();
}

void ControlWidget::perturb() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	mainWin->urbanGeometry->perturbRoads(ui.lineEditPerturbationFactor->text().toFloat());

	mainWin->glWidget->updateGL();
}

void ControlWidget::clear() {
	if (mainWin->urbanGeometry->areas.selectedIndex >= 0) {
		mainWin->urbanGeometry->areas.selectedArea()->clear();
	}

	mainWin->glWidget->updateGL();
}

void ControlWidget::mergeRoads() {
	mainWin->urbanGeometry->mergeRoads();

	mainWin->glWidget->updateGL();
}

/**
 * エリア間の境界上で、エッジができる限りつながるように、微調整する。
 */
void ControlWidget::connectRoads() {
	mainWin->urbanGeometry->connectRoads();

	mainWin->glWidget->updateGL();
}


