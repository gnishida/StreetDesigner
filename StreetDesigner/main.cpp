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
#include <QVector3D>


int main(int argc, char *argv[]) {
	G::global()["showTerrain"] = true;
	G::global()["showRoads"] = true;
	G::global()["showBlocks"] = true;
	G::global()["showParcels"] = true;
	G::global()["showBuildings"] = true;
	G::global()["showParcels"] = true;
	G::global()["showTrees"] = true;

	G::global()["parcelAreaMean"] = 3600;
	G::global()["parcelAreaDeviation"] = 49;
	G::global()["parcelSplitDeviation"] = 0.19;
	G::global()["parkPercentage"] = 0.2f;
	G::global()["parksRatio"] = 0.05f;

	G::global()["buildingHeightMean"] = 12;
	G::global()["buildingHeightDeviation"] = 90;
	G::global()["buildingBaseAboveGround"] = 3.0f;
	G::global()["meanBuildingFloorHeight"] = 3.0f;
	G::global()["buildingHeightFactor"] = 0.067f;
	G::global()["maxHeightBaseAspectRatio"] = 1.0f;
	G::global()["minBuildingArea"] = 10.0f;

	G::global()["roadSnapFactor"] = 0.7f;
	//G::global()["roadAngleTolerance"] = 1.2566f;
	G::global()["roadAngleTolerance"] = 1.0f;

	QApplication a(argc, argv);
	MainWindow w;
	w.showMaximized();
	return a.exec();
}