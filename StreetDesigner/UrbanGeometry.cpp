﻿/*********************************************************************
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
#include "GraphUtil.h"
#include "UShapeRoadGenerator.h"
#include "MultiExRoadGenerator.h"
#include "MultiIntExRoadGenerator.h"
#include "IntRoadGenerator.h"
#include "WarpRoadGenerator.h"
#include "SmoothWarpRoadGenerator.h"
#include "VerySmoothWarpRoadGenerator.h"
#include "RoadGeneratorHelper.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"
#include "BlockGenerator.h"
#include "ParcelGenerator.h"

UrbanGeometry::UrbanGeometry(MainWindow* mainWin) {
	this->mainWin = mainWin;

	terrain = NULL;

	waterRenderer = new mylib::WaterRenderer(3000, 3000, -2.0f);

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

	// clean up memory allocated for blocks
	for (int i = 0; i < blocks.size(); ++i) {
		delete blocks[i];
	}
	blocks.clear();
}

void UrbanGeometry::generateRoadsMultiEx(std::vector<ExFeature> &features) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	MultiExRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, features);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsMultiIntEx(std::vector<ExFeature> &features) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	MultiIntExRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, features);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsInterpolation(ExFeature &feature) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	IntRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, feature);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsWarp(ExFeature &feature) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	WarpRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, feature);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsSmoothWarp(ExFeature &feature) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	SmoothWarpRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, feature);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsVerySmoothWarp(ExFeature &feature) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	VerySmoothWarpRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, feature);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateRoadsUShape(ExFeature &feature) {
	if (areas.selectedIndex == -1) return;
	if (areas.selectedArea()->hintLine.size() == 0) return;

	UShapeRoadGenerator generator(mainWin, areas.selectedArea()->roads, areas.selectedArea()->area, areas.selectedArea()->hintLine, terrain, feature);
	generator.generateRoadNetwork(G::getBool("animation"));

	areas.selectedArea()->roads.adaptToTerrain(terrain);
}

void UrbanGeometry::generateBlocks() {
	BlockGenerator generator(mainWin);
	generator.run();

	for (int i = 0; i < blocks.size(); ++i) {
		blocks[i]->adaptToTerrain(terrain);
	}
}

void UrbanGeometry::generateParcels() {
	ParcelGenerator generator(mainWin);
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
	
	// draw a terrain
	renderer.render(terrain, textureManager);

	// draw a road network
	renderer.render(&roads, textureManager);

	// draw blocks and parcels
	for (int i = 0; i < blocks.size(); ++i) {
		renderer.render(blocks[i], textureManager);

		ParcelGraphVertexIter vi, viEnd;
		for(boost::tie(vi, viEnd) = boost::vertices(blocks[i]->parcels); vi != viEnd; ++vi) {
			renderer.render(blocks[i]->parcels[*vi], textureManager);
		}
	}

	// draw the area which is now being defined
	if (areaBuilder.selecting()) {
		areaBuilder.adaptToTerrain(terrain);
		rendererHelper.renderPolyline(areaBuilder.polyline3D(), QColor(0, 0, 255), GL_LINE_STIPPLE);
	}

	// draw a hint polyline
	if (hintLineBuilder.selecting()) {
		hintLineBuilder.adaptToTerrain(terrain);
		rendererHelper.renderPolyline(hintLineBuilder.polyline3D(), QColor(255, 0, 0), GL_LINE_STIPPLE);
	}

	// draw a avenue sketch polyline
	if (avenueBuilder.selecting()) {
		avenueBuilder.adaptToTerrain(terrain);
		rendererHelper.renderPolyline(avenueBuilder.polyline3D(), QColor(255, 255, 0), GL_LINE_STIPPLE);
	}

	// draw the areas
	for (int i = 0; i < areas.size(); ++i) {
		if (i == areas.selectedIndex) {
			rendererHelper.renderPolyline(areas[i]->area3D, QColor(0, 0, 255), GL_LINE_STIPPLE);
			rendererHelper.renderPolyline(areas[i]->hintLine3D, QColor(255, 0, 0), GL_LINE_STIPPLE);
		} else {
			rendererHelper.renderPolyline(areas[i]->area3D, QColor(196, 196, 255), GL_LINE_STIPPLE);
		}

		// draw the road graph
		areas[i]->roads.adaptToTerrain(terrain);
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

/**
 * 指定された道路を追加する。
 */
void UrbanGeometry::addRoad(int roadType, const Polyline2D &polyline, int lanes) {
	RoadVertexDesc v1_desc;
	if (!GraphUtil::getVertex(roads, polyline[0], 10.0f, v1_desc)) {
		RoadVertexPtr v1 = RoadVertexPtr(new RoadVertex(polyline[0]));
		v1_desc = boost::add_vertex(roads.graph);
		roads.graph[v1_desc] = v1;
	}

	RoadVertexDesc v2_desc;
	if (!GraphUtil::getVertex(roads, polyline.last(), 10.0f, v2_desc)) {
		RoadVertexPtr v2 = RoadVertexPtr(new RoadVertex(polyline.last()));
		v2_desc = boost::add_vertex(roads.graph);
		roads.graph[v2_desc] = v2;
	}

	RoadEdgeDesc e_desc = GraphUtil::addEdge(roads, v1_desc, v2_desc, roadType, lanes);
	roads.graph[e_desc]->polyline = polyline;

	GraphUtil::planarify(roads);

	roads.adaptToTerrain(terrain);
}

void UrbanGeometry::mergeRoads() {
	// もともとある道路の頂点は、すべてfixedにしておく
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		roads.graph[*vi]->fixed = true;
	}

	areas.mergeRoads();
	GraphUtil::mergeRoads(roads, areas.roads);

	areas.roads.clear();
	areas.selectedIndex = -1;
	areas.clear();
}

void UrbanGeometry::connectRoads() {
	RoadGeneratorHelper::connectRoads(roads, 200.0f, 0.15f);
	GraphUtil::removeDeadEnd(roads);

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
	out << terrain->width << " " << terrain->depth << " " << terrain->getCellLength() << endl;
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

/*
void UrbanGeometry::saveTerrain(const QString &filename) {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		std::cerr << "MyUrbanGeometry::saveInfoLayers... The file is not writable: " << filename.toUtf8().constData() << endl;
		throw "The file is not writable: " + filename;
	}

	float cellLength = terrain->getCellLength();
	QTextStream out(&file);
	out << terrain->width << " " << terrain->depth << " " << cellLength / 4 << endl;
	int count = 0;
	for (int i = 0; i < terrain->getNumCols() * 4; ++i) {
		for (int j = 0; j < terrain->getNumRows() * 4; ++j) {
			out << count++ << " " << i << " " << j << endl;
			float x = i * 10 - 1000;//terrain->getCell(i / 4, j / 4).getX();
			float y = j * 10 - 1000;//terrain->getCell(i / 4, j / 4).getY();

			out << x << " " << y << endl;

			float z = terrain->getValue(x, y);
			if (z < 0.0f && z > -3) {
				z = 0.0f;
			}
			out << z << endl;
		}
	}
}
*/

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

void UrbanGeometry::clearRoads() {
	roads.clear();
	for (int i = 0; i < blocks.size(); ++i) {
		delete blocks[i];
	}
	blocks.clear();
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
