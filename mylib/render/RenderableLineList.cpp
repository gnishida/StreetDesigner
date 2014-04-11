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

#include "RenderableLineList.h"

namespace mylib {

RenderableLineList::RenderableLineList() : Renderable(GL_LINES, 3) {
}

RenderableLineList::RenderableLineList(Texture* texture) : Renderable(GL_LINES, 3) {
	this->texture = texture;
}

void RenderableLineList::addLine(const Polyline3D &polyline, const QVector3D &normal, const QColor& color) {
	for (int i = 0; i < polyline.size() - 1; ++i) {
		generateMeshVertex(polyline[i].x(), polyline[i].y(), polyline[i].z(), normal.x(), normal.y(), normal.z(), color);
		generateMeshVertex(polyline[i + 1].x(), polyline[i + 1].y(), polyline[i + 1].z(), normal.x(), normal.y(), normal.z(), color);
	}
}

void RenderableLineList::addLine(const Polygon3D &polygon, const QVector3D &normal, const QColor& color) {
	for (int i = 0; i < polygon.size(); ++i) {
		int next = (i + 1) % polygon.size();
		generateMeshVertex(polygon[i].x(), polygon[i].y(), polygon[i].z(), normal.x(), normal.y(), normal.z(), color);
		generateMeshVertex(polygon[next].x(), polygon[next].y(), polygon[next].z(), normal.x(), normal.y(), normal.z(), color);
	}
}

} // namespace mylib