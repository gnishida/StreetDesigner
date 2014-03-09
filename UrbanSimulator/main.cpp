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
#include "../Core/global.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
	ucore::G::global()["showTerrain"] = true;
	ucore::G::global()["showRoads"] = true;
	ucore::G::global()["showBlocks"] = true;
	ucore::G::global()["showParcels"] = true;
	ucore::G::global()["showBuildings"] = true;
	ucore::G::global()["showParcels"] = true;
	ucore::G::global()["showTrees"] = true;

	ucore::G::global()["parcelAreaMean"] = 3600;
	ucore::G::global()["parcelAreaDeviation"] = 49;
	ucore::G::global()["parcelSplitDeviation"] = 0.19;
	ucore::G::global()["parkPercentage"] = 0.2f;
	ucore::G::global()["parksRatio"] = 0.05f;

	ucore::G::global()["buildingHeightMean"] = 12;
	ucore::G::global()["buildingHeightDeviation"] = 90;
	ucore::G::global()["buildingBaseAboveGround"] = 3.0f;
	ucore::G::global()["meanBuildingFloorHeight"] = 3.0f;
	ucore::G::global()["buildingHeightFactor"] = 0.067f;
	ucore::G::global()["maxHeightBaseAspectRatio"] = 1.0f;
	ucore::G::global()["minBuildingArea"] = 10.0f;

	ucore::G::global()["snapToClosestVertexFactor"] = 0.7f;
	ucore::G::global()["roadAngleTolerance"] = 1.2566f;

	QApplication a(argc, argv);
	MainWindow w;
	w.showMaximized();
	return a.exec();
}