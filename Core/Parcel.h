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

#include "GeometryObject.h"
#include "Polygon3D.h"
#include "BBox.h"

namespace ucore {

class Block;
class Building;
class Terrain;
class TextureManager;

class Parcel : public GeometryObject {
protected:
	Block* block;

	/** Contour of the parcel */
	Polygon3D contour;

	BBox bbox;

	std::vector<int> frontEdges;
	std::vector<int> rearEdges;
	std::vector<int> sideEdges;

	/** Building Wrapper */
	Building* building;

	/** Land use */
	int landUse;

public:
	Parcel(Block* block, const Loop3D& contour);
	~Parcel();

	//void setBlock(Block* block) { this->block = block; }

	/** getter for contour */
	Polygon3D& getContour() { return contour; }

	/** getter for bbox */
	BBox& getBBox() { return bbox; }

	//std::vector<int>& getFrontEdges() { return frontEdges; }
	std::vector<int>& getRearEdges() { return rearEdges; }
	std::vector<int>& getSideEdges() { return sideEdges; }

	/** getter/setter for building */
	Building* getBuilding() { return building; }
	void setBuilding(Building* building);

	/** getter/setter for landUse */
	int getLandUse() { return landUse; }
	void setLandUse(int landUse);

	bool computeBuildingFootprintPolygon(float frontSetback, float rearSetback, float sideSetback, float maxFrontage, float maxDepth, Loop3D &buildingFootprint);

	void adaptToTerrain(Terrain* terrain);

private:
	void findParcelFrontAndBackEdges();
	void _generateMeshVertices(TextureManager* textureManager);
};

} // namespace ucore