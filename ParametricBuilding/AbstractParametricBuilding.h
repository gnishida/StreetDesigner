/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#pragma once

#include "../Core/Building.h"
#include "../Core/Texture.h"
#include "../Core/TextureManager.h"
#include "../Core/Renderable.h"
#include "../Core/RenderableQuadList.h"

namespace pb {

class AbstractParametricBuilding : public ucore::Building {
protected:
	bool envelopeCalculated;
	std::vector<QVector3D> bb;
	std::vector<QVector3D> normals;

	// ROOF TEXTURES
	static bool roofTexturesLoaded;
	static std::vector<ucore::Texture*> roofTextures;
	static std::vector<ucore::Texture*> houseRoofTextures;
	static std::vector<QVector2D> roofTexCoord;
	static std::vector<QVector2D> houseRoofTexCoord;

public:
	AbstractParametricBuilding(const ucore::Loop3D& footprint, int numStories, float height);
	~AbstractParametricBuilding();
	
	//static void initializeTextures(std::vector<QString>* listTextures, std::vector<ucore::Texture*>* textures, ucore::TextureManager* textureManager);
	static void loadRoofTextures(ucore::TextureManager* textureManager);
	int bestTextureForRoof(float x,float y);
	//void renderFace(int face, float orgX, float orgY, float orgZ, float width, float depth, float height, ucore::Texture* texture, float s0, float s1, float t0, float t1);
	void renderFace(ucore::Renderable* renderable, int face, float orgX, float orgY, float orgZ, float width, float depth, float height, std::vector<QVector3D>* normals, float s0, float s1, float t0, float t1);
	void renderFace(ucore::Renderable* renderable, int face, float orgX, float orgY, float orgZ, float width, float depth, float height, std::vector<QVector3D>* normals, float s1, float t1);
	void renderFlatRoof(ucore::Polygon3D& p, ucore::Texture* texture);
	void renderCylinder(float orgX, float orgY, float orgZ, float cbase,float ctop, float cheight, int cslices,int cstacks, ucore::Texture* texture1, ucore::Texture* texture2);
	//void drawTriangle(std::vector<QVector3D>& v,QVector3D& norm, ucore::Texture* texture, float s0, float t0, float s1, float t1, float s2, float t2);
	void drawRoofTriangle(ucore::Renderable* r, ucore::Renderable* r2, float orgX, float orgY, float orgZ, float dx,float dy,float dz,std::vector<QVector3D>* normals, float yTexShift, bool turn90, bool half, float s0 = 1.0f);

private:
	int setEnvelope();
};

} // namespace pb