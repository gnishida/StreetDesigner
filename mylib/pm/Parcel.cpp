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

#include "Parcel.h"
#include "../common/Util.h"
#include "LandUse.h"
#include "Block.h"
#include "../render/Terrain.h"
#include "../render/Renderable.h"
#include "../render/RenderableConcave.h"
#include "../render/RenderableLineList.h"

Parcel::Parcel(Block* block, const Polygon3D& contour) {
	this->block = block;
	//color = QColor(Util::genRand(150, 200), Util::genRand(150, 200), Util::genRand(50, 100));
	color = QColor(block->color.red() -20, block->color.green() - 20, block->color.blue() - 20);
	//building = NULL;
	//landUse = LandUse::kLUNone;

	this->contour = contour;

	findParcelFrontAndBackEdges();

	//bbox.recalculate(contour);

	setModified();
}

Parcel::~Parcel() {
	//if (building != NULL) delete building;
}

/*void Parcel::setBuilding(Building* building) {
	if (this->building != NULL) delete this->building;
	this->building = building;
}*/

void Parcel::setLandUse(int landUse) {
	this->landUse = landUse;

	setModified();
}

/**
* Compute Building Footprint Polygon
**/
/*
bool Parcel::computeBuildingFootprintPolygon(float frontSetback, float rearSetback, float sideSetback, float maxFrontage, float maxDepth, Loop3D &buildingFootprint) {
	Loop3D buildableContour;

	size_t contourSz = contour.getContour().size();
	if (contourSz < 3) return false;
									
	//--- first, initialize values to side setback (most edges are sides)
	std::vector<float> offsetValues(contourSz, sideSetback);

	//--- then, append front ant back values
	for (int i = 0; i < frontEdges.size(); ++i) {
		if (frontEdges[i] < offsetValues.size()) {
			offsetValues[frontEdges[i]] = frontSetback;
		}
	}

	for (int i = 0; i < rearEdges.size(); ++i) {
		if (rearEdges[i] < offsetValues.size()) {
			offsetValues[rearEdges[i]] = rearSetback;
		}
	}
			
	contour.computeInset(offsetValues, buildableContour);
	if (Polygon3D::isSelfIntersecting(buildableContour)) return false;

	if ((maxFrontage < 1.0f) || (maxDepth < 1.0f)) {
		buildingFootprint = buildableContour;
		return true;
	}

	buildingFootprint.clear();

	int frontageIdx = -1;
	int frontageIdxNext;

	int baSz = buildableContour.size();
	if (baSz < 3) return false;

	float curLength;
	float maxBALength = -FLT_MAX;

	int thisIdx;
	int nextIdx;

	bool orientedCW = Polygon3D::reorientFace(buildableContour, true);

	for (int i = 0; i < frontEdges.size(); ++i) {
		thisIdx = frontEdges.at(i);
		if (orientedCW) {			
			nextIdx = (thisIdx - 1 + baSz) % baSz;
		} else {
			nextIdx = (thisIdx + 1) % baSz;		
		}
		curLength = (buildableContour[thisIdx] - buildableContour[nextIdx]).lengthSquared();		
		if (curLength > maxBALength) {
			maxBALength = curLength;
			frontageIdx = thisIdx;
			frontageIdxNext = nextIdx;
		}
	}

	maxBALength = sqrt(maxBALength);

	if (frontageIdx == -1) return false;

	QVector3D frontPtA, frontPtB;
	QVector3D rearPtA,  rearPtB;

	QVector3D frontageCenter = 0.5f * (buildableContour[frontageIdx] + buildableContour[frontageIdxNext]);
	QVector3D frontageVector = (buildableContour[frontageIdx] - buildableContour[frontageIdxNext]).normalized();
	QVector3D depthVector(frontageVector.y(), -frontageVector.x(), frontageVector.z());

	float actualFrontage = std::min<float>(maxFrontage, maxBALength);
	float actualDepth = maxDepth + Util::genRand(-0.05f, 0.05f) * maxDepth;

	frontPtA = frontageCenter - 0.5 * actualFrontage * frontageVector;
	frontPtB = frontageCenter + 0.5 * actualFrontage * frontageVector;
	rearPtA  = frontPtA + depthVector * actualDepth;
	rearPtB  = frontPtB + depthVector * actualDepth;

	buildingFootprint.push_back(rearPtA);
	buildingFootprint.push_back(rearPtB);
	buildingFootprint.push_back(frontPtB);
	buildingFootprint.push_back(frontPtA);	

	return true;
}
*/

void Parcel::adaptToTerrain(mylib::Terrain* terrain) {
	for (int i = 0; i < contour.size(); ++i) {
		float z = terrain->getValue(contour[i].x(), contour[i].y());
		contour[i].setZ(z + 0.5f);
	}

	setModified();
}

void Parcel::_generateMeshVertices(mylib::TextureManager* textureManager) {
	if (contour.size() < 3) return;

	//renderables.push_back(mylib::RenderablePtr(new mylib::RenderableConcave(contour, color)));

	mylib::RenderableLineList* renderable1 = new mylib::RenderableLineList();
	QVector3D normal(0, 0, 1);// = Util::calculateNormal(contour[0], contour[1], contour[2]);
	renderable1->addLine(contour, normal, color);

	renderables.push_back(mylib::RenderablePtr(renderable1));
}

void Parcel::findParcelFrontAndBackEdges() {
	QVector3D midPt;
	int next;

	frontEdges.clear();
	rearEdges.clear();
	sideEdges.clear();

	float kDistTol = 0.01f;

	for (int i = 0; i < contour.size(); ++i) {
		next = (i + 1) % contour.size();

		midPt = 0.5f * (contour.at(i) + contour.at(next));
		float distPtThis = boost::geometry::distance(block->getContour(), contour[i]);
		float distPtNext = boost::geometry::distance(block->getContour(), contour[next]);
		float distPtMid = boost::geometry::distance(block->getContour(), midPt);

		int numPtsThatAreClose = (int)(distPtThis < kDistTol) + (int)(distPtMid < kDistTol) + (int)(distPtNext < kDistTol);

		bool midPtIsClose = (distPtThis < kDistTol);

		switch (numPtsThatAreClose) {
		case 0:
			rearEdges.push_back(i);
			break;
		case 1:					
		case 2:
			sideEdges.push_back(i);
			break;
		case 3:
			frontEdges.push_back(i);
			break;
		}
	}
}
