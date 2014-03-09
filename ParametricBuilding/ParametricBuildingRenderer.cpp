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

#include "ParametricBuildingRenderer.h"
#include "../Core/Texture.h"
#include "../Core/Renderable.h"
#include "../Core/GeometryObject.h"
#include "GLU.h"
#include "qmath.h"

namespace pb {

void __stdcall vertexCallback(void* vertex) {
	GLdouble *ptr;

	ptr = (GLdouble *) vertex;
	glTexCoord2dv((GLdouble *) ptr + 3);
	glVertex3dv((GLdouble *) ptr);

}

ParametricBuildingRenderer::ParametricBuildingRenderer() {
}

ParametricBuildingRenderer::~ParametricBuildingRenderer() {
}

void ParametricBuildingRenderer::_render(ucore::GeometryObject* object, ucore::TextureManager* textureManager) {
	glPushMatrix();

	glTranslatef(object->getPt().x(), object->getPt().y(), object->getPt().z());
	glRotatef(object->getRotateZ() * 180 / M_PI, 0, 0, 1);
	glScalef(object->getScale(), object->getScale(), object->getScale());

	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		ucore::Renderable* renderable = object->getRenderable(i);

		glBindTexture(GL_TEXTURE_2D, renderable->getTexture()->getId());

		glBegin(renderable->getGlBeginMode());
		for (int j = 0; j < renderable->getNumMeshVertices(); ++j) {
			glNormal3f(renderable->getMeshVertex(j).normal[0], renderable->getMeshVertex(j).normal[1], renderable->getMeshVertex(j).normal[2]);
			glTexCoord2f(renderable->getMeshVertex(j).tex[0], renderable->getMeshVertex(j).tex[1]);
			glVertex3f(renderable->getMeshVertex(j).location[0], renderable->getMeshVertex(j).location[1], renderable->getMeshVertex(j).location[2]);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

} // namespace pb