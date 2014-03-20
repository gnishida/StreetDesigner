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

#include <limits>
#include <iostream>
#include <QFile>
#include <common/common.h>
#include <common/global.h>
#include <render/WaterRenderer.h>
#include <render/TextureManager.h>
#include <render/Texture.h>
#include <render/GeometryObject.h>
#include <render/Terrain.h>
#include <road/GraphUtil.h>
#include <road/generator/KDERoadGenerator.h>
#include <road/feature/KDEFeature.h>
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "BlockGenerator.h"

UrbanGeometry::UrbanGeometry(MainWindow* mainWin) {
	this->mainWin = mainWin;

	terrain = NULL;

	waterRenderer = new mylib::WaterRenderer(300, 3000, -10.0f);

	loadTerrain("data/default.trn");

	selectedAreaIndex = -1;
}

UrbanGeometry::~UrbanGeometry() {
}

void UrbanGeometry::clear() {
	clearGeometry();
}

void UrbanGeometry::clearGeometry() {
	if (terrain != NULL) delete terrain;

	roads.clear();
}

void UrbanGeometry::generateRoads(RoadFeature &rf) {
	if (areas.selectedIndex == -1) return;

	KDERoadGenerator::generateRoadNetwork(areas.selectedArea()->roads, areas.selectedArea()->area, dynamic_cast<KDEFeature&>(*rf.features[0]));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateBlocks() {
	BlockGenerator generator(mainWin);
	generator.run();

	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i]->adaptToTerrain(terrain);
	}
}

void UrbanGeometry::render(mylib::TextureManager* textureManager) {
	glEnable(GL_LIGHTING);

	if (waterRenderer != NULL) {
		waterRenderer->renderMe(textureManager);
	}
	
	renderer.render(terrain, textureManager);
	renderer.render(&roads, textureManager);
	for (int i = 0; i < blocks.size(); ++i) {
		renderer.render(blocks[i], textureManager);
	}



	// draw the area which is now being defined
	if (areaBuilder.selecting()) {
		areaBuilder.adaptToTerrain(terrain);
		rendererHelper.renderPolyline(areaBuilder.polyline3D(), QColor(0, 0, 255), GL_LINE_STIPPLE);
	}

	// draw the areas
	for (int i = 0; i < areas.size(); ++i) {
		if (i == areas.selectedIndex) {
			rendererHelper.renderPolyline(areas[i]->area3D, QColor(0, 0, 255), GL_LINE_STIPPLE);
		} else {
			rendererHelper.renderPolyline(areas[i]->area3D, QColor(196, 196, 255), GL_LINE_STIPPLE);
		}

		// draw the road graph
		renderer.render(&areas[i]->roads, textureManager);
	}

	glDisable(GL_LIGHTING);
}

/**
 * Adapt all geometry objects to terrain.
 */
void UrbanGeometry::adaptToTerrain() {
	roads.adaptToTerrain(terrain);
}

void UrbanGeometry::mergeRoads() {
	areas.mergeRoads();
	GraphUtil::copyRoads(areas.roads, roads);

	areas.roads.clear();
	areas.selectedIndex = -1;
	areas.clear();
}

void UrbanGeometry::connectRoads() {
	KDERoadGenerator::connectRoads(roads, 200.0f, 0.15f);

	roads.adaptToTerrain(terrain);
}

void UrbanGeometry::newTerrain(int width, int depth, int cellLength) {
	clear();

	terrain = new mylib::Terrain(width, depth, cellLength);

	if (waterRenderer != NULL) {
		waterRenderer->setWidth(width);
		waterRenderer->setDepth(depth);
	}
}

void UrbanGeometry::loadTerrain(const QString &filename) {
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

	terrain = new mylib::Terrain(width, depth, cellLength);

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

void UrbanGeometry::saveTerrain(const QString &filename) {
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

void UrbanGeometry::loadRoads(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		std::cerr << "MyUrbanGeometry::loadInfoLayers... The file is not accessible: " << filename.toUtf8().constData() << endl;
		throw "The file is not accessible: " + filename;
	}

	roads.clear();
	GraphUtil::loadRoads(roads, filename);

	roads.adaptToTerrain(terrain);
}

void UrbanGeometry::saveRoads(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		std::cerr << "MyUrbanGeometry::loadInfoLayers... The file is not accessible: " << filename.toUtf8().constData() << endl;
		throw "The file is not accessible: " + filename;
	}

	GraphUtil::saveRoads(roads, filename);
}

void UrbanGeometry::perturbRoads(float perturbationFactor) {
	GraphUtil::perturb(areas.selectedArea()->roads, areas.selectedArea()->area, perturbationFactor);

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::loadAreas(const QString &filename) {
	areas.load(filename);

	for (int i = 0; i < areas.size(); ++i) {
		areas[i]->adaptToTerrain(terrain);
	}
}

void UrbanGeometry::saveAreas(const QString &filename) {
	areas.save(filename);
}
