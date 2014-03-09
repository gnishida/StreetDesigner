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

#include "RenderableCylinder.h"
#include "qmath.h"

namespace ucore {

RenderableCylinder::RenderableCylinder(float x, float y, float z, float baseRadius, float topRadius, float height, int slices, int stacks, ucore::Texture* texture) : Renderable(GL_QUADS) {
	this->texture = texture;

	for (int i = 0; i < slices; i++) {
		float angle1 = 2.0 * M_PI * i / slices;
		float angle2 = 2.0 * M_PI * (i + 1) / slices;

		for (int j = 0; j < stacks; j++) {
			float z1 = height * j / stacks;
			float z2 = height * (j + 1) / stacks;
			float radius1 = baseRadius + (float)(topRadius - baseRadius) * j / stacks;
			float radius2 = baseRadius + (float)(topRadius - baseRadius) * (j + 1) / stacks;

			generateMeshVertex(x + radius1 * cos(angle1), y + radius1 * sin(angle1), z + z1, cos(angle1), sin(angle1), 0, (float)i / slices, (float)j / stacks);
			generateMeshVertex(x + radius1 * cos(angle2), y + radius1 * sin(angle2), z + z1, cos(angle2), sin(angle2), 0, (float)(i+1) / slices, (float)j / stacks);
			generateMeshVertex(x + radius2 * cos(angle2), y + radius2 * sin(angle2), z + z2, cos(angle2), sin(angle2), 0, (float)(i+1) / slices, (float)(j+1) / stacks);
			generateMeshVertex(x + radius2 * cos(angle1), y + radius2 * sin(angle1), z + z2, cos(angle1), sin(angle1), 0, (float)i / slices, (float)(j+1) / stacks);
		}
	}
}

RenderableCylinder::~RenderableCylinder() {
}

} // namespace ucore