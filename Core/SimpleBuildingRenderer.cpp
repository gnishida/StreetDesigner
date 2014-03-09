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

#include "SimpleBuildingRenderer.h"
#include "SimpleBuilding.h"
#include "Renderable.h"
#include "GeometryObject.h"
#include "qmath.h"

namespace ucore {

SimpleBuildingRenderer::SimpleBuildingRenderer() {
}


SimpleBuildingRenderer::~SimpleBuildingRenderer() {
}

void SimpleBuildingRenderer::_render(GeometryObject* object, TextureManager* textureManager) {
	glPushMatrix();

	glTranslatef(object->getPt().x(), object->getPt().y(), object->getPt().z());
	glRotatef(object->getRotateZ() * 180 / M_PI, 0, 0, 1);
	glScalef(object->getScale(), object->getScale(), object->getScale());

	for (int i = 0; i < object->getNumRenderables(); ++i) {
		glBegin(object->getRenderable(i)->getGlBeginMode());
		for (int j = 0; j < object->getRenderable(i)->getNumMeshVertices(); ++j) {
			glColor3f(object->getRenderable(i)->getMeshVertex(j).color[0], object->getRenderable(i)->getMeshVertex(j).color[1], object->getRenderable(i)->getMeshVertex(j).color[2]);
			glNormal3f(object->getRenderable(i)->getMeshVertex(j).normal[0], object->getRenderable(i)->getMeshVertex(j).normal[1], object->getRenderable(i)->getMeshVertex(j).normal[2]);
			glVertex3f(object->getRenderable(i)->getMeshVertex(j).location[0], object->getRenderable(i)->getMeshVertex(j).location[1], object->getRenderable(i)->getMeshVertex(j).location[2]);
		}
		glEnd();
	}

	glPopMatrix();
}

} // namespace ucore