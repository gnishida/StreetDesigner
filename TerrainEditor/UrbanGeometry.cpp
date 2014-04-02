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
#include "MainWindow.h"
#include "UrbanGeometry.h"

UrbanGeometry::UrbanGeometry(MainWindow* mainWin) {
	this->mainWin = mainWin;

	terrain = NULL;

	waterRenderer = new mylib::WaterRenderer(300, 3000, -2.0f);

	loadTerrain("data/default.trn");
}

UrbanGeometry::~UrbanGeometry() {
}

void UrbanGeometry::clear() {
	clearGeometry();
}

void UrbanGeometry::clearGeometry() {
	if (terrain != NULL) delete terrain;
}

void UrbanGeometry::render(mylib::TextureManager* textureManager) {
	glEnable(GL_LIGHTING);

	if (waterRenderer != NULL) {
		waterRenderer->renderMe(textureManager);
	}
	
	renderer.render(terrain, textureManager);

	// draw the area which is now being defined
	if (areaBuilder.selecting()) {
		areaBuilder.adaptToTerrain(terrain);
		rendererHelper.renderPolyline(areaBuilder.polyline3D(), QColor(0, 0, 255), GL_LINE_STIPPLE);
	}

	rendererHelper.renderArea(area, QColor(0, 0, 255), GL_LINE_STIPPLE);

	glDisable(GL_LIGHTING);
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

void UrbanGeometry::setElevation(float z) {
	for (int i = 0; i < terrain->getNumCols(); ++i) {
		for (int j = 0; j < terrain->getNumRows(); ++j) {
			float x = terrain->getCell(i, j).getX();
			float y = terrain->getCell(i, j).getY();

			if (area.contains(QVector3D(x, y, 0))) {
				terrain->getCell(i, j).setElevation(z);
				terrain->getCell(i, j).setCornersElevationModified();
			}
		}
	}
	terrain->setModified();
}