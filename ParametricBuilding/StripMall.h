/*********************************************************************************
 *
 *		MTC Project - Geometry Project - RS Renderer Class
 *
 *
 *		@desc Class containing the renderer for RS buildings
 *		@author igarciad
 *
 *********************************************************************************/

#pragma once

#include "AbstractParametricBuilding.h"

namespace pb {
	class StripMall : public AbstractParametricBuilding {
	private:
		static bool texturesLoaded;
		static std::vector<QString> textureNames;
		static std::vector<ucore::Texture*> textures;

		int roofTex;
		int subType;

	public:
		StripMall(const ucore::Loop3D& footprint, int numStories, float height);
		~StripMall();

	private:
		void _generateMeshVertices(ucore::TextureManager* textureManager);	
	};
}

