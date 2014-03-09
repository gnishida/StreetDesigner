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

#include "MyRoadGraphRenderer.h"
#include "../Core/Renderable.h"
#include "../Core/GeometryObject.h"
#include "../Core/Texture.h"



MyRoadGraphRenderer::MyRoadGraphRenderer() {
}

MyRoadGraphRenderer::~MyRoadGraphRenderer() {
}

void MyRoadGraphRenderer::_render(ucore::GeometryObject* object, ucore::TextureManager* textureManager) {
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		ucore::Renderable* renderable = object->getRenderable(i);
		ucore::Texture* texture = renderable->getTexture();

		if (texture != NULL) {
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
		} else {
			glBegin(renderable->getGlBeginMode());
			for (int j = 0; j < renderable->getNumMeshVertices(); ++j) {
				glColor3f(renderable->getMeshVertex(j).color[0], renderable->getMeshVertex(j).color[1], renderable->getMeshVertex(j).color[2]);
				glNormal3f(renderable->getMeshVertex(j).normal[0], renderable->getMeshVertex(j).normal[1], renderable->getMeshVertex(j).normal[2]);
				glVertex3f(renderable->getMeshVertex(j).location[0], renderable->getMeshVertex(j).location[1], renderable->getMeshVertex(j).location[2]);
			}
			glEnd();
		}
	}
}

