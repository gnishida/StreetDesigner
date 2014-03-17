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

#include <limits>
#include <boost/polygon/polygon.hpp>
#include "../common/Util.h"
#include "RenderableConcave.h"

namespace mylib {
	
RenderableConcave::RenderableConcave(const Polygon3D& polygon, const QColor& color) : Renderable(GL_TRIANGLES) {
	tessellate(polygon, color);
}

RenderableConcave::RenderableConcave(const Polygon3D& polygon, Texture* texture) : Renderable(GL_TRIANGLES) {
	this->texture = texture;

	tessellate(polygon);
}

RenderableConcave::~RenderableConcave() {
}

void RenderableConcave::tessellate(const Polygon3D& polygon) {
	if (polygon.size() < 3) return;

	float minX = (std::numeric_limits<float>::max)();
	float maxX = (std::numeric_limits<float>::min)();
	float minY = (std::numeric_limits<float>::max)();
	float maxY = (std::numeric_limits<float>::min)();

	for (int i = 0; i < polygon.size(); i++) {
		minX = std::min<float>(polygon[i].x(), minX);
		maxX = std::max<float>(polygon[i].x(), maxX);
		minY = std::min<float>(polygon[i].y(), minY);
		maxY = std::max<float>(polygon[i].y(), maxY);
	}

	std::vector<Polygon3D> trapezoids = polygon.tessellate();

	for (int i = 0; i < trapezoids.size(); ++i) {
		if (trapezoids[i].size() < 3) continue;

		QVector2D tex0((trapezoids[i][0].x() - minX) / (maxX - minX), (trapezoids[i][0].y() - minY) / (maxY - minY));
		for (int j = 1; j < trapezoids[i].size() - 1; ++j) {
			generateMeshVertex(trapezoids[i][0].x(), trapezoids[i][0].y(), trapezoids[i][0].z(), 0, 0, 1, tex0.x(), tex0.y());
			generateMeshVertex(trapezoids[i][j].x(), trapezoids[i][j].y(), trapezoids[i][j].z(), 0, 0, 1, (trapezoids[i][j].x() - minX) / (maxX - minX), (trapezoids[i][j].y() - minY) / (maxY - minY));
			generateMeshVertex(trapezoids[i][j+1].x(), trapezoids[i][j+1].y(), trapezoids[i][j+1].z(), 0, 0, 1, (trapezoids[i][j+1].x() - minX) / (maxX - minX), (trapezoids[i][j+1].y() - minY) / (maxY - minY));
		}
	}
}

void RenderableConcave::tessellate(const Polygon3D& polygon, const QColor& color) {
	if (polygon.size() < 3) return;

	std::vector<Polygon3D> trapezoids = polygon.tessellate();

	for (int i = 0; i < trapezoids.size(); ++i) {
		if (trapezoids[i].size() < 3) continue;

		QVector3D n = Util::calculateNormal(trapezoids[i][0], trapezoids[i][1], trapezoids[i][2]);

		for (int j = 1; j < trapezoids[i].size() - 1; ++j) {
			generateMeshVertex(trapezoids[i][0].x(), trapezoids[i][0].y(), trapezoids[i][0].z(), 0, 0, 1, color);
			generateMeshVertex(trapezoids[i][j].x(), trapezoids[i][j].y(), trapezoids[i][j].z(), 0, 0, 1, color);
			generateMeshVertex(trapezoids[i][j+1].x(), trapezoids[i][j+1].y(), trapezoids[i][j+1].z(), 0, 0, 1, color);
		}
	}
}

} // namespace mylib