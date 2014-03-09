#pragma once

#include "../Core/Renderable.h"

namespace pb {

class Quad : public ucore::Renderable {
public:
	Quad(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& pt4, const QVector3D& normal, ucore::Texture* texture, float s0, float t0, float s1, float t1);
	~Quad();
};

} // namespace pb