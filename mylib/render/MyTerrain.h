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

#include <render/GeometryObject.h>
#include "MyTerrainCell.h"
#include <render/TextureManager.h>
#include <common/ElevationColorTable.h>


class MyTerrain : public ucore::GeometryObject {
private:
	int width;
	int depth;
	int numCols;
	int numRows;
	int cellLength;
	std::vector<MyTerrainCell> cells;

	ucore::ElevationColorTable colorTable;

public:
	MyTerrain(int width, int depth, int cellLength);
	~MyTerrain();

	int getNumCols() { return numCols; }
	int getNumRows() { return numRows; }
	int getCellLength() { return cellLength; }
	MyTerrainCell& getCell(int idxX, int idxY);

	void init();
	float getValue(int x, int y);
	void setValue(int x, int y, float elevation);
	void addValue(int x, int y, float change);
	//void recalcCornersElevation();
	float getSlope(int x, int y);
	float getSlopeAtCell(int idxX, int idxY);

	void _generateMeshVertices(ucore::TextureManager* textureManager);
};

