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

#pragma once

#include "Renderable.h"
#include "qvector3d.h"

namespace ucore {

class RenderableTriangleList : public Renderable {
public:
	RenderableTriangleList();
	RenderableTriangleList(Texture* texture);
	~RenderableTriangleList();

	void addTriangle(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& normal, float s0, float t0, float s1, float t1, float s2, float t2);
	void addTriangle(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& normal, const QColor& color);
	void addTriangle(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& normal, const QColor& color1, const QColor& color2, const QColor& color3);
};

} // namespace ucore