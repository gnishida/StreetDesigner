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

#include "SimpleFlatTerrain.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Renderable.h"
#include "RenderableQuad.h"

namespace ucore {

//Texture* SimpleFlatTerrain::texture = NULL;

SimpleFlatTerrain::SimpleFlatTerrain(float elevation, int width, int depth) : Terrain(width, depth) {
	this->elevation = elevation;

	setModified();
}

SimpleFlatTerrain::~SimpleFlatTerrain() {
}

float SimpleFlatTerrain::getValue(int x, int y) {
	return elevation;
}

void SimpleFlatTerrain::_setValue(int x, int y, float value) {
	// do nothing because this is a flat terrain.
}

void SimpleFlatTerrain::_addValue(int x, int y, float change) {
	// do nothing because this is a flat terrain.
}

void SimpleFlatTerrain::_generateMeshVertices(TextureManager* textureManager) {
	QVector3D p1(-width/2, -depth/2, elevation);
	QVector3D p2(width/2, -depth/2, elevation);
	QVector3D p3(width/2, depth/2, elevation);
	QVector3D p4(-width/2, depth/2, elevation);

	Texture* texture = textureManager->get("data/textures/grass.jpg");
	renderables.push_back(new RenderableQuad(p1, p2, p3, p4, QVector3D(0, 0, 1), texture, 0, 0, width / texture->getSizeX(), depth / texture->getSizeY()));
}

} // namespace ucore