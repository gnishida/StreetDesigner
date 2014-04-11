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

#include <QVector3D>
#include "Renderable.h"
#include "../common/Polyline3D.h"
#include "../common/Polygon3D.h"

namespace mylib {

class RenderableLineList : public Renderable {
public:
	RenderableLineList();
	RenderableLineList(Texture* texture);
	~RenderableLineList() {}

	void addLine(const Polyline3D &polyline, const QVector3D &normal, const QColor& color);
	void addLine(const Polygon3D &polygon, const QVector3D &normal, const QColor& color);
};

} // namespace mylib