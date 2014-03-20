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

#include "Renderer.h"
#include "GeometryObject.h"
#include "Renderable.h"
#include "Texture.h"

namespace mylib {

Renderer::Renderer() {
}

Renderer::~Renderer() {
}

void Renderer::render(GeometryObject* object, TextureManager* textureManager) {
	if (!object->getModified()) {
		glCallList(object->getDispList());
	} else {
		object->generateMeshVertices(textureManager);

		object->newDispList();

		_render(object, textureManager);

		glEndList();
	}
}

void Renderer::_render(GeometryObject* object, TextureManager* textureManager) {
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		mylib::RenderablePtr renderable = object->getRenderable(i);
		mylib::Texture* texture = renderable->getTexture();

		if (texture != NULL) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, renderable->getTexture()->getId());

			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	

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


	/*
	for (int i = 0; i < object->getNumRenderables(); ++i) {
		glBegin(object->getRenderable(i)->getGlBeginMode());
		for (int j = 0; j < object->getRenderable(i)->getNumMeshVertices(); ++j) {
			glColor3f(object->getRenderable(i)->getMeshVertex(j).color[0], object->getRenderable(i)->getMeshVertex(j).color[1], object->getRenderable(i)->getMeshVertex(j).color[2]);
			glNormal3f(object->getRenderable(i)->getMeshVertex(j).normal[0], object->getRenderable(i)->getMeshVertex(j).normal[1], object->getRenderable(i)->getMeshVertex(j).normal[2]);
			glVertex3f(object->getRenderable(i)->getMeshVertex(j).location[0], object->getRenderable(i)->getMeshVertex(j).location[1], object->getRenderable(i)->getMeshVertex(j).location[2]);
		}
		glEnd();
	}
	*/
}

} // namespace mylib