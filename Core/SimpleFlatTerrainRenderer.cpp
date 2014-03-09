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

//#include <GL/glew.h>
#include "SimpleFlatTerrainRenderer.h"
#include "SimpleFlatTerrain.h"
#include "Terrain.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Renderable.h"

namespace ucore {

SimpleFlatTerrainRenderer::SimpleFlatTerrainRenderer() {
	/*
	vertex = glGetAttribLocation(programId,"position");
    color = glGetAttribLocation(programId, "color");
	tex = glGetAttribLocation(programId, "tex");
	*/
}


SimpleFlatTerrainRenderer::~SimpleFlatTerrainRenderer() {
}

void SimpleFlatTerrainRenderer::_render(GeometryObject* object, TextureManager* textureManager) {
	SimpleFlatTerrain* terrain = dynamic_cast<SimpleFlatTerrain*>(object);

	for (int i = 0; i < object->getNumRenderables(); ++i) {
		Renderable* renderable = object->getRenderable(i);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, renderable->getTexture()->getId());

		glBegin(renderable->getGlBeginMode());
		for (int j = 0; j < renderable->getNumMeshVertices(); ++j) {
			glNormal3f(renderable->getMeshVertex(j).normal[0], renderable->getMeshVertex(j).normal[1], renderable->getMeshVertex(j).normal[2]);
			glTexCoord2f(renderable->getMeshVertex(j).tex[0], renderable->getMeshVertex(j).tex[1]);
			glVertex3f(renderable->getMeshVertex(j).location[0], renderable->getMeshVertex(j).location[1], renderable->getMeshVertex(j).location[2]);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
}

} // namespace ucore