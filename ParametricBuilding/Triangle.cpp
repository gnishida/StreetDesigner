#include "Triangle.h"

namespace pb {

Triangle::Triangle(std::vector<QVector3D>& pts, const QVector3D& normal, ucore::Texture* texture, float s0, float t0, float s1, float t1, float s2, float t2) {
	this->glBeginMode = GL_TRIANGLES;
	this->texture = texture;

	generateMeshVertex(pts[0].x(), pts[0].y(), pts[0].z(), normal.x(), normal.y(), normal.z(), s0, t0);
	generateMeshVertex(pts[1].x(), pts[1].y(), pts[1].z(), normal.x(), normal.y(), normal.z(), s1, t1);
	generateMeshVertex(pts[2].x(), pts[2].y(), pts[2].z(), normal.x(), normal.y(), normal.z(), s2, t2);
}

Triangle::~Triangle() {
}


} // namespace pb