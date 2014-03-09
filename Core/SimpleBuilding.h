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

#include "Building.h"
#include "Polygon3D.h"

namespace ucore {

class Terrain;
class TextureManager;

class SimpleBuilding : public Building {
protected:
	QColor roofColor;

public:
	SimpleBuilding(const Loop3D& footprint, int numStories, float height);
	//SimpleBuilding(const QVector3D& pt, const QVector3D axis, float width, float depth, int numStories, float height);
	~SimpleBuilding();

	void adaptToTerrain(Terrain* terrain);

private:
	void _generateMeshVertices(TextureManager* textureManager);
};

} // namespace ucore