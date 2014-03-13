/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include "MyControlWidget.h"
#include "../Core/UrbanMain.h"
#include "../Core/UrbanGeometry.h"
#include "MyGeometryAttributes.h"
#include "MyUrbanGeometry.h"
#include "../Core/global.h"
#include <QtTest/qtest.h>

namespace US {

MyControlWidget::MyControlWidget(ucore::UrbanMain* urbanMain, ucore::UrbanGeometry* urbanGeometry) : ucore::ControlWidget(tr("UrbanSimulator"), urbanMain, urbanGeometry) {
	ui.setupUi(this);

	//connect(ui.pushButtonElevation, SIGNAL(clicked(bool)), this, SLOT(showElevation(bool)));

	ui.checkBoxTerrain->setChecked(ucore::G::getBool("showTerrain"));
	ui.checkBoxRoads->setChecked(ucore::G::getBool("showRoads"));
	ui.checkBoxBlocks->setChecked(ucore::G::getBool("showBlocks"));
	ui.checkBoxParcels->setChecked(ucore::G::getBool("showParcels"));
	ui.checkBoxBuildings->setChecked(ucore::G::getBool("showBuildings"));
	ui.checkBoxTrees->setChecked(ucore::G::getBool("showTrees"));
	connect(ui.checkBoxTerrain, SIGNAL(clicked(bool)), this, SLOT(showTerrain(bool)));
	connect(ui.checkBoxRoads, SIGNAL(clicked(bool)), this, SLOT(showRoads(bool)));
	connect(ui.checkBoxBlocks, SIGNAL(clicked(bool)), this, SLOT(showBlocks(bool)));
	connect(ui.checkBoxParcels, SIGNAL(clicked(bool)), this, SLOT(showParcels(bool)));
	connect(ui.checkBoxBuildings, SIGNAL(clicked(bool)), this, SLOT(showBuildings(bool)));
	connect(ui.checkBoxTrees, SIGNAL(clicked(bool)), this, SLOT(showTrees(bool)));

	ui.lineEditSizeX->setText("30");
	ui.lineEditSizeY->setText("30");
	connect(ui.pushButtonNewCity, SIGNAL(clicked()), this, SLOT(newCity()));
	connect(ui.pushButtonSaveCity, SIGNAL(clicked()), this, SLOT(saveCity()));
	connect(ui.pushButtonLoadCity, SIGNAL(clicked()), this, SLOT(loadCity()));

	connect(ui.pushButtonGenerateAll, SIGNAL(clicked()), this, SLOT(generateAll()));
}

/*void MyControlWidget::showElevation(bool flag) {
	setChecked(MyGeometryAttributes::LAYER_ELEVATION);
	urbanGeometry->getInfoLayerManager()->select(MyGeometryAttributes::LAYER_ELEVATION);
	urbanMain->updateGL();
}*/

void MyControlWidget::showTerrain(bool flag) {
	ucore::G::global()["showTerrain"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::showRoads(bool flag) {
	ucore::G::global()["showRoads"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::showBlocks(bool flag) {
	ucore::G::global()["showBlocks"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::showParcels(bool flag) {
	ucore::G::global()["showParcels"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::showBuildings(bool flag) {
	ucore::G::global()["showBuildings"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::showTrees(bool flag) {
	ucore::G::global()["showTrees"] = flag;
	urbanMain->updateGL();
}

void MyControlWidget::generateAll() {
	setCursor(Qt::WaitCursor);
	disableAllButtons();
	QTest::qWait(30);

	ucore::G::global()["organicFactor"] = ui.comboBoxOrganicFactor->currentText();

	urbanGeometry->generate();
	urbanMain->updateGL();

	enableAllButtons();
	setCursor(Qt::ArrowCursor);
}

void MyControlWidget::newCity() {
	setCursor(Qt::WaitCursor);
	disableAllButtons();
	QTest::qWait(30);

	int x = ui.lineEditSizeX->text().toInt();
	int y = ui.lineEditSizeY->text().toInt();

	MyUrbanGeometry* ug = dynamic_cast<MyUrbanGeometry*>(urbanGeometry);
	ug->newInfoLayers(x * 100, y * 100);

	urbanMain->updateGL();

	enableAllButtons();
	setCursor(Qt::ArrowCursor);
}

void MyControlWidget::saveCity() {
	setCursor(Qt::WaitCursor);
	disableAllButtons();
	QTest::qWait(30);

	QString filename = QFileDialog::getSaveFileName(this, tr("Save city as ..."), QString(), tr("All Files (*)"));
	if (!filename.isEmpty()) {
		MyUrbanGeometry* ug = dynamic_cast<MyUrbanGeometry*>(urbanGeometry);
		ug->saveInfoLayers(filename);
	}

	enableAllButtons();
	setCursor(Qt::ArrowCursor);
}

void MyControlWidget::loadCity() {
	setCursor(Qt::WaitCursor);
	disableAllButtons();
	QTest::qWait(30);

	QString filename = QFileDialog::getOpenFileName(this, tr("Load city as ..."), QString(), tr("All Files (*)"));
	if (filename != QString::null && !filename.isEmpty()) {
		MyUrbanGeometry* ug = dynamic_cast<MyUrbanGeometry*>(urbanGeometry);
		ug->loadInfoLayers(filename);

		urbanMain->updateGL();
	}

	enableAllButtons();
	setCursor(Qt::ArrowCursor);
}

void MyControlWidget::disableAllButtons() {
	ui.pushButtonNewCity->setEnabled(false);
	ui.pushButtonLoadCity->setEnabled(false);
	ui.pushButtonSaveCity->setEnabled(false);
	ui.pushButtonGenerateAll->setEnabled(false);
}

void MyControlWidget::enableAllButtons() {
	ui.pushButtonNewCity->setEnabled(true);
	ui.pushButtonLoadCity->setEnabled(true);
	ui.pushButtonSaveCity->setEnabled(true);
	ui.pushButtonGenerateAll->setEnabled(true);
}

} // namespace US