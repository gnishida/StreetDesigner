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

#include "RenderableCircleList.h"

namespace mylib {

RenderableCircleList::RenderableCircleList() : Renderable(GL_TRIANGLES) {
}

RenderableCircleList::RenderableCircleList(Texture* texture) : Renderable(GL_TRIANGLES) {
	this->texture = texture;
}

void RenderableCircleList::addCircle(const QVector3D& o, float r, int slices, const QColor& color) {
	for (int i = 0; i < slices; i++) {
		float angle1 = 2.0 * M_PI * i / slices;
		float angle2 = 2.0 * M_PI * (i + 1) / slices;

		generateMeshVertex(o.x(), o.y(), o.z(), 0, 0, 1, color);
		generateMeshVertex(o.x() + r * cos(angle1), o.y() + r * sin(angle1), o.z(), 0, 0, 1, color);
		generateMeshVertex(o.x() + r * cos(angle2), o.y() + r * sin(angle2), o.z(), 0, 0, 1, color);
	}
}

} // namespace mylib