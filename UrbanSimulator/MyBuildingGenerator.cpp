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

#include "MyBuildingGenerator.h"
#include "../Core/Polygon3D.h"
#include "../Core/Util.h"
#include "../Core/Block.h"
#include "../Core/Building.h"
#include "../Core/BuildingFactory.h"
#include "../Core/SimpleBuilding.h"
#include "../Core/global.h"
#include "MainWindow.h"
#include "MyTerrain.h"
#include "MyUrbanGeometry.h"
#include <QtTest/qtest.h>



MyBuildingGenerator::MyBuildingGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry) {
	this->mainWin = mainWin;
	this->urbanGeometry = urbanGeometry;
}

MyBuildingGenerator::~MyBuildingGenerator() {
}

void MyBuildingGenerator::run() {
	std::vector<ucore::Block*>* blocks = &urbanGeometry->getBlocks();

	int count = 0;
	for (int i = 0; i < blocks->size(); ++i) {
		generateBlockBuildings(blocks->at(i));

		if (++count % 10 == 0) {
			QTest::qWait(10);
		}
	}
}

bool MyBuildingGenerator::generateParcelBuildings(ucore::Block* block, ucore::Parcel* parcel) {
	//if (parcel->getLandUse() == ucore::LandUse::kLUPark) return false;
	if (parcel->getContour().size() <= 2) return false;

	// compute the oriented bounding box
	QVector3D size;
	QMatrix4x4 xformMat;
	parcel->getContour().getMyOBB(size, xformMat);
	float minSide = size.x() > size.y() ? size.y() : size.x();

	// compute the center of the footprint
	QVector3D center = parcel->getContour().getCentroid();

	// retrieve info layers
	MyTerrain* terrain = urbanGeometry->terrain;
	ucore::InfoLayer* layerPopulation;// = urbanGeometry->getInfoLayerManager()->get(1);
	ucore::InfoLayer* layerJobs;// = urbanGeometry->getInfoLayerManager()->get(2);

	// retrive information
	float population = layerPopulation->getValue(center.x(), center.y());
	float jobs = layerJobs->getValue(center.x(), center.y());

	float numStories;
	ucore::Loop3D buildingBase;
	/*
	switch (parcel->getLandUse()) {
	case ucore::LandUse::kLUResS:
		numStories = 1 + (int)ucore::Util::genRand(0, 100) % 2;
		if (!generateBuildingBase(minSide * 0.3, minSide * 0.3, parcel->getContour().getContour(), xformMat, &buildingBase)) return false;
		break;
	case ucore::LandUse::kLUResM:
	case ucore::LandUse::kLUOfficeM:
	case ucore::LandUse::kLUOfficeS:
		numStories = (population + jobs) * ucore::Util::genRand(0.5, 1.5) * ucore::G::getFloat("buildingHeightFactor");
		if (!generateBuildingBase(minSide * 0.9, minSide * 0.6, parcel->getContour().getContour(), xformMat, &buildingBase)) return false;
		break;
	case ucore::LandUse::kLUResL:
	case ucore::LandUse::kLUOfficeL:
		numStories = (population + jobs) * ucore::Util::genRand(0.5, 2.0) * ucore::G::getFloat("buildingHeightFactor");
		if (!generateBuildingBase(minSide * 0.9, minSide * 0.9, parcel->getContour().getContour(), xformMat, &buildingBase)) return false;
		break;
	default:
		break;
	}
	*/

	if (numStories > minSide * ucore::G::getFloat("maxHeightBaseAspectRatio")) {
		numStories = minSide * ucore::G::getFloat("maxHeightBaseAspectRatio");
	}

	float height = ucore::G::getFloat("meanBuildingFloorHeight") * numStories;
	ucore::Building* building = urbanGeometry->getBuildingFactory()->createBuilding(buildingBase, numStories, height, rand() % 3); 
	//building->adaptToTerrain(terrain);
	parcel->setBuilding(building);

	return true;
}


bool MyBuildingGenerator::generateBlockBuildings(ucore::Block* block) {
	ucore::ParcelGraphVertexIter vi, viEnd;	

	for (boost::tie(vi, viEnd) = boost::vertices(block->getParcels()); vi != viEnd; ++vi) {					
		generateParcelBuildings(block, block->getParcel(*vi));
	}
	return true;
}

bool MyBuildingGenerator::generateBuildingBase(float width, float depth, ucore::Loop3D& contour, QMatrix4x4& mat, ucore::Loop3D* outBuildingBase) {
	float at, bt;
	QVector2D intPt;
	double reduceFactor = 0.99;

	float buildingBaseAboveGround = ucore::G::getFloat("buildingBaseAboveGround");
	float minBuildingArea = ucore::G::getFloat("minBuildingArea");

	if (width > 2.0 * depth) width = 2.0 * depth;
	if (depth > 2.0 * width) depth = 2.0 * width;

	bool exceeded = false;
	do {
		outBuildingBase->clear();
		QVector3D pt(-width/2, -depth/2, 0);
		outBuildingBase->push_back(pt * mat);
		pt = QVector3D(width/2, -depth/2, 0);
		outBuildingBase->push_back(pt * mat);
		pt = QVector3D(width/2, depth/2, 0);
		outBuildingBase->push_back(pt * mat);
		pt = QVector3D(-width/2, depth/2, 0);
		outBuildingBase->push_back(pt * mat);

		exceeded = false;
		for (int i = 0; i < outBuildingBase->size(); ++i) {
			QVector2D a1(outBuildingBase->at(i));
			QVector2D a2(outBuildingBase->at((i+1) % outBuildingBase->size()));

			for (int j = 0; j < contour.size(); ++j) {
				QVector2D b1(contour[j]);
				QVector2D b2(contour[(j+1) % contour.size()]);

				if (ucore::Util::segmentSegmentIntersectXY(a1, a2, b1, b2, &at, &bt, true, intPt)) {
					exceeded = true;
					break;
				}
			}

			if (exceeded) {
				reduceFactor *= reduceFactor;
				width *= reduceFactor;
				depth *= reduceFactor;
				if (width * depth < minBuildingArea) return false;
			}
		}
	} while (exceeded);

	for (int i = 0; i < outBuildingBase->size(); ++i) {
		outBuildingBase->at(i).setZ(buildingBaseAboveGround);
	}

	return true;
}


