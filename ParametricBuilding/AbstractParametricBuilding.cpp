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

#include "AbstractParametricBuilding.h"
#include "../Core/TextureManager.h"
#include "BuildingFace.h"
#include "../Core/RenderableCylinder.h"
#include "../Core/RenderableCircle.h"
#include "../Core/RenderableTriangleList.h"
#include "../Core/RenderableConcave.h"
#include "BuildingPolygonFace.h"
#include "../Core/Util.h"
#include <assert.h>
#include <iostream>
#include <QFile>

namespace pb {

bool AbstractParametricBuilding::roofTexturesLoaded = false;
std::vector<ucore::Texture*> AbstractParametricBuilding::roofTextures;
std::vector<ucore::Texture*> AbstractParametricBuilding::houseRoofTextures;
std::vector<QVector2D> AbstractParametricBuilding::roofTexCoord;
std::vector<QVector2D> AbstractParametricBuilding::houseRoofTexCoord;

AbstractParametricBuilding::AbstractParametricBuilding(const ucore::Loop3D& footprint, int numStories, float height) : ucore::Building(footprint, numStories, height) {
	setEnvelope();
}

AbstractParametricBuilding::~AbstractParametricBuilding() {
}

void AbstractParametricBuilding::loadRoofTextures(ucore::TextureManager* textureManager) {
	if (!roofTexturesLoaded) {
		std::vector<QString> houseRoofList;
		houseRoofList.push_back("data/buildings/roof/houseRoof0.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof1.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof2.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof3.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof4.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof5.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof6.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof7.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof8.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof9.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof10.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof11.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof12.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof13.jpg");
		houseRoofList.push_back("data/buildings/roof/houseRoof14.jpg");

		//initializeTextures(&houseRoofList, &houseRoofTextures, textureManager);
		for (int i = 0 ; i < houseRoofList.size() ; ++i) {
			if (!QFile::exists(houseRoofList[i])) {
				printf("AbstractParametricBuilding::loadRoofTextures()... The file does not exist: %s\n",houseRoofList[i].toUtf8().constData());
				throw "The file does not exist: " + houseRoofList[i];
			}

			ucore::Texture* texture = textureManager->get(houseRoofList[i]);
			houseRoofTextures.push_back(texture);
			houseRoofTexCoord.push_back(QVector2D(texture->getSizeX(), texture->getSizeY()));
		}

		std::vector<QString> roofList;
		roofList.push_back("data/buildings/roof/roof0.jpg");
		roofList.push_back("data/buildings/roof/roof1.jpg");
		roofList.push_back("data/buildings/roof/roof2.jpg");
		roofList.push_back("data/buildings/roof/roof3.jpg");
		roofList.push_back("data/buildings/roof/roof4.jpg");
		roofList.push_back("data/buildings/roof/roof5.jpg");
		roofList.push_back("data/buildings/roof/roof6.jpg");
		roofList.push_back("data/buildings/roof/roof7.jpg");
		roofList.push_back("data/buildings/roof/roof8.jpg");
		roofList.push_back("data/buildings/roof/roof9.jpg");
		roofList.push_back("data/buildings/roof/roof10.jpg");
		roofList.push_back("data/buildings/roof/roof11.jpg");
		roofList.push_back("data/buildings/roof/roof12.jpg");
		roofList.push_back("data/buildings/roof/roof13.jpg");
		roofList.push_back("data/buildings/roof/roof14.jpg");
		roofList.push_back("data/buildings/roof/roof15.jpg");
		roofList.push_back("data/buildings/roof/roof16.jpg");
		roofList.push_back("data/buildings/roof/roof17.jpg");
		roofList.push_back("data/buildings/roof/roof18.jpg");
		roofList.push_back("data/buildings/roof/roof19.jpg");
		roofList.push_back("data/buildings/roof/roof20.jpg");
		roofList.push_back("data/buildings/roof/roof21.jpg");
		roofList.push_back("data/buildings/roof/roof22.jpg");

		//initializeTextures(&roofList, &roofTextures, textureManager);
		for (int i = 0 ; i < roofList.size() ; ++i) {
			if (!QFile::exists(roofList[i])) {
				printf("AbstractParametricBuilding::loadRoofTextures()... The file does not exist: %s\n",roofList[i].toUtf8().constData());
				throw "The file does not exist: " + roofList[i];
			}

			ucore::Texture* texture = textureManager->get(roofList[i]);
			roofTextures.push_back(texture);
			roofTexCoord.push_back(QVector2D(texture->getSizeX(), texture->getSizeY()));
		}

		roofTexturesLoaded = true;
	}
}

int AbstractParametricBuilding::bestTextureForRoof(float x,float y) {
	if (roofTexCoord.size() == 1) return 0;

	int bestTex = 0;

	//find the two roof the closest aspect ratio
	float minMin = abs(x / y - roofTexCoord.at(0).x() / roofTexCoord.at(0).y());
	float min = abs(x / y - roofTexCoord.at(1).x() / roofTexCoord.at(1).y());
	int indMinMin = 0;
	int indMin = 1;
	if (minMin > min) {
		indMinMin = 1;
		indMin = 0;
		float sw = minMin;
		minMin = min;
		min = sw;
	}
	float div;
	for (int i = 2; i < roofTexCoord.size(); ++i) {
		div = abs(x / y - roofTexCoord.at(i).x() / roofTexCoord.at(i).y());
		if (div < minMin) {
			min = minMin;
			indMin = indMinMin;
			minMin = div;
			indMinMin = i;
		} else {
			if (div < min) {
				min = div;
				indMin = i;
			}
		}
	}
	srand(x * y);
	int m = rand() % 2;
	if (m == 0)
		return indMinMin;
	else
		return indMin;
}

void AbstractParametricBuilding::renderFace(ucore::Renderable* r, int face, float orgX, float orgY, float orgZ, float width, float depth, float height, std::vector<QVector3D>* normals, float s0, float s1, float t0, float t1) {
	//ucore::RenderableQuadList* renderable = (ucore::RenderableQuadList*)(r);
	ucore::RenderableTriangleList* renderable = (ucore::RenderableTriangleList*)(r);

	std::vector<QVector3D> towerBB;
	towerBB.push_back(QVector3D(orgX, orgY, orgZ));
	towerBB.push_back(QVector3D(orgX + width, orgY, orgZ));
	towerBB.push_back(QVector3D(orgX + width, orgY + depth, orgZ));
	towerBB.push_back(QVector3D(orgX, orgY + depth, orgZ));

	towerBB.push_back(QVector3D(orgX, orgY, orgZ + height));
	towerBB.push_back(QVector3D(orgX + width, orgY, orgZ + height));
	towerBB.push_back(QVector3D(orgX + width, orgY + depth, orgZ + height));
	towerBB.push_back(QVector3D(orgX, orgY + depth, orgZ + height));

	switch (face) {
	case 2:
		renderable->addTriangle(towerBB[1], towerBB[2], towerBB[6], normals->at(1), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[1], towerBB[6], towerBB[5], normals->at(1), s0, t0, s1, t1, s0, t1);
		break;
	case 4:
		renderable->addTriangle(towerBB[3], towerBB[0], towerBB[4], normals->at(3), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[3], towerBB[4], towerBB[7], normals->at(3), s0, t0, s1, t1, s0, t1);
		break;
	case 1:
		renderable->addTriangle(towerBB[2], towerBB[3], towerBB[7], normals->at(0), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[2], towerBB[7], towerBB[6], normals->at(0), s0, t0, s1, t1, s0, t1);
		break;
	case 3:
		renderable->addTriangle(towerBB[0], towerBB[1], towerBB[5], normals->at(2), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[0], towerBB[5], towerBB[4], normals->at(2), s0, t0, s1, t1, s0, t1);
		break;
	case 5:
		renderable->addTriangle(towerBB[4], towerBB[5], towerBB[6], normals->at(4), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[4], towerBB[6], towerBB[7], normals->at(4), s0, t0, s1, t1, s0, t1);
		break;
	case 6:
		renderable->addTriangle(towerBB[0], towerBB[3], towerBB[2], normals->at(5), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[0], towerBB[2], towerBB[1], normals->at(5), s0, t0, s1, t1, s0, t1);
		break;
	case 7:
		renderable->addTriangle(towerBB[3], towerBB[1], towerBB[5], normals->at(6), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[3], towerBB[5], towerBB[7], normals->at(6), s0, t0, s1, t1, s0, t1);
		break;
	case 8:
		renderable->addTriangle(towerBB[0], towerBB[2], towerBB[6], normals->at(7), s0, t0, s1, t0, s1, t1);
		renderable->addTriangle(towerBB[0], towerBB[6], towerBB[4], normals->at(7), s0, t0, s1, t0, s1, t1);
		break;
	}
}

void AbstractParametricBuilding::renderFace(ucore::Renderable* renderable, int face, float orgX, float orgY, float orgZ, float width, float depth, float height, std::vector<QVector3D>* normals, float s1, float t1) {
	renderFace(renderable, face, orgX, orgY, orgZ, width, depth, height, normals, 0, s1, 0, t1);
}

void AbstractParametricBuilding::renderFlatRoof(ucore::Polygon3D& p, ucore::Texture* texture) {
	if (p.size() < 3) return;

	renderables.push_back(new ucore::RenderableConcave(p.getContour(), texture));
}

void AbstractParametricBuilding::renderCylinder(float orgX, float orgY, float orgZ, float cbase,float ctop, float cheight, int cslices,int cstacks, ucore::Texture* texture1, ucore::Texture* texture2){
	renderables.push_back(new ucore::RenderableCylinder(orgX, orgY, orgZ, cbase, ctop, cheight, cslices, cstacks, texture1));
	renderables.push_back(new ucore::RenderableCircle(orgX, orgY, orgZ + cheight, ctop, cslices, texture2));
}

void AbstractParametricBuilding::drawRoofTriangle(ucore::Renderable* r, ucore::Renderable* r2, float orgX, float orgY, float orgZ, float dx,float dy,float dz,std::vector<QVector3D>* normals, float yTexShift, bool turn90, bool half, float s0) {
	//ucore::RenderableTriangleList* renderable1 = new ucore::RenderableTriangleList(texture);
	//ucore::RenderableTriangleList* renderable2 = new ucore::RenderableTriangleList(roofTexture);

	ucore::RenderableTriangleList* renderable = (ucore::RenderableTriangleList*)(r);
	ucore::RenderableTriangleList* renderableRoof = (ucore::RenderableTriangleList*)(r2);

	float roofTexX=dx;
	float roofTexY=dy;

	roofTexX = renderableRoof->getTexture()->getSizeX();//roofTexture->getSizeX();
	roofTexY = renderableRoof->getTexture()->getSizeY();//roofTexture->getSizeY();

	std::vector<QVector3D> v; //base and then top
	if (turn90==false) {
		v.push_back(QVector3D(orgX, orgY, orgZ));//0
		v.push_back(QVector3D(orgX, orgY + dy, orgZ));//1
		v.push_back(QVector3D(orgX + dx, orgY + dy, orgZ));//2
		v.push_back(QVector3D(orgX + dx, orgY, orgZ));//3

		v.push_back(QVector3D(orgX + dx/2.0f, orgY, orgZ + dz));//4
		v.push_back(QVector3D(orgX + dx/2.0f, orgY + dy, orgZ + dz));//5
	} else {
		v.push_back(QVector3D(orgX + dx, orgY, orgZ));//3
		v.push_back(QVector3D(orgX, orgY, orgZ));//0
		v.push_back(QVector3D(orgX, orgY + dy, orgZ));//1
		v.push_back(QVector3D(orgX + dx, orgY + dy, orgZ));//2

		v.push_back(QVector3D(orgX + dx, orgY + dy/2.0f, orgZ + dz));//4
		v.push_back(QVector3D(orgX, orgY + dy/2.0f, orgZ+ dz));//5
	}
	//front and back
	std::vector<QVector3D> t;

	t.push_back(v.at(0));
	if(half)
		t.push_back((v.at(3)+v.at(0))/2.0f);
	else 
		t.push_back(v.at(3));
	t.push_back(v.at(4));
	QVector3D n1 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderable->addTriangle(t[0], t[1], t[2], n1, 0, yTexShift, 1, yTexShift, 0.5, 1);

	//
	t.clear();

	if(half)
		t.push_back((v.at(2)+v.at(1))/2.0f);
	else
		t.push_back(v.at(2));
	t.push_back(v.at(1));
	t.push_back(v.at(5));
			
	QVector3D n2 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderable->addTriangle(t[0], t[1], t[2], n2, 0, yTexShift, 1, yTexShift, 0.5, 1);

	//roof
	t.clear();
	t.push_back(v.at(5));
	t.push_back(v.at(1));
	t.push_back(v.at(0));
	n2 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderableRoof->addTriangle(t[0], t[1], t[2], n2, 0, sqrt((dz*dz)+(dx*dx/4))/roofTexY, 0, 0, dy/roofTexX, 0);

	t.clear();
	t.push_back(v.at(4));
	t.push_back(v.at(5));
	t.push_back(v.at(0));
	n2 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderableRoof->addTriangle(t[0], t[1], t[2], n2, dy/roofTexX, sqrt((dz*dz)+(dx*dx/4))/roofTexY, 0, sqrt((dz*dz)+(dx*dx/4))/roofTexY, dy/roofTexX, 0);

	t.clear();
	t.push_back(v.at(2));
	t.push_back(v.at(5));
	t.push_back(v.at(4));
	n2 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderableRoof->addTriangle(t[0], t[1], t[2], n2, dy/roofTexX,0.0f,dy/roofTexX,sqrt((dz*dz)+(dx*dx/4))/roofTexY,0.0f,sqrt((dz*dz)+(dx*dx/4))/roofTexY);

	t.clear();
	t.push_back(v.at(3));
	t.push_back(v.at(2));
	t.push_back(v.at(4));
	n2 = ucore::Util::calculateNormal(t.at(2),t.at(0),t.at(1));
	renderableRoof->addTriangle(t[0], t[1], t[2], n2, 0.0f,0.0f,dy/roofTexX,0.0f,0.0f,sqrt((dz*dz)+(dx*dx/4))/roofTexY);

	//renderables.push_back(renderable1);
	//renderables.push_back(renderable2);
}

int AbstractParametricBuilding::setEnvelope() {
	std::vector<QVector3D> bb;

	float _height = 0.0f;
	bb.push_back(QVector3D(0, 0, _height));
	bb.push_back(QVector3D(width, 0, _height));
	bb.push_back(QVector3D(width, depth, _height));
	bb.push_back(QVector3D(0, depth, _height));

	_height = this->height;
	bb.push_back(QVector3D(0, 0, _height));
	bb.push_back(QVector3D(width, 0, _height));
	bb.push_back(QVector3D(width, depth, _height));
	bb.push_back(QVector3D(0, depth, _height));

	normals.clear();
	normals.push_back(QVector3D(0, 1, 0));
	normals.push_back(QVector3D(1, 0, 0));
	normals.push_back(QVector3D(0, -1, 0));
	normals.push_back(QVector3D(-1, 0, 0));
	normals.push_back(QVector3D(0, 0, 1));
	normals.push_back(QVector3D(0, 0, -1));
	normals.push_back(QVector3D(-0.7071, -0.7071, 0));
	normals.push_back(QVector3D(0.7071, -0.7071, 0));

	return 0;
}

} // namespace pb