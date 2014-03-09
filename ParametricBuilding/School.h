/*********************************************************************************
 *
 *		MTC Project - Geometry Project - SC Renderer Class
 *
 *
 *		@desc Class containing the renderer for office buildings
 *		@author igarciad
 *
 *********************************************************************************/
 
#pragma once

#include "AbstractParametricBuilding.h"
#include "../Core/TextureManager.h"
#include "../Core/Texture.h"

namespace pb {
	class School : public AbstractParametricBuilding {
	private:
		static bool texturesLoaded;
		static std::vector<QString> textureNames;
		static std::vector<ucore::Texture*> textures;
		static const int numSubType;

		int roofTex;
		int roofTex2;
		int subType;

	public:
		School(const ucore::Loop3D& footprint, int numStories, float height);
		~School();

	private:
		void _generateMeshVertices(ucore::TextureManager* textureManager);
		void _generateMeshVerticesForType0(ucore::TextureManager* textureManager);
		void _generateMeshVerticesForType1(ucore::TextureManager* textureManager);
	};
}
