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
	ui.lineEditNumAvenueIterations->setText("1000");
	ui.lineEditNumStreetIterations->setText("5000");
	ui.horizontalSliderExactSimilarityFactor->setMinimum(0);
	ui.horizontalSliderExactSimilarityFactor->setMaximum(100);
	ui.checkBoxCleanAvenues->setChecked(true);
	ui.checkBoxCleanStreets->setChecked(true);
	ui.checkBoxLocalStreets->setChecked(false);
	ui.checkBoxCropping->setChecked(true);
	ui.radioButtonCartesianCoordinate->setChecked(true);
	ui.lineEditPerturbationFactor->setText("0.1");

	// register the event handlers
	connect(ui.pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generateRoads()));
	connect(ui.pushButtonPerturb, SIGNAL(clicked()), this, SLOT(perturb()));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectRoads()));
	connect(ui.pushButtonMerge, SIGNAL(clicked()), this, SLOT(mergeRoads()));
	connect(ui.pushButtonGenerateMultiEx, SIGNAL(clicked()), this, SLOT(generateRoadsMultiEx()));

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

/**
 * Event handler for button [Generate Roads]
 */
void ControlWidget::generateRoads() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));

	if (filename.isEmpty()) return;

	G::global()["numAvenueIterations"] = ui.lineEditNumAvenueIterations->text().toInt();
	G::global()["numStreetIterations"] = ui.lineEditNumStreetIterations->text().toInt();
	G::global()["cleanAvenues"] = ui.checkBoxCleanAvenues->isChecked();
	G::global()["cleanStreets"] = ui.checkBoxCleanStreets->isChecked();
	G::global()["roadExactSimilarityFactor"] = ui.horizontalSliderExactSimilarityFactor->value() * 0.01f;
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();

	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["areaScaling"] = ui.checkBoxAreaScaling->isChecked();

	G::global()["coordiniates"] = ui.radioButtonCartesianCoordinate->isChecked() ? "cartesian" : "polar";

	int orientation = ui.dialOrientation->value() - 180;
	bool areaScaling = ui.checkBoxAreaScaling->isChecked();

	ExFeature feature;
	feature.load(filename);

	if (orientation != 0) {
		//feature.rotate(orientation);
	}

	if (areaScaling) {
		//feature.scale(mainWin->urbanGeometry->areas.selectedArea()->area);
	}

	mainWin->urbanGeometry->generateRoads(feature);

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

void ControlWidget::generateRoadsMultiEx() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	std::vector<ExFeature> features;
	features.resize(mainWin->urbanGeometry->areas.selectedArea()->hintLine.size());
	for (int i = 0; i < mainWin->urbanGeometry->areas.selectedArea()->hintLine.size(); ++i) {
		QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));
		if (filename.isEmpty()) return;
	
		features[i].load(filename);
	}


	G::global()["numAvenueIterations"] = ui.lineEditNumAvenueIterations->text().toInt();
	G::global()["numStreetIterations"] = ui.lineEditNumStreetIterations->text().toInt();
	G::global()["cleanAvenues"] = ui.checkBoxCleanAvenues->isChecked();
	G::global()["cleanStreets"] = ui.checkBoxCleanStreets->isChecked();
	G::global()["roadExactSimilarityFactor"] = ui.horizontalSliderExactSimilarityFactor->value() * 0.01f;
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();

	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["areaScaling"] = ui.checkBoxAreaScaling->isChecked();

	G::global()["coordiniates"] = ui.radioButtonCartesianCoordinate->isChecked() ? "cartesian" : "polar";

	int orientation = ui.dialOrientation->value() - 180;
	bool areaScaling = ui.checkBoxAreaScaling->isChecked();


	if (orientation != 0) {
		//feature.rotate(orientation);
	}

	if (areaScaling) {
		//feature.scale(mainWin->urbanGeometry->areas.selectedArea()->area);
	}

	mainWin->urbanGeometry->generateRoadsMultiEx(features);

	mainWin->glWidget->updateGL();
}

