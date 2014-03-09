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

#include "MyUrbanGeometry.h"
#include "MyTerrain.h"
#include "MyRoadGraphRenderer.h"
#include "../Core/BlockRenderer.h"
#include "../Core/ParcelRenderer.h"
#include "../Core/BuildingRenderer.h"
#include "../Core/WaterRenderer.h"
#include "../Core/TextureManager.h"
#include "../Core/Texture.h"
#include "../Core/GeometryObject.h"
#include "MyTerrain.h"
#include <limits>
#include <iostream>
#include <QFile>
#include "MainWindow.h"
#include "../Core/global.h"
#include <road/GraphUtil.h>
#include <road/generator/KDERoadGenerator.h>
#include <road/feature/KDEFeature.h>

#define SQR(x)	((x) * (x))


MyUrbanGeometry::MyUrbanGeometry(MainWindow* mainWin, int width, int depth) {
	this->mainWin = mainWin;
	this->width = width;
	this->depth = depth;

	terrain = NULL;

	waterRenderer = new ucore::WaterRenderer(width, depth, -10.0f);

	loadTerrain("data/default.trn");

	selectedAreaIndex = -1;
}

MyUrbanGeometry::~MyUrbanGeometry() {
}

void MyUrbanGeometry::setBuildingFactory(ucore::BuildingFactory* buildingFactory) {
	this->buildingFactory = buildingFactory;
}

void MyUrbanGeometry::clear() {
	clearGeometry();
}

void MyUrbanGeometry::clearGeometry() {
	if (terrain != NULL) delete terrain;

	roads.clear();

	for (int i = 0; i < blocks.size(); ++i) {
		delete blocks[i];
	}
	blocks.clear();
}

void MyUrbanGeometry::generate() {
	if (geometryGenerator != NULL) {
		geometryGenerator->generate();
	}
}

void MyUrbanGeometry::generateRoads(RoadFeature &rf) {
	if (areas.selectedIndex == -1) return;

	KDERoadGenerator::generateRoadNetwork(areas.selectedArea().roads, areas.selectedArea().area, dynamic_cast<KDEFeature&>(*rf.features[0]));

	areas.selectedArea().roads.adaptToTerrain(terrain);
}

/**
 * returns true if bounding rectangle contains testPt
 */
bool MyUrbanGeometry::containsPoint(const QVector3D& testPt) {
	if (testPt.x() < -width/2.0f) return false;
	if (testPt.x() > width/2.0f) return false;
	if (testPt.y() < -depth/2.0f) return false;
	if (testPt.y() > depth/2.0f) return false;
	return true;
}

void MyUrbanGeometry::render(ucore::TextureManager* textureManager) {
	glEnable(GL_LIGHTING);

	if (waterRenderer != NULL) {
		waterRenderer->renderMe(textureManager);
	}
	
	terrainRenderer.render(terrain, textureManager);

	roadGraphRenderer.render(&roads, textureManager);

	// draw the building area
	if (areaBuilder.selecting()) {
		areaBuilder.adaptToTerrain(terrain);
		renderer.renderPolyline(areaBuilder.polyline3D(), QColor(0, 0, 255), GL_LINE_STIPPLE);
	}

	// draw the areas
	for (int i = 0; i < areas.size(); ++i) {
		areas[i].adaptToTerrain(terrain);
		if (i == areas.selectedIndex) {
			renderer.renderPolyline(areas[i].area3D, QColor(0, 0, 255), GL_LINE_STIPPLE);
		} else {
			renderer.renderPolyline(areas[i].area3D, QColor(196, 196, 255), GL_LINE_STIPPLE);
		}

		// draw the road graph
		roadGraphRenderer.render(&areas[i].roads, textureManager);
		//areas[i].roads.generateMesh();
		//renderer.render(areas[i].roads.renderables);
	}


	/*
	for (int i = 0; i < blocks.size(); ++i) {
		renderBlock(blocks[i], textureManager);
	}
	*/

	glDisable(GL_LIGHTING);
}

/**
 * Adapt all geometry objects to terrain.
 */
void MyUrbanGeometry::adaptToTerrain() {
	roads.adaptToTerrain(terrain);
	/*
	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i]->adaptToTerrain(terrain);
	}
	*/
}

void MyUrbanGeometry::newTerrain(int width, int depth, int cellLength) {
	clear();

	terrain = new MyTerrain(width, depth, cellLength);

	if (waterRenderer != NULL) {
		waterRenderer->setWidth(width);
		waterRenderer->setDepth(depth);
	}
}

void MyUrbanGeometry::loadTerrain(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		std::cerr << "MyUrbanGeometry::loadInfoLayers... The file is not accessible: " << filename.toUtf8().constData() << endl;
		throw "The file is not accessible: " + filename;
	}

	clear();

	QTextStream in(&file);
	QString line = in.readLine();
	this->width = line.split(" ")[0].toInt();
	this->depth = line.split(" ")[1].toInt();
	int cellLength = line.split(" ")[2].toInt();

	terrain = new MyTerrain(width, depth, cellLength);

	for (int i = 0; i < terrain->getNumRows() * terrain->getNumCols(); ++i) {
		line = in.readLine();
		int idxX = line.split(" ").at(1).toInt();
		int idxY = line.split(" ").at(2).toInt();

		line = in.readLine();
		float x = line.split(" ").at(0).toFloat();
		float y = line.split(" ").at(1).toFloat();

		line = in.readLine();
		terrain->setValue(x, y, line.toFloat());
	}

	if (waterRenderer != NULL) {
		waterRenderer->setWidth(width);// * 1.2f);
		waterRenderer->setDepth(depth);// * 1.2f);
	}
}

void MyUrbanGeometry::saveTerrain(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		std::cerr << "MyUrbanGeometry::saveInfoLayers... The file is not writable: " << filename.toUtf8().constData() << endl;
		throw "The file is not writable: " + filename;
	}

	QTextStream out(&file);
	out << this->width << " " << this->depth << " " << terrain->getCellLength() << endl;
	int count = 0;
	for (int i = 0; i < terrain->getNumCols(); ++i) {
		for (int j = 0; j < terrain->getNumRows(); ++j) {
			out << count++ << " " << i << " " << j << endl;
			float x = terrain->getCell(i, j).getX();
			float y = terrain->getCell(i, j).getY();

			out << x << " " << y << endl;

			out << terrain->getValue(x, y) << endl;
		}
	}
}

void MyUrbanGeometry::loadRoads(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		std::cerr << "MyUrbanGeometry::loadInfoLayers... The file is not accessible: " << filename.toUtf8().constData() << endl;
		throw "The file is not accessible: " + filename;
	}

	roads.clear();
	GraphUtil::loadRoads(roads, filename);

	roads.adaptToTerrain(terrain);
}

void MyUrbanGeometry::loadAreas(const QString &filename) {
	areas.load(filename);
}

void MyUrbanGeometry::renderBlock(ucore::Block* block, ucore::TextureManager* textureManager) {
	if (ucore::G::getBool("showBlocks") && blockRenderer != NULL) {
		blockRenderer->render(block, textureManager);
	}

	ucore::ParcelGraphVertexIter vi, viEnd;
	for(boost::tie(vi, viEnd) = boost::vertices(block->getParcels()); vi != viEnd; ++vi) {
		renderParcel(block->getParcels()[*vi], textureManager);
	}
}

void MyUrbanGeometry::renderParcel(ucore::Parcel* parcel, ucore::TextureManager* textureManager) {
	if (ucore::G::getBool("showParcels") && parcelRenderer != NULL) {
		parcelRenderer->render(parcel, textureManager);
	}

	if (ucore::G::getBool("showBuildings") && buildingRenderer != NULL && parcel->getBuilding() != NULL) {		
		buildingRenderer->render((ucore::GeometryObject*)parcel->getBuilding(), textureManager);
	}
}

