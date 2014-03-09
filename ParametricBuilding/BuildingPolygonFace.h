#pragma once

#include "../Core/Renderable.h"
#include "../Core/Polygon3D.h"

namespace pb {

class BuildingPolygonFace : public ucore::Renderable {
private:
	ucore::Polygon3D polygon;
	QVector3D normal;

public:
	BuildingPolygonFace(const QVector3D& pt, const ucore::Polygon3D& polygon, const QVector3D& normal, ucore::Texture* texture, float rotateZ);
	~BuildingPolygonFace();

	void initializeTextures(ucore::TextureManager* textureManager);
	void _generateMeshVertices();
};

} // namespace pb