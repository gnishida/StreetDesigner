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

#include "BlockRenderer.h"
#include "Block.h"
#include "GeometryObject.h"
#include "Renderable.h"

namespace ucore {

BlockRenderer::BlockRenderer() {
}

BlockRenderer::~BlockRenderer() {
}

void BlockRenderer::_render(GeometryObject* object, TextureManager* textureManager) {
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		glBegin(object->getRenderable(i)->getGlBeginMode());
		for (int j = 0; j < object->getRenderable(i)->getNumMeshVertices(); ++j) {
			glColor3f(object->getRenderable(i)->getMeshVertex(j).color[0], object->getRenderable(i)->getMeshVertex(j).color[1], object->getRenderable(i)->getMeshVertex(j).color[2]);
			glNormal3f(object->getRenderable(i)->getMeshVertex(j).normal[0], object->getRenderable(i)->getMeshVertex(j).normal[1], object->getRenderable(i)->getMeshVertex(j).normal[2]);
			glVertex3f(object->getRenderable(i)->getMeshVertex(j).location[0], object->getRenderable(i)->getMeshVertex(j).location[1], object->getRenderable(i)->getMeshVertex(j).location[2]);
		}
		glEnd();
	}
}

} // namespace ucore