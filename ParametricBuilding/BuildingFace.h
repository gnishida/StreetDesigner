#pragma once

#include "../Core/Renderable.h"
#include "../Core/Texture.h"
#include <qvector3d.h>

namespace pb {

class BuildingFace : public ucore::Renderable {
private:
	QVector3D pt1;
	QVector3D pt2;
	QVector3D pt3;
	QVector3D pt4;
	QVector3D normal;
	float s0;
	float s1;
	float t0;
	float t1;

public:
	BuildingFace(const QVector3D& pt, QVector3D* pt1, QVector3D* pt2, QVector3D* pt3, QVector3D* pt4, QVector3D* normal, ucore::Texture* texture, float s0, float s1, float t0, float t1, float rotateZ);
	~BuildingFace();

	void initializeTextures(ucore::TextureManager* textureManager);
	void _generateMeshVertices();
};

} // namespace pb