#include "ControlWidget.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "GLWidget3D.h"
#include <common/global.h>
#include <road/GraphUtil.h>
#include <road/feature/ExFeature.h>


ControlWidget::ControlWidget(MainWindow* mainWin) : QDockWidget("Control Widget", (QWidget*)mainWin) {
	this->mainWin = mainWin;

	// set up the UI
	ui.setupUi(this);
	ui.lineEditNumAvenueIterations->setText("1000");
	ui.lineEditNumStreetIterations->setText("5000");
	ui.horizontalSliderInterpolationFactor->setMinimum(0);
	ui.horizontalSliderInterpolationFactor->setMaximum(100);
	ui.checkBoxCleanAvenues->setChecked(true);
	ui.checkBoxCleanStreets->setChecked(true);
	ui.checkBoxLocalStreets->setChecked(true);
	ui.checkBoxCropping->setChecked(false);
	ui.checkBoxAnimation->setChecked(false);
	ui.checkBoxAdaptiveFitting->setChecked(false);

	// register the event handlers
	connect(ui.horizontalSliderInterpolationFactor, SIGNAL(valueChanged(int)), this, SLOT(updateInterpolationFactor(int)));
	connect(ui.pushButtonGenerate, SIGNAL(clicked()), this, SLOT(generateRoads()));
	connect(ui.pushButtonGenerateInterpolation, SIGNAL(clicked()), this, SLOT(generateRoadsInterpolation()));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonConnect, SIGNAL(clicked()), this, SLOT(connectRoads()));
	connect(ui.pushButtonMerge, SIGNAL(clicked()), this, SLOT(mergeRoads()));

	hide();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void ControlWidget::updateInterpolationFactor(int value) {
	QString str;
	str.setNum(value);
	ui.labelInterpolationFactor->setText(str);
}

/**
 * Event handler for button [Generate Roads]
 */
void ControlWidget::generateRoads() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	G::global()["numAvenueIterations"] = ui.lineEditNumAvenueIterations->text().toInt();
	G::global()["numStreetIterations"] = ui.lineEditNumStreetIterations->text().toInt();
	G::global()["cleanAvenues"] = ui.checkBoxCleanAvenues->isChecked();
	G::global()["cleanStreets"] = ui.checkBoxCleanStreets->isChecked();
	G::global()["roadInterpolationFactor"] = ui.horizontalSliderInterpolationFactor->value() * 0.01f;
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();
	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["animation"] = ui.checkBoxAnimation->isChecked();

	if (ui.checkBoxAdaptiveFitting->isChecked()) {
		ExFeature feature;

		QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));
		if (filename.isEmpty()) return;

		feature.load(filename);

		mainWin->urbanGeometry->generateRoads(feature);
	} else {
		std::vector<ExFeature> features;
		features.resize(mainWin->urbanGeometry->areas.selectedArea()->hintLine.size());
		for (int i = 0; i < mainWin->urbanGeometry->areas.selectedArea()->hintLine.size(); ++i) {
			QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));
			if (filename.isEmpty()) return;
	
			features[i].load(filename);
		}

		mainWin->urbanGeometry->generateRoadsMultiEx(features);
	}
	
	mainWin->glWidget->updateGL();
}

/**
 * Event handler for button [Generate Roads]
 */
void ControlWidget::generateRoadsInterpolation() {
	if (mainWin->urbanGeometry->areas.selectedIndex == -1) return;

	G::global()["numAvenueIterations"] = ui.lineEditNumAvenueIterations->text().toInt();
	G::global()["numStreetIterations"] = ui.lineEditNumStreetIterations->text().toInt();
	G::global()["cleanAvenues"] = ui.checkBoxCleanAvenues->isChecked();
	G::global()["cleanStreets"] = ui.checkBoxCleanStreets->isChecked();
	G::global()["roadInterpolationFactor"] = ui.horizontalSliderInterpolationFactor->value() * 0.01f;
	G::global()["generateLocalStreets"] = ui.checkBoxLocalStreets->isChecked();
	G::global()["cropping"] = ui.checkBoxCropping->isChecked();
	G::global()["animation"] = ui.checkBoxAnimation->isChecked();

	ExFeature feature;
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Feature file..."), "", tr("StreetMap Files (*.xml)"));
	if (filename.isEmpty()) return;
	
	feature.load(filename);

	mainWin->urbanGeometry->generateRoadsInterpolation(feature);
	
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

