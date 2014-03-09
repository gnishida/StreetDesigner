/*********************************************************************************
 *
 *		MTC Project - Geometry Project - OfficeRenderer Class
 *
 *
 *		@desc Class containing the renderer for office buildings
 *		@author Ignacio Garcia Dorado
 *
 *********************************************************************************/
 
#pragma once

#include "AbstractParametricBuilding.h"
#include "../Core/Texture.h"

namespace pb {
	class Office : public AbstractParametricBuilding {
	private:
		static bool texturesLoaded;	
		static std::vector<QString> textureNames;
		static std::vector<ucore::Texture*> textures;
		int roofTex;
		int subType;

	public:
		Office(const ucore::Loop3D& footprint, int numStories, float height);
		~Office();

		//void render();

	private:
		void initializeTextures(ucore::TextureManager* textureManager);
		void _generateMeshVertices();
	};
}

