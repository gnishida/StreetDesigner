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

#include "RenderableTriangle.h"

namespace ucore {

RenderableTriangle::RenderableTriangle(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& normal, ucore::Texture* texture, float s0, float t0, float s1, float t1, float s2, float t2) {
	this->texture = texture;

	generateMeshVertex(pt1.x(), pt1.y(), pt1.z(), normal.x(), normal.y(), normal.z(), s0, t0);
	generateMeshVertex(pt2.x(), pt2.y(), pt2.z(), normal.x(), normal.y(), normal.z(), s1, t1);
	generateMeshVertex(pt3.x(), pt3.y(), pt3.z(), normal.x(), normal.y(), normal.z(), s2, t2);
}

RenderableTriangle::~RenderableTriangle() {
}

} // namespace ucore