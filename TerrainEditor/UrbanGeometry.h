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
#include <render/Terrain.h>
#include <render/Renderer.h>
#include <render/RendererHelper.h>
#include <render/Renderer.h>

class MainWindow;

class UrbanGeometry {
public:
	int width;
	int depth;
	MainWindow* mainWin;
	mylib::Terrain* terrain;

	mylib::Renderer renderer;

	mylib::WaterRenderer* waterRenderer;

	PolygonBuilder areaBuilder;
	Polygon3D area;

	RendererHelper rendererHelper;


public:
	UrbanGeometry(MainWindow* mainWin);
	~UrbanGeometry();

	/** getter for width */
	int getWidth() { return width; }

	/** getter for depth */
	int getDepth() { return depth; }

	void clear();
	void clearGeometry();

	void render(mylib::TextureManager* textureManager);

	void newTerrain(int width, int depth, int cellLength);
	void loadTerrain(const QString &filename);
	void saveTerrain(const QString &filename);

	void setElevation(float z);

private:
};
