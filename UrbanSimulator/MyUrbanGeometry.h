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

#include "../Core/InfoLayerManager.h"
#include "../Core/InfoLayer.h"
#include "../Core/TextureManager.h"
#include "../Core/InfoLayerRenderer.h"
#include "../Core/Block.h"
#include "../Core/Parcel.h"
#include "../Core/BuildingFactory.h"
#include "../Core/BuildingRenderer.h"
#include "../Core/WaterRenderer.h"
#include <road/RoadGraph.h>
#include "../Core/TerrainRenderer.h"
#include "../Core/ParcelRenderer.h"
#include "../Core/BlockRenderer.h"
#include "MyGeometryGenerator.h"
#include "MyTerrain.h"
#include "MyRoadGraphRenderer.h"
#include "MyTerrainRenderer.h"
#include <road/RoadAreaSet.h>
#include <QProgressBar>
#include <QMatrix4x4>
#include <common/PolygonBuilder.h>
#include <common/Renderer.h>
#include <road/feature/RoadFeature.h>

class MainWindow;

class MyUrbanGeometry {
public:
	int width;
	int depth;
	QVector3D pt;
	MainWindow* mainWin;
	::RoadGraph roads;
	std::vector<ucore::Block*> blocks;
	MyTerrain* terrain;
	std::vector<ucore::InfoLayer*> infoLayers;

	ucore::BuildingFactory* buildingFactory;

	MyTerrainRenderer terrainRenderer;

	/** Road Graph Renderer */
	MyRoadGraphRenderer roadGraphRenderer;

	ucore::BlockRenderer* blockRenderer;
	ucore::ParcelRenderer* parcelRenderer;
	ucore::BuildingRenderer* buildingRenderer;
	ucore::WaterRenderer* waterRenderer;

	MyGeometryGenerator* geometryGenerator;

	PolygonBuilder areaBuilder;
	RoadAreaSet areas;
	int selectedAreaIndex;
	Renderer renderer;


public:
	MyUrbanGeometry(MainWindow* mainWin, int width, int depth);
	~MyUrbanGeometry();

	/** getter for width */
	int getWidth() { return width; }

	/** getter for depth */
	int getDepth() { return depth; }

	/** getter for pt */
	QVector3D getPt() { return pt; }

	/** getter for blocks */
	std::vector<ucore::Block*>& getBlocks() { return blocks; }

	/** getter/setter for buildingFactory */
	ucore::BuildingFactory* getBuildingFactory() { return buildingFactory; }
	void setBuildingFactory(ucore::BuildingFactory* buildingFactory);

	/** setter for blockRenderer */
	void setBlockRenderer(ucore::BlockRenderer* blockRenderer) { this->blockRenderer = blockRenderer; }

	/** setter for parcelRenderer */
	void setParcelRenderer(ucore::ParcelRenderer* parcelRenderer) { this->parcelRenderer = parcelRenderer; }

	/** setter for buildingRenderer */
	void setBuildingRenderer(ucore::BuildingRenderer* buildingRenderer) { this->buildingRenderer = buildingRenderer; }

	void clear();
	void clearGeometry();
	void generate();
	void generateRoads(RoadFeature &rf);

	bool containsPoint(const QVector3D& testPt);

	void render(ucore::TextureManager* textureManager);
	void adaptToTerrain();

	void newTerrain(int width, int depth, int cellLength);
	void loadTerrain(const QString &filename);
	void saveTerrain(const QString &filename);

	void loadRoads(const QString &filename);
	void saveRoads(const QString &filename);
	void perturbRoads(float perturbationFactor);

	void loadAreas(const QString &filename);
	void saveAreas(const QString &filename);

private:
	void renderBlock(ucore::Block* block, ucore::TextureManager* textureManager);
	void renderParcel(ucore::Parcel* parcel, ucore::TextureManager* textureManager);
};
