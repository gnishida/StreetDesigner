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

#pragma once

#include <QProgressBar>
#include <QMatrix4x4>
#include <util/PolygonBuilder.h>
#include <render/TextureManager.h>
#include <render/WaterRenderer.h>
#include <render/MyTerrain.h>
#include <render/MyTerrainRenderer.h>
#include <render/MyRenderer.h>
#include <road/RoadGraph.h>
#include <road/RoadAreaSet.h>
#include <road/feature/RoadFeature.h>
#include "MyGeometryGenerator.h"
#include "MyRoadGraphRenderer.h"

class MainWindow;

class MyUrbanGeometry {
public:
	int width;
	int depth;
	QVector3D pt;
	MainWindow* mainWin;
	RoadGraph roads;
	MyTerrain* terrain;

	MyTerrainRenderer terrainRenderer;

	/** Road Graph Renderer */
	MyRoadGraphRenderer roadGraphRenderer;

	ucore::WaterRenderer* waterRenderer;

	MyGeometryGenerator* geometryGenerator;

	PolygonBuilder areaBuilder;
	RoadAreaSet areas;
	int selectedAreaIndex;
	MyRenderer renderer;


public:
	MyUrbanGeometry(MainWindow* mainWin, int width, int depth);
	~MyUrbanGeometry();

	/** getter for width */
	int getWidth() { return width; }

	/** getter for depth */
	int getDepth() { return depth; }

	/** getter for pt */
	QVector3D getPt() { return pt; }

	void clear();
	void clearGeometry();
	void generate();
	void generateRoads(RoadFeature &rf);

	bool containsPoint(const QVector3D& testPt);

	void render(ucore::TextureManager* textureManager);
	void adaptToTerrain();

	void mergeRoads();
	void connectRoads();

	void newTerrain(int width, int depth, int cellLength);
	void loadTerrain(const QString &filename);
	void saveTerrain(const QString &filename);

	void loadRoads(const QString &filename);
	void saveRoads(const QString &filename);
	void perturbRoads(float perturbationFactor);

	void loadAreas(const QString &filename);
	void saveAreas(const QString &filename);

private:
};
