#pragma once

#include "../Core/Renderable.h"

namespace pb {

class Triangle : public ucore::Renderable {
public:
	Triangle(std::vector<QVector3D>& pts, const QVector3D& normal, ucore::Texture* texture, float s0, float t0, float s1, float t1, float s2, float t2);
	~Triangle();
};

} // namespace pb