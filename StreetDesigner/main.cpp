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

#include <QtGui/QApplication>
#include <common/global.h>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
	mylib::G::global()["showTerrain"] = true;
	mylib::G::global()["showRoads"] = true;
	mylib::G::global()["showBlocks"] = true;
	mylib::G::global()["showParcels"] = true;
	mylib::G::global()["showBuildings"] = true;
	mylib::G::global()["showParcels"] = true;
	mylib::G::global()["showTrees"] = true;

	mylib::G::global()["parcelAreaMean"] = 3600;
	mylib::G::global()["parcelAreaDeviation"] = 49;
	mylib::G::global()["parcelSplitDeviation"] = 0.19;
	mylib::G::global()["parkPercentage"] = 0.2f;
	mylib::G::global()["parksRatio"] = 0.05f;

	mylib::G::global()["buildingHeightMean"] = 12;
	mylib::G::global()["buildingHeightDeviation"] = 90;
	mylib::G::global()["buildingBaseAboveGround"] = 3.0f;
	mylib::G::global()["meanBuildingFloorHeight"] = 3.0f;
	mylib::G::global()["buildingHeightFactor"] = 0.067f;
	mylib::G::global()["maxHeightBaseAspectRatio"] = 1.0f;
	mylib::G::global()["minBuildingArea"] = 10.0f;

	mylib::G::global()["snapToClosestVertexFactor"] = 0.7f;
	mylib::G::global()["roadAngleTolerance"] = 1.2566f;

	QApplication a(argc, argv);
	MainWindow w;
	w.showMaximized();
	return a.exec();
}