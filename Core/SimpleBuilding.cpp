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

#include "SimpleBuilding.h"
#include "Terrain.h"
#include "Renderable.h"
#include "RenderableQuadList.h"
#include <limits>

namespace ucore {

SimpleBuilding::SimpleBuilding(const Loop3D& footprint, int numStories, float height) : Building(footprint, numStories, height) {
	int colorR = this->color.red() + qrand() % 30 + 10;

	roofColor = QColor(colorR, colorR, colorR);

	setModified();
}

/*SimpleBuilding::SimpleBuilding(const QVector3D& pt, const QVector3D axis, float width, float depth, int numStories, float height) : Building(pt, axis, width, depth, numStories, height) {
	int colorR = this->color.red() + qrand() % 30 + 10;

	roofColor = QColor(colorR, colorR, colorR);

	setModified();
}*/

SimpleBuilding::~SimpleBuilding() {
}

void SimpleBuilding::_generateMeshVertices(TextureManager* textureManager) {
	renderables.push_back(new Renderable());

	std::vector<QVector3D> box;
	box.push_back(QVector3D(0, 0, 0));
	box.push_back(QVector3D(width, 0, 0));
	box.push_back(QVector3D(width, depth, 0));
	box.push_back(QVector3D(0, depth, 0));
	box.push_back(QVector3D(0, 0, height));
	box.push_back(QVector3D(width, 0, height));
	box.push_back(QVector3D(width, depth, height));
	box.push_back(QVector3D(0, depth, height));

	RenderableQuadList* renderable = new RenderableQuadList();
	renderable->addQuad(box[1], box[2], box[6], box[5], QVector3D(1, 0, 0), color);
	renderable->addQuad(box[3], box[0], box[4], box[7], QVector3D(-1, 0, 0), color);
	renderable->addQuad(box[2], box[3], box[7], box[6], QVector3D(0, 1, 0), color);
	renderable->addQuad(box[0], box[1], box[5], box[4], QVector3D(0, -1, 0), color);
	renderable->addQuad(box[4], box[5], box[6], box[7], QVector3D(0, 0, 1), roofColor);

	renderables.push_back(renderable);
}

} // namespace ucore