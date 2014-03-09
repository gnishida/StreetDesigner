#include "BuildingFace.h"

namespace pb {

BuildingFace::BuildingFace(const QVector3D& pt, QVector3D* pt1, QVector3D* pt2, QVector3D* pt3, QVector3D* pt4, QVector3D* normal, ucore::Texture* texture, float s0, float s1, float t0, float t1, float rotateZ) {
	this->pt = pt;
	this->pt1 = *pt1;
	this->pt2 = *pt2;
	this->pt3 = *pt3;
	this->pt4 = *pt4;
	this->normal = *normal;
	this->s0 = s0;
	this->s1 = s1;
	this->t0 = t0;
	this->t1 = t1;
	this->texture = texture;
	this->rotateZ = rotateZ;

	setModified();
}

BuildingFace::~BuildingFace() {
}

void BuildingFace::initializeTextures(ucore::TextureManager* textureManager) {
	// Since the texture is loaded by Building class itself, this class does not load textures.
}

void BuildingFace::_generateMeshVertices() {
	generateMeshVertex(pt1.x(), pt1.y(), pt1.z(), normal.x(), normal.y(), normal.z(), s0, t0);
	generateMeshVertex(pt2.x(), pt2.y(), pt2.z(), normal.x(), normal.y(), normal.z(), s1, t0);
	generateMeshVertex(pt3.x(), pt3.y(), pt3.z(), normal.x(), normal.y(), normal.z(), s1, t1);

	generateMeshVertex(pt1.x(), pt1.y(), pt1.z(), normal.x(), normal.y(), normal.z(), s0, t0);
	generateMeshVertex(pt3.x(), pt3.y(), pt3.z(), normal.x(), normal.y(), normal.z(), s1, t1);
	generateMeshVertex(pt4.x(), pt4.y(), pt4.z(), normal.x(), normal.y(), normal.z(), s0, t1);
}

} // namespace pb