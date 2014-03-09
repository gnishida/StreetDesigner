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

#include "MyParcelGenerator.h"
#include "../Core/Polygon3D.h"
#include "../Core/Block.h"
#include "../Core/Parcel.h"
#include "../Core/LandUse.h"
#include "../Core/InfoLayer.h"
#include "../Core/InfoLayerManager.h"
#include "../Core/Util.h"
#include <QtTest/qtest.h>
#include <QMatrix4x4>
#include "../Core/global.h"
#include "MainWindow.h"
#include "MyUrbanGeometry.h"



MyParcelGenerator::MyParcelGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry) {
	this->mainWin = mainWin;
	this->urbanGeometry = urbanGeometry;
}

MyParcelGenerator::~MyParcelGenerator() {
}

void MyParcelGenerator::run() {
	std::vector<ucore::Block*>* blocks = &urbanGeometry->getBlocks();

	std::cout << "start #" << blocks->size() << "...";

	int count = 0;
	for (int i = 0; i < blocks->size(); ++i) {
		subdivideBlockIntoParcels(blocks->at(i));

		if (++count % 100 == 0) {
			mainWin->glWidget->updateGL();
			QTest::qWait(1);
		}
	}
	std::cout << "end...";

	mainWin->glWidget->updateGL();
	QTest::qWait(1);
}

void MyParcelGenerator::subdivideBlockIntoParcels(ucore::Block* block) {	
	srand(block->getRandSeed());
	std::vector<ucore::Parcel*> tmpParcels;

	block->clearParcels();

	subdivideParcel(block, block->getContour(), ucore::G::getFloat("parcelAreaMean"), ucore::G::getFloat("parcelAreaDeviation")/100.0f, ucore::G::getFloat("parcelSplitDeviation"), tmpParcels);

	for (int i = 0; i < tmpParcels.size(); ++i) {
		block->addParcel(tmpParcels[i]);
	}
}

/**
* Parcel subdivision
* @desc: Recursive subdivision of a parcel using OBB technique
* @return: true if parcel was successfully subdivided, false otherwise
* @areaMean: mean target area of parcels after subdivision
* @areaVar: variance of parcels area after subdivision (normalized in 0-1)
* @splitIrregularity: A normalized value 0-1 indicating how far
*					from the middle point the split line should be
**/
bool MyParcelGenerator::subdivideParcel(ucore::Block* block, ucore::Polygon3D& contour, float areaMean, float areaStd, float splitIrregularity, std::vector<ucore::Parcel*>& outParcels) {
	float thresholdArea = areaMean + ucore::Util::genRand(-1.0f, 1.0f) * areaStd * areaMean;

	if (contour.computeArea(true) < thresholdArea) {
		QVector3D center = contour.getCentroid();

		ucore::InfoLayer* layerPopulation = urbanGeometry->infoLayers[1];
		ucore::InfoLayer* layerJobs = urbanGeometry->infoLayers[2];
		float population = layerPopulation->getValue(center.x(), center.y());
		float jobs = layerJobs->getValue(center.x(), center.y());

		ucore::Parcel* parcel = new ucore::Parcel(block, contour.getContour());

		if (ucore::Util::genRand(0, 1) < ucore::G::getFloat("parksRatio")) {
			parcel->setLandUse(ucore::LandUse::kLUPark);		// Park
		} else if (population + jobs < 100 + ucore::Util::genRand(0, 200)) {
			parcel->setLandUse(ucore::LandUse::kLUResS);		// Residential area
		} else if (population + jobs < 600 + ucore::Util::genRand(0, 200)) {
			if (population > jobs * ucore::Util::genRand(0.7, 1.3)) {
				parcel->setLandUse(ucore::LandUse::kLUResM);	// Residential area
			} else {
				parcel->setLandUse(ucore::LandUse::kLUOfficeM);	// Office area
			}
		} else {
			if (population > jobs * ucore::Util::genRand(0.7, 1.3)) {
				parcel->setLandUse(ucore::LandUse::kLUResL);	// Residential
			} else {
				parcel->setLandUse(ucore::LandUse::kLUOfficeL);	// Office area
			}
		}

		outParcels.push_back(parcel);
		return false;
	}

	//compute OBB
	QVector3D obbSize;
	QMatrix4x4 obbMat;
	contour.getMyOBB(obbSize, obbMat);

	//compute split line passing through center of OBB TODO (+/- irregularity)
	//		and with direction parallel/perpendicular to OBB main axis
	QVector3D slEndPoint;
	QVector3D dirVectorInit, dirVector, dirVectorOrthogonal;
	QVector3D midPt(0.0f, 0.0f, 0.0f);
	QVector3D auxPt(1.0f, 0.0f, 0.0f);
	QVector3D midPtNoise(0.0f, 0.0f, 0.0f);
	std::vector<QVector3D> splitLine;	

	midPt = midPt * obbMat;

	dirVectorInit = auxPt * obbMat - midPt;
	dirVectorInit.normalize();
	if (obbSize.x() > obbSize.y()) {
		dirVector.setX( -dirVectorInit.y() );
		dirVector.setY(  dirVectorInit.x() );
	} else {
		dirVector = dirVectorInit;
	}

	midPtNoise.setX( splitIrregularity * ucore::Util::genRand(-10.0f, 10.0f) );
	midPtNoise.setY( splitIrregularity * ucore::Util::genRand(-10.0f, 10.0f) );
	midPt = midPt + midPtNoise;

	slEndPoint = midPt + 10000.0f * dirVector;
	splitLine.push_back(slEndPoint);
	slEndPoint = midPt - 10000.0f * dirVector;
	splitLine.push_back(slEndPoint);

	//split parcel with line and obtain two new parcels
	ucore::Polygon3D pgon1, pgon2;

	float kDistTol = 0.01f;

	if (contour.splitMeWithPolyline(splitLine, pgon1.getContour(), pgon2.getContour())) {
		//check if new contours of pgon1 and pgon2 "touch" the boundary of the block
		if (ucore::Polygon3D::distanceXYfromContourAVerticesToContourB(pgon1.getContour(), block->getContour().getContour()) > kDistTol 
			|| ucore::Polygon3D::distanceXYfromContourAVerticesToContourB(pgon2.getContour(), block->getContour().getContour()) > kDistTol) {
			splitLine.clear();
			pgon1.clear();
			pgon2.clear();

			//if they don't have street access, rotate split line by 90 degrees and recompute
			dirVectorOrthogonal.setX(-dirVector.y());
			dirVectorOrthogonal.setY(dirVector.x());

			slEndPoint = midPt + 10000.0f * dirVectorOrthogonal;
			splitLine.push_back(slEndPoint);
			slEndPoint = midPt - 10000.0f * dirVectorOrthogonal;
			splitLine.push_back(slEndPoint);			

			contour.splitMeWithPolyline(splitLine, pgon1.getContour(), pgon2.getContour());
		}

		subdivideParcel(block, pgon1, areaMean, areaStd, splitIrregularity, outParcels);
		subdivideParcel(block, pgon2, areaMean, areaStd, splitIrregularity, outParcels);
	} else {
		return false;
	}

	return true;
}

void MyParcelGenerator::setParcelsAsParks(std::vector<ucore::Block*>& blocks) {
	std::vector<ucore::Parcel*> parcelPtrs;

	bool isFirst = true;
	int seedOfFirstBlock = 0;

	//get all the parcels of that place type in an array
	for (int j = 0; j < blocks.size(); ++j) {
		ucore::ParcelGraphVertexIter vi, viEnd;
		for (boost::tie(vi, viEnd) = boost::vertices(blocks[j]->getParcels()); vi != viEnd; ++vi) {
			if (isFirst) {
				seedOfFirstBlock = blocks[j]->getRandSeed();
				isFirst = false;
			}

			//blocks[j].getParcel(*vi).getBuildingAttributes().att_is_park = false;
			parcelPtrs.push_back(blocks[j]->getParcel(*vi));
		}
	}

	srand(seedOfFirstBlock);

	float parkPercentage = ucore::G::getFloat("parkPercentage");

	//shuffle and select first parkPercentage %
	int numToSetAsParks = (int)(parkPercentage * (float)parcelPtrs.size());
	std::random_shuffle( parcelPtrs.begin(), parcelPtrs.end() );

	int countMax = std::min<float>( parcelPtrs.size(), numToSetAsParks );
	for (int i = 0; i < countMax ; ++i) {
		parcelPtrs[i]->setLandUse(ucore::LandUse::kLUPark);
	}
}

