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

#include "RenderableCircle.h"
#include "qmath.h"

namespace ucore {

RenderableCircle::RenderableCircle(float x, float y, float z, float radius, int slices, ucore::Texture* texture) : Renderable(GL_TRIANGLES) {
	this->texture = texture;

	for (int i = 0; i < slices; i++) {
		float angle1 = 2.0 * M_PI * i / slices;
		float angle2 = 2.0 * M_PI * (i + 1) / slices;

		generateMeshVertex(x, y, z, 0, 0, 1, 0.5, 0.5);
		generateMeshVertex(x + radius * cos(angle1), y + radius * sin(angle1), z, 0, 0, 1, cos(angle1) / 2 + 0.5, sin(angle1) / 2 + 0.5);
		generateMeshVertex(x + radius * cos(angle2), y + radius * sin(angle2), z, 0, 0, 1, cos(angle2) / 2 + 0.5, sin(angle2) / 2 + 0.5);
	}
}

RenderableCircle::~RenderableCircle() {
}

} // namespace ucore