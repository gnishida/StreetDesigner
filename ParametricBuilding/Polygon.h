#pragma once

#include "../Core/Renderable.h"
#include "../Core/Polygon3D.h"

namespace pb {

class Polygon : public ucore::Renderable {
public:
	Polygon(const ucore::Polygon3D& polygon, const QVector3D& normal, ucore::Texture* texture);
	~Polygon();
};

} // namespace pb