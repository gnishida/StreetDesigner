#include "Quad.h"

namespace pb {

Quad::Quad(const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& pt4, const QVector3D& normal, ucore::Texture* texture, float s0, float t0, float s1, float t1) {
	this->glBeginMode = GL_QUADS;
	this->texture = texture;

	generateMeshVertex(pt1.x(), pt1.y(), pt1.z(), normal.x(), normal.y(), normal.z(), s0, t0);
	generateMeshVertex(pt2.x(), pt2.y(), pt2.z(), normal.x(), normal.y(), normal.z(), s1, t0);
	generateMeshVertex(pt3.x(), pt3.y(), pt3.z(), normal.x(), normal.y(), normal.z(), s1, t1);
	generateMeshVertex(pt4.x(), pt4.y(), pt4.z(), normal.x(), normal.y(), normal.z(), s1, t1);
}

Quad::~Quad() {
}

} // namespace pb