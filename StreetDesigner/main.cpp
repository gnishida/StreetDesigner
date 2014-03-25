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


	// tessellation test
	Polygon3D polygon;
polygon.push_back(QVector3D(37.718,449.84,5.0f));
polygon.push_back(QVector3D(30.3862,449.331,5.0f));
/**/
/*polygon.push_back(QVector3D(20.4336,448.57,5.0f));
polygon.push_back(QVector3D(10.483,447.739,5.0f));
polygon.push_back(QVector3D(0.530501,446.866,5.0f));
polygon.push_back(QVector3D(-9.42185,445.963,5.0f));
polygon.push_back(QVector3D(-19.3715,445.038,5.0f));
polygon.push_back(QVector3D(-29.3084,444.081,5.0f));
polygon.push_back(QVector3D(-39.2225,443.028,5.0f));
polygon.push_back(QVector3D(-49.1251,441.843,5.0f));

polygon.push_back(QVector3D(-67.2296,439.549,5.0f));
polygon.push_back(QVector3D(-66.3253,432.225,5.0f));
polygon.push_back(QVector3D(-65.0829,422.31,5.0f));
polygon.push_back(QVector3D(-63.7782,412.399,5.0f));
polygon.push_back(QVector3D(-62.4852,402.476,5.0f));
polygon.push_back(QVector3D(-61.2608,392.535,5.0f));
polygon.push_back(QVector3D(-60.1465,382.573,5.0f));
polygon.push_back(QVector3D(-59.4326,375.037,5.0f));
polygon.push_back(QVector3D(-55.4499,376.831,5.0f));

polygon.push_back(QVector3D(-49.2427,379.654,5.0f));
polygon.push_back(QVector3D(-43.0303,382.539,5.0f));
polygon.push_back(QVector3D(-36.8084,385.421,5.0f));
polygon.push_back(QVector3D(-30.5906,388.27,5.0f));
polygon.push_back(QVector3D(-24.3756,391.141,5.0f));
polygon.push_back(QVector3D(-17.7988,394.181,5.0f));
polygon.push_back(QVector3D(-7.41065,394.058,5.0f));
polygon.push_back(QVector3D(2.57815,394.018,5.0f));
polygon.push_back(QVector3D(12.5696,394.027,5.0f));
polygon.push_back(QVector3D(22.557,394.084,5.0f));
polygon.push_back(QVector3D(32.5423,394.234,5.0f));
polygon.push_back(QVector3D(39.9101,394.391,5.0f));
*/
polygon.push_back(QVector3D(39.7343,402.635,5.0f));
polygon.push_back(QVector3D(39.4901,412.621,5.0f));
polygon.push_back(QVector3D(39.1896,422.589,5.0f));
polygon.push_back(QVector3D(38.7374,432.545,5.0f));
polygon.push_back(QVector3D(38.1814,442.506,5.0f));

	std::vector<Polygon3D> trapezoids = polygon.tessellate();










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

	G::global()["snapToClosestVertexFactor"] = 0.7f;
	G::global()["roadAngleTolerance"] = 1.2566f;

	QApplication a(argc, argv);
	MainWindow w;
	w.showMaximized();
	return a.exec();
}