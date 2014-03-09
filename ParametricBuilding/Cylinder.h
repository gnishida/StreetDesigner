#pragma once

#include "../Core/Renderable.h"

namespace pb {

class Cylinder : public ucore::Renderable {
public:
	Cylinder(float orgX, float orgY, float orgZ, float baseRadius, float topRadius, float height, int slices, int stacks, ucore::Texture* texture);
	~Cylinder();
};

} // namespace pb