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

#include "Terrain.h"

namespace ucore {

class TextureManager;
class Texture;

class SimpleFlatTerrain : public Terrain {
private:
	float elevation;
	//static Texture* texture;

public:
	SimpleFlatTerrain(float elevation, int width, int depth);
	~SimpleFlatTerrain();

	float getValue(int x, int y);
	void _setValue(int x, int y, float value);
	void _addValue(int x, int y, float change);

private:
	void _generateMeshVertices(TextureManager* textureManager);
};

} // namespace ucore