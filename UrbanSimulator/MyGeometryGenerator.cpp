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

#include "MyGeometryGenerator.h"
#include "MyBlockGenerator.h"
#include "MyParcelGenerator.h"
#include "MyBuildingGenerator.h"
#include "MyUrbanGeometry.h"
#include "MainWindow.h"
#include <QtTest/qtest.h>
#include "../Core/global.h"



MyGeometryGenerator::MyGeometryGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry) {
}

MyGeometryGenerator::~MyGeometryGenerator() {
}

void MyGeometryGenerator::generate() {
	urbanGeometry->clearGeometry();

	//MyRoadGenerator rg(urbanMain, urbanGeometry);
	//rg.run();

	/*
	MyUrbanGeometry* ug = dynamic_cast<MyUrbanGeometry*>(urbanGeometry);

	MyBlockGenerator bg(urbanMain, urbanGeometry);;
	bg.run();

	bool showParcels = ucore::G::getBool("showParcels");
	ucore::G::global()["showParcels"] = true;
	MyParcelGenerator pg(urbanMain, urbanGeometry);
	pg.run();
	ucore::G::global()["showParcels"] = showParcels;

	MyBuildingGenerator dg(urbanMain, urbanGeometry);
	dg.start();
	while (dg.isRunning()) {
		urbanMain->updateGL();
		QTest::qWait(300);
	}
	*/
}
