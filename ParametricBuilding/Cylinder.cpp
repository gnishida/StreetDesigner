#include "Cylinder.h"

namespace pb {

Cylinder::Cylinder(float orgX, float orgY, float orgZ, float baseRadius, float topRadius, float height, int slices, int stacks, ucore::Texture* texture) {
	this->glBeginMode = GL_QUADS;
	this->texture = texture;

	for (int i = 0; i < slices; i++) {
		float angle1 = 2.0 * M_PI * i / slices;
		float angle2 = 2.0 * M_PI * (i + 1) / slices;

		for (int j = 0; j < stacks; j++) {
			float z1 = height * j / stacks;
			float z2 = height * (j + 1) / stacks;
			float radius1 = baseRadius + (float)(topRadius - baseRadius) * j / stacks;
			float radius2 = baseRadius + (float)(topRadius - baseRadius) * (j + 1) / stacks;

			generateMeshVertex(orgX + radius1 * cos(angle1), orgY + radius1 * sin(angle1), orgZ + z1, cos(angle1), sin(angle1), 0, (float)i / slices, (float)j / stacks);
			generateMeshVertex(orgX + radius1 * cos(angle2), orgY + radius1 * sin(angle2), orgZ + z1, cos(angle2), sin(angle2), 0, (float)(i+1) / slices, (float)j / stacks);
			generateMeshVertex(orgX + radius2 * cos(angle2), orgY + radius2 * sin(angle2), orgZ + z2, cos(angle2), sin(angle2), 0, (float)(i+1) / slices, (float)(j+1) / stacks);
			generateMeshVertex(orgX + radius2 * cos(angle1), orgY + radius2 * sin(angle1), orgZ + z2, cos(angle1), sin(angle1), 0, (float)i / slices, (float)(j+1) / stacks);
		}
	}
}

Cylinder::~Cylinder() {
}

} // namespace pb