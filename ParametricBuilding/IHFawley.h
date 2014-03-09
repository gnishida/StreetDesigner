/*********************************************************************************
 *
 *		MTC Project - Geometry Project - IH Renderer Class
 *
 *
 *		@desc Class containing the renderer for IH buildings
 *		@author igarciad
 *
 *********************************************************************************/
 
#pragma once

#include "AbstractParametricBuilding.h"
#include "../Core/TextureManager.h"
#include "../Core/Texture.h"

namespace pb {
	class IHFawley : public AbstractParametricBuilding {
	private:
		static bool texturesLoaded;
		static std::vector<QString> textureNames;
		static std::vector<ucore::Texture*> textures;
		//static GLUquadricObj* cylinder;
		int roofTex;
		int numBlockX;
		int numBlockY;
		int subType;
		int backTex;

	public:
		IHFawley(const ucore::Loop3D& footprint, int numStories, float height);
		~IHFawley();

	private:
		void initializeTextures(ucore::TextureManager* textureManager);
		void _generateMeshVertices();
	};
}

