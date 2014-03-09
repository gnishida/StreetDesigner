#include "BuildingPolygonFace.h"

namespace pb {

BuildingPolygonFace::BuildingPolygonFace(const QVector3D& pt, const ucore::Polygon3D& polygon, const QVector3D& normal, ucore::Texture* texture, float rotateZ) {
	this->pt = pt;
	this->glBeginMode = GL_POLYGON;
	this->polygon = polygon;
	this->normal = normal;
	this->texture = texture;
	this->rotateZ = rotateZ;

	setModified();
}

BuildingPolygonFace::~BuildingPolygonFace() {
}

void BuildingPolygonFace::initializeTextures(ucore::TextureManager* textureManager) {
	// Since the texture is loaded by Building class itself, this class does not load textures.
}

void BuildingPolygonFace::_generateMeshVertices() {
	for (int i = 0; i < polygon.size(); i++) {
		generateMeshVertex(polygon[i].x(), polygon[i].y(), polygon[i].z(), normal.x(), normal.y(), normal.z(), 0, 0);
	}
}

} // namespace pb