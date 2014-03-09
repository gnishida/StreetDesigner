/*********************************************************************************
*
*		MTC Project - Geometry Project - SC Class
*
*
*		@desc Class containing the renderer for SC buildings
*		@author Ignacio Garcia Dorado
*
*********************************************************************************/

#include "School.h"
#include "../Core/Renderable.h"
#include "../Core/RenderableTriangleList.h"
#include "../Core/RenderableConcave.h"
#include <qvector3d.h>

namespace pb {
	const int School::numSubType = 2;

	std::vector<ucore::Texture*> School::textures;
	bool School::texturesLoaded = false;
	std::vector<QString> School::textureNames;

	School::School(const ucore::Loop3D& footprint, int numStories, float height) : AbstractParametricBuilding(footprint, numStories, height) {
		if (textureNames.size() == 0) {
			textureNames.push_back("data/buildings/SC/0_back0.jpg");//0
			textureNames.push_back("data/buildings/SC/0_back1.jpg");//1
			textureNames.push_back("data/buildings/SC/0_front.jpg");//2
			textureNames.push_back("data/buildings/SC/0_frontSides0.jpg");//3
			textureNames.push_back("data/buildings/SC/0_frontSides1.jpg");//4
			textureNames.push_back("data/buildings/SC/0_left0.jpg");//5
			textureNames.push_back("data/buildings/SC/0_left1.jpg");//6
			textureNames.push_back("data/buildings/SC/0_left2.jpg");//7
			textureNames.push_back("data/buildings/SC/0_left3.jpg");//8
			textureNames.push_back("data/buildings/SC/0_right0.jpg");//9
			textureNames.push_back("data/buildings/SC/0_right1.jpg");//10
			textureNames.push_back("data/buildings/SC/0_center0.jpg");//11
			textureNames.push_back("data/buildings/SC/1_front.jpg");//0
			textureNames.push_back("data/buildings/SC/1_frontBack.jpg");//1
			textureNames.push_back("data/buildings/SC/1_back.jpg");//2 
			textureNames.push_back("data/buildings/SC/1_inside.jpg");//3
			textureNames.push_back("data/buildings/SC/1_sides.jpg");//4
			textureNames.push_back("data/buildings/SC/1_frontFarSides.jpg");//5
			textureNames.push_back("data/buildings/SC/1_sidesWall.jpg");//6
		}

		roofTex=-1;
		roofTex2=-1;
		subType=-1;

		float subMins[numSubType] ={17.0f, 0.0f}; // fill with subtype minimum widths

		while (subType==-1) {	
			subType = rand() % numSubType;
			if (width < subMins[subType] || depth < subMins[subType]) subType = -1;
		}

		//scale logic
		float minDim = 10.2f;
		if (width < minDim || depth < minDim) {
			scale = min(width, depth) / minDim;
			width /= scale;
			depth /= scale;
		}
	}

	/*********************************************************************************
	*	SCRenderer destructor
	*		Destructor for SC building renderers.  Nothing to do.
	*********************************************************************************/
	School::~School() {
	}

	void School::_generateMeshVertices(ucore::TextureManager* textureManager) {
		AbstractParametricBuilding::loadRoofTextures(textureManager);

		for (int i = 0; i < textureNames.size(); i++) {
			renderables.push_back(new ucore::RenderableTriangleList(textureManager->get(textureNames[i])));
		}
		for (int i = 0; i < roofTextures.size(); i++) {
			renderables.push_back(new ucore::RenderableTriangleList(roofTextures[i]));
		}

		if (subType == 0) {
			_generateMeshVerticesForType0(textureManager);
		} else if (subType == 1) {
			_generateMeshVerticesForType1(textureManager);
		}
	}

	void School::_generateMeshVerticesForType0(ucore::TextureManager* textureManager) {
		float dx = width;
		float dy = height;

		int texShift=12;
		float frontHeigh=4.47f;
		float frontSideDepth=6.0f;
		float colDepth=0.4f;
		float shiftX=0;
		float frontWidth=frontHeigh*679.0f/200;
		float farSide=1.0f;

		// 1. LEFT
		dx-=frontWidth;
		dx-=farSide*2;//far sides
		dx/=2.0f;//left and right
		renderFace(renderables[texShift+5], 3,shiftX,frontSideDepth,0,farSide,depth-frontSideDepth*2,frontHeigh, &normals, 0, farSide/(frontHeigh*49/200), 0, 1);
		shiftX+=farSide;
		//columns

		float columnSpan=5.6f+frontHeigh*56.0f/200;
		int numColumns=floor(dx/columnSpan);
		columnSpan=dx/numColumns;
		float colShift=0.0f;
		for(int c=0;c<numColumns;c++){
			renderFace(renderables[texShift], 1, shiftX+colShift, frontSideDepth, 0, frontHeigh*56.0f/200, colDepth,frontHeigh*122.0f/200.0f, &normals, 0, 56.0f/679, 0, 122.0f/200.0f);
			renderFace(renderables[texShift], 3, shiftX+colShift, frontSideDepth, 0, frontHeigh*56.0f/200, colDepth,frontHeigh*122.0f/200.0f, &normals, 0, 56.0f/679, 0, 122.0f/200.0f);
			renderFace(renderables[texShift], 2, shiftX+colShift, frontSideDepth, 0, frontHeigh*56.0f/200, colDepth,frontHeigh*122.0f/200.0f, &normals, 0, (colDepth/(frontHeigh*56.0f/200))*56.0f/679, 0, 122.0f/200.0f);
			renderFace(renderables[texShift], 4, shiftX+colShift, frontSideDepth, 0, frontHeigh*56.0f/200, colDepth,frontHeigh*122.0f/200.0f, &normals, 0, (colDepth/(frontHeigh*56.0f/200))*56.0f/679, 0, 122.0f/200.0f);
			colShift+=columnSpan;
		}

		//top
		renderFace(renderables[texShift], 3,shiftX,frontSideDepth,frontHeigh*122.0f/200.0f,dx,colDepth,frontHeigh*78.0f/200.0f,&normals,0,dx/(frontWidth),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 6,shiftX,frontSideDepth,frontHeigh*122.0f/200.0f,dx,frontSideDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);
		//back
		renderFace(renderables[texShift+3], 3,shiftX,frontSideDepth*2.0f,0,dx,depth-frontSideDepth*2,frontHeigh*122.0f/200.0f,&normals,0,dx*200*200.0f/(frontHeigh*1429*122.0f),0.5f,1.0f);
		renderFace(renderables[texShift+4], 2,shiftX,frontSideDepth+colDepth,0,0,frontSideDepth-colDepth,frontHeigh*122.0f/200.0f,&normals,270.0f/696,452.0f/696,0,176.0f/200);
		renderFace(renderables[texShift+4], 4,shiftX+dx,frontSideDepth,0,0,frontSideDepth,frontHeigh*122.0f/200.0f,&normals,270.0f/696,452.0f/696,0,176.0f/200);


		shiftX+=dx;

		// 2. MAIN
		//front
		//left
		renderFace(renderables[texShift], 1,shiftX,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 3,shiftX,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 2,shiftX,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
		//renderFace(4,shiftX,0,0),frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,textures[texShift+0],0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
		//right
		renderFace(renderables[texShift], 1,shiftX+frontHeigh*299.0f/200.0f,0,0,frontHeigh*115.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,299.0f/679.0f,414.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 3,shiftX+frontHeigh*299.0f/200.0f,0,0,frontHeigh*115.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,299.0f/679.0f,414.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 2,shiftX+frontHeigh*299.0f/200.0f,0,0,frontHeigh*115.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 4,shiftX+frontHeigh*299.0f/200.0f,0,0,frontHeigh*115.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);

		//top
		renderFace(renderables[texShift], 1,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 3,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);

		renderFace(renderables[texShift], 5,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 6,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);

		renderFace(renderables[texShift], 2,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth/(frontHeigh*679.0f/200)),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 4,shiftX,0,frontHeigh*122.0f/200.0f,frontHeigh*679.0f/200,colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth/(frontHeigh*679.0f/200)),122.0f/200.0f,1.0f);
		//side
		//top

		renderFace(renderables[texShift], 2,shiftX,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(frontSideDepth-colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 4,shiftX,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(frontSideDepth-colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);

		renderFace(renderables[texShift], 5,shiftX,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 6,shiftX,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);

		//column
		renderFace(renderables[texShift], 2,shiftX,colDepth,0,colDepth,frontHeigh*56.0f/200-colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 4,shiftX,0,0,colDepth,frontHeigh*56.0f/200,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 1,shiftX,colDepth,0,colDepth,frontHeigh*56.0f/200-colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);

		// front back
		renderFace(renderables[texShift+1], 3,shiftX,frontSideDepth,0,frontHeigh*679.0f/200,depth-frontSideDepth,frontHeigh,&normals,1.0f,1.0f);
		//front right
		renderFace(renderables[texShift], 1,shiftX+frontWidth-frontHeigh*56.0f/200,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 3,shiftX+frontWidth-frontHeigh*56.0f/200,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 4,shiftX+frontWidth-frontHeigh*56.0f/200,0,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
		//top
		renderFace(renderables[texShift], 2,shiftX+frontWidth-colDepth,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(frontSideDepth-colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 4,shiftX+frontWidth-colDepth,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(frontSideDepth-colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);

		renderFace(renderables[texShift], 5,shiftX+frontWidth-colDepth,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 6,shiftX+frontWidth-colDepth,colDepth,frontHeigh*122.0f/200.0f,colDepth,frontSideDepth-colDepth,frontHeigh*78.0f/200.0f,&normals,0,(colDepth)/(frontHeigh*679.0f/200),122.0f/200.0f,1.0f);
		//column
		renderFace(renderables[texShift], 4,shiftX+frontWidth-colDepth,colDepth,0,colDepth,frontHeigh*56.0f/200-colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 2,shiftX+frontWidth-colDepth,0,0,colDepth,frontHeigh*56.0f/200,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
		renderFace(renderables[texShift], 1,shiftX+frontWidth-colDepth,colDepth,0,colDepth,frontHeigh*56.0f/200-colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);



		shiftX+=frontWidth;

		// 2. RIGHT
		//columns
		colShift=0.0f;
		for(int c=0;c<numColumns;c++){
			renderFace(renderables[texShift], 1,shiftX+dx-(colShift)-frontHeigh*56.0f/200,frontSideDepth,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
			renderFace(renderables[texShift], 3,shiftX+dx-(colShift)-frontHeigh*56.0f/200,frontSideDepth,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,56.0f/679,0.0f,122.0f/200.0f);
			renderFace(renderables[texShift], 2,shiftX+dx-(colShift)-frontHeigh*56.0f/200,frontSideDepth,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
			renderFace(renderables[texShift], 4,shiftX+dx-(colShift)-frontHeigh*56.0f/200,frontSideDepth,0,frontHeigh*56.0f/200,colDepth,frontHeigh*122.0f/200.0f,&normals,0,(colDepth/(frontHeigh*56.0f/200))*56.0f/679,0.0f,122.0f/200.0f);
			colShift+=columnSpan;
		}
		//top
		renderFace(renderables[texShift], 3,shiftX,frontSideDepth,frontHeigh*122.0f/200.0f,dx,colDepth,frontHeigh*78.0f/200.0f,&normals,0,dx/(frontWidth),122.0f/200.0f,1.0f);
		renderFace(renderables[texShift], 6,shiftX,frontSideDepth,frontHeigh*122.0f/200.0f,dx,frontSideDepth,frontHeigh*78.0f/200.0f,&normals,0,1.0f,122.0f/200.0f,1.0f);
		//back
		renderFace(renderables[texShift+3], 3,shiftX,frontSideDepth*2.0f,0,dx,depth-frontSideDepth*2,frontHeigh*122.0f/200.0f,&normals,0,dx*200*200.0f/(frontHeigh*1429*122.0f),0.5f,1.0f);
		renderFace(renderables[texShift+4], 2,shiftX,frontSideDepth,0,0,frontSideDepth,frontHeigh*122.0f/200.0f,&normals,270.0f/696,452.0f/696,0,176.0f/200);
		renderFace(renderables[texShift+4], 4,shiftX+dx,frontSideDepth+colDepth,0,0,frontSideDepth-colDepth,frontHeigh*122.0f/200.0f,&normals,270.0f/696,452.0f/696,0,176.0f/200);
		shiftX+=dx;
		renderFace(renderables[texShift+5], 3,shiftX,frontSideDepth,0,farSide,depth-frontSideDepth*2,frontHeigh,&normals,0,farSide/(frontHeigh*49/200),0.0f,1.0f);
		shiftX+=farSide;



		// 3. BACK
		float texBackStart=0,texBackEnd=0;
		float backWidth=frontHeigh*1340/200;
		float backX=width;
		if(width>backWidth){
			texBackEnd=floor(width/backWidth);
			backX-=texBackEnd*backWidth;
		}
		if(backX>(backWidth*400/1340)){
			texBackEnd+=400.0f/1340;
			backX-=texBackEnd*backWidth;
		}
		renderFace(renderables[texShift+2], 1,0,0,0,width,depth,frontHeigh,&normals,texBackStart,texBackEnd,0,1.0f);

		// 4. SIDES
		//left
		float doorDepth=frontHeigh*244.0f/200;
		float dyHalf=(depth-frontSideDepth-doorDepth)/2.0f;
		renderFace(renderables[texShift+6], 4,0,frontSideDepth,0,width,dyHalf,frontHeigh,&normals,dyHalf/(frontHeigh*182/200),1.0f,0,1.0f);
		renderFace(renderables[texShift+4], 4,0,frontSideDepth+dyHalf,0,width,doorDepth,frontHeigh,&normals,452.0f/696.0f,1.0f,0,1.0f);
		renderFace(renderables[texShift+6], 4,0,depth-dyHalf,0,width,dyHalf,frontHeigh,&normals,dyHalf/(frontHeigh*182/200),1.0f,0,1.0f);


		renderFace(renderables[texShift+6], 2,0,frontSideDepth,0,width,dyHalf,frontHeigh,&normals,dyHalf/(frontHeigh*182/200),1.0f,0,1.0f);
		renderFace(renderables[texShift+4], 2,0,frontSideDepth+dyHalf,0,width,doorDepth,frontHeigh,&normals,452.0f/696.0f,1.0f,0,1.0f);
		renderFace(renderables[texShift+6], 2,0,depth-dyHalf,0,width,dyHalf,frontHeigh,&normals,dyHalf/(frontHeigh*182/200),1.0f,0,1.0f);

		// 5. ROOF
		if(roofTex==-1){
			roofTex=bestTextureForRoof(width,depth-frontSideDepth);
		}
		renderFace(renderables[textureNames.size() + roofTex], 5,0,frontSideDepth,0,width,depth-frontSideDepth,frontHeigh,&normals,1.0f,1.0f);//roof
	}

	void School::_generateMeshVerticesForType1(ucore::TextureManager* textureManager) {
		if (stories < 2) stories = 2;
		
		float frontHeight = 3.0f;
		float frontTopHeight = 0.83f;
		float frontTopDepth = 1.0f;
		float frontWidth = (frontHeight + frontTopHeight) * 524 / 200;

		float frontSideHeight = 7.64f;
		float frontSideWidth = frontSideHeight * 447 / 250;

		float depthOneBuilding = 22.0f;
		float backWidth = frontSideHeight * 549 / 250;
		float mainSideWidth = (frontHeight + frontSideHeight) * 549 / 250;

		float leftWidth = frontSideHeight * 472 / 250;
		float rightWidth = frontSideHeight * 251 / 250;
		float backWidth2 = frontSideHeight * 549 / 250;

		float corridorWidth = (depth - frontTopDepth) / 3.0f;
		if (corridorWidth > depthOneBuilding) corridorWidth = depthOneBuilding;

		float shiftX = 0;
		float shiftY = 0;

		float dx = width - frontWidth;

		// front left
		renderFace(renderables[3], 3, shiftX, frontTopDepth, 0, dx/2, depth, frontSideHeight * 104 / 250, &normals, 0, floor(dx * 0.5 / frontSideWidth) + 1, 0, 104.0f / 250.0f);
		shiftY += frontSideHeight * 104 / 250;
		int extraStories = stories - 2;
		if (extraStories > 0) {
			renderFace(renderables[4], 3, shiftX, frontTopDepth, shiftY, dx*0.5, depth, extraStories * frontSideHeight * 104 / 250, &normals, 0, floor(dx * 0.5 / frontSideWidth) + 1, 0, extraStories);
			shiftY += extraStories * frontSideHeight * 104 / 250;
		}
		renderFace(renderables[3], 3, shiftX, frontTopDepth, shiftY, dx/2, depth, frontSideHeight * 146 / 250, &normals, 0, floor(dx * 0.5 / frontSideWidth) + 1, 104.0f / 250, 1);
		shiftX+=dx/2.0f;

		// main entrance
		renderFace(renderables[2], 3, shiftX, frontTopDepth, 0, frontWidth, depth, frontHeight, &normals, 0, 1, 0, 140.f/200.0f);
		for (int i = 1; i < 6; i++) {
			if (i == 5) {
				renderFace(renderables[2], i, shiftX, 0, frontHeight, frontWidth, frontTopDepth, frontTopHeight, &normals, 0, 1, 140.f/200, 161.0f/200);
			} else if (i == 1 || i == 3) {
				renderFace(renderables[2], i, shiftX, 0, frontHeight, frontWidth, frontTopDepth, frontTopHeight, &normals, 0, 1, 161.0f/200, 1);
			} else {
				renderFace(renderables[2], i, shiftX, 0, frontHeight, frontWidth, frontTopDepth, frontTopHeight, &normals, 0, 50.0f/524.0f, 161.0f/200, 1);
			}
		}
		renderFace(renderables[0], 2, shiftX, frontTopDepth, 0, frontWidth, depth - corridorWidth - frontTopDepth, frontHeight + frontTopHeight, &normals, 0, floor(depth/mainSideWidth) + 1, 0, 1);
		renderFace(renderables[0], 4, shiftX, frontTopDepth, 0, frontWidth, depth - corridorWidth - frontTopDepth, frontHeight + frontTopHeight, &normals, 0, floor(depth/mainSideWidth) + 1, 0, 1);
		if (roofTex2 == -1) {
			roofTex2 = bestTextureForRoof(frontWidth, depth-corridorWidth-frontTopDepth);
		}
		renderFace(renderables[textureNames.size() + roofTex2], 5, shiftX, frontTopDepth, 0, frontWidth, depth - corridorWidth - frontTopDepth, frontHeight + frontTopHeight, &normals, 0, 1, 0, 1);
		shiftX+=frontWidth;

		// front right
		shiftY = 0;
		renderFace(renderables[3], 3, shiftX, frontTopDepth, 0, dx/2, depth, frontSideHeight * 104 / 250, &normals, 0, floor(dx * 0.5 /frontSideWidth) + 1, 0, 104.0f/250);
		shiftY += frontSideHeight * 104 / 250;
		extraStories = stories - 2;
		if (extraStories > 0) {
			renderFace(renderables[4], 3, shiftX, frontTopDepth, shiftY, dx/2, depth, extraStories * frontSideHeight * 104 /250, &normals, 0, floor(dx * 0.5 / frontSideWidth) +1, 0, extraStories);
			shiftY += extraStories * frontSideHeight * 104 / 250;
		}
		renderFace(renderables[3], 3, shiftX, frontTopDepth, shiftY, dx/2, depth, frontSideHeight * 146 / 250, &normals, 0, floor(dx * 0.5 / frontSideWidth) + 1, 104.0f / 250, 1);

		// left
		shiftX = 0.0f;
		shiftY = 0;
		renderFace(renderables[5], 4, shiftX, frontTopDepth, 0, width, depth - frontTopDepth, frontSideHeight*98/250.0f, &normals, 0, floor((depth-frontTopDepth) * 0.5 / leftWidth) + 1, 0, 98.0f/250.0f);
		shiftY+=frontSideHeight*98/250.0f;
		extraStories = stories - 2;
		if (extraStories > 0) {
			renderFace(renderables[6], 4, shiftX, frontTopDepth, shiftY, width, depth - frontTopDepth, extraStories*frontSideHeight*104/250.0f, &normals, 0, floor((depth-frontTopDepth)*0.5/leftWidth) + 1, 0, extraStories);
			shiftY += extraStories * frontSideHeight * 104 / 250;
		}
		renderFace(renderables[5], 4, shiftX, frontTopDepth, shiftY, width, depth-frontTopDepth,frontSideHeight*152.0f/250.0f, &normals, 0, floor((depth-frontTopDepth)/(2*leftWidth)) + 1, 98.0f/250.0f, 1);

		// right
		shiftX = 0;
		shiftY = 0;
		renderFace(renderables[9], 2, shiftX, frontTopDepth, 0, width, depth-frontTopDepth,frontSideHeight*128.0f/250.0f, &normals, 0, floor((depth-frontTopDepth) * 0.5 / rightWidth) + 1, 0, 128.0f/250.0f);
		shiftY+=frontSideHeight*128/250.0f;
		extraStories = stories - 2;
		if (extraStories > 0) {
			renderFace(renderables[10], 2, shiftX, frontTopDepth, shiftY, width, depth-frontTopDepth,extraStories*frontSideHeight*104/250.0f, &normals, 0, floor((depth-frontTopDepth) * 0.5 / rightWidth) + 1, 0, extraStories);
			shiftY+=extraStories*frontSideHeight*104/250.0f;
		}
		renderFace(renderables[9], 2, shiftX, frontTopDepth, shiftY, width, depth-frontTopDepth, frontSideHeight*122.0f/250.0f, &normals, 0, floor((depth-frontTopDepth) * 0.5 / rightWidth) + 1, 122.0f/250.0f, 1);

		// back
		shiftX = 0;
		shiftY = 0;
		renderFace(renderables[0], 1, shiftX, frontTopDepth, 0, width, depth-frontTopDepth, frontSideHeight*127.0f/250.0f, &normals, 0, floor((width)/(2*backWidth2)) + 1, 0, 127.0f/250.0f);
		shiftY+=frontSideHeight*127/250.0f;
		extraStories = stories - 2;
		if(extraStories>0){
			renderFace(renderables[1], 1, shiftX, frontTopDepth, shiftY, width, depth - frontTopDepth, extraStories*frontSideHeight*104/250.0f, &normals, 0, floor((depth-frontTopDepth)/(2*backWidth2))+1, 0, extraStories);
			shiftY+=extraStories*frontSideHeight*104/250.0f;
		}
		renderFace(renderables[0], 1, shiftX, frontTopDepth, shiftY, width, depth-frontTopDepth, frontSideHeight*123.0f/250.0f, &normals, 0, floor((width-frontTopDepth)/(2*backWidth2))+1, 123.0f/250.0f, 1);

		// center
		//corridor from main to corridor side
		bool corridorFromMain = false;
		if (dx/2.0f > corridorWidth) {
			corridorFromMain = true;
			for(int sid=0;sid<2;sid++){
				if(sid==0){
					shiftX=corridorWidth;
				} else {
					shiftX = dx/2.0f + frontWidth;
				}
				shiftY=0;
				renderFace(renderables[0], 1,shiftX,frontTopDepth,shiftY,dx/2.0f-corridorWidth,corridorWidth,frontSideHeight*127.0f/250.0f,&normals,0,floor(((dx/2.0f)-corridorWidth)/(2*backWidth2))+1.0f,0.0f,127.0f/250.0f);
				shiftY+=frontSideHeight*127/250.0f;
				extraStories = stories-2;
				if(extraStories>0){
					renderFace(renderables[1], 1,shiftX,frontTopDepth,shiftY,dx/2.0f-corridorWidth,corridorWidth,extraStories*frontSideHeight*104/250.0f,&normals,0,floor(((dx/2.0f)-corridorWidth)/(2*backWidth2))+1.0f,0.0f,1.0f*extraStories);
					shiftY+=extraStories*frontSideHeight*104/250.0f;
				}
				renderFace(renderables[0], 1,shiftX,frontTopDepth,shiftY,dx/2.0f-corridorWidth,corridorWidth,frontSideHeight*123.0f/250.0f,&normals,0,floor(((dx/2.0f)-corridorWidth)/(2*backWidth2))+1.0f,123.0f/250.0f,1.0f);
			}

			//left and right corridor
			float depth2 = frontTopDepth+corridorFromMain*corridorWidth;
			float leftRightCorWidth = depth-frontTopDepth-corridorFromMain*2.0f*corridorWidth;
			for(int sid=0;sid<2;sid++){
				int face;
				shiftY=0;
				if(sid==0){
					shiftX=0;
					face=2;
				}
				else{
					shiftX=(dx/2.0f)+frontWidth+corridorFromMain*((dx/2.0f)-corridorWidth);
					face=4;
				}
				shiftY=0;
				renderFace(renderables[0], face,shiftX,depth2,shiftY,corridorWidth,leftRightCorWidth,frontSideHeight*127.0f/250.0f,&normals,0,floor((leftRightCorWidth)/(2*backWidth2))+1.0f,0.0f,127.0f/250.0f);
				shiftY+=frontSideHeight*127/250.0f;
				extraStories=stories-2;
				if(extraStories>0){
					renderFace(renderables[1], face,shiftX,depth2,shiftY,corridorWidth,leftRightCorWidth,extraStories*frontSideHeight*104/250.0f,&normals,0,floor((leftRightCorWidth)/(2*backWidth2))+1.0f,0.0f,1.0f*extraStories);
					shiftY+=extraStories*frontSideHeight*104/250.0f;
				}
				renderFace(renderables[0], face,shiftX,depth2,shiftY,corridorWidth,leftRightCorWidth,frontSideHeight*123.0f/250.0f,&normals,0,floor((leftRightCorWidth)/(2*backWidth2))+1.0f,123.0f/250.0f,1.0f);
			}
			//brick sides
			for(int sid=0;sid<2;sid++){
				int face;
				shiftY=0;
				if(sid==0){
					face=2;
					shiftX=corridorWidth;
				}
				else{
					shiftX=(dx/2.0f)+frontWidth;
					face=4;
				}
				shiftY=0;
				renderFace(renderables[11], face,shiftX,frontTopDepth,shiftY,dx/2.0f-corridorWidth,corridorWidth,frontSideHeight+frontSideHeight*extraStories*104.0f/250.0f,&normals,0,floor(corridorWidth)/(3.35f)+1.0f,0.0f,(frontSideHeight+frontSideHeight*extraStories*104.f/250.0f)/(10.0f));
			}
			//front of the back corridor
			shiftX=corridorWidth;
			float backCorridorWidth=width-2*corridorWidth;
			shiftY=0;
			renderFace(renderables[0], 3, shiftX, depth-corridorWidth, shiftY, backCorridorWidth, corridorWidth, frontSideHeight*127.0f/250.0f, &normals, 0, floor(backCorridorWidth*0.5/backWidth2)+1, 0, 127.0f/250.0f);
			shiftY+=frontSideHeight*127/250.0f;
			extraStories=stories-2;
			if(extraStories>0){
				renderFace(renderables[1], 3, shiftX, depth-corridorWidth, shiftY, backCorridorWidth, corridorWidth, frontSideHeight*extraStories*104/250.0f, &normals, 0, floor(backCorridorWidth*0.5/backWidth2)+1, 0, extraStories);
				shiftY+=extraStories*frontSideHeight*104/250.0f;
			}
			renderFace(renderables[0], 3, shiftX, depth-corridorWidth, shiftY, backCorridorWidth, corridorWidth,frontSideHeight*123.0f/250.0f, &normals, 0, floor(backCorridorWidth*0.5/backWidth2)+1, 123.0f/250.0f, 1);
		}else{
			//there is not square center
			for(int sid=0;sid<2;sid++){
				int face;
				shiftY=0;
				if(sid==0){
					face=2;
					shiftX=0;
				}
				else{
					shiftX=(dx/2.0f)+frontWidth;
					face=4;
				}
				shiftY=0;
				renderFace(renderables[11], face,shiftX,frontTopDepth,shiftY,dx/2.0f,depth-corridorWidth-frontTopDepth,frontSideHeight+frontSideHeight*extraStories*104.0f/250.0f,&normals,0,floor(depth-corridorWidth-frontTopDepth)/(3.35f)+1.0f,0.0f,(frontSideHeight+frontSideHeight*extraStories*104.f/250.0f)/(10.0f));
			}


			//front of the back corridor
			shiftX=dx/2.0f;
			float backCorridorWidth=width-frontWidth;
			shiftY=0;
			renderFace(renderables[0], 3,shiftX,depth-corridorWidth,shiftY,frontWidth,corridorWidth,frontSideHeight*127.0f/250.0f,&normals,0,floor((backCorridorWidth)/(2*backWidth2))+1.0f,0.0f,127.0f/250.0f);
			shiftY+=frontSideHeight*127/250.0f;
			extraStories=stories-2;
			if(extraStories>0){
				renderFace(renderables[1], 3,shiftX,depth-corridorWidth,shiftY,frontWidth,corridorWidth,frontSideHeight*extraStories*104/250.0f,&normals,0,floor((backCorridorWidth)/(2*backWidth2))+1.0f,0.0f,1.0f*extraStories);
				shiftY+=extraStories*frontSideHeight*104/250.0f;
			}
			renderFace(renderables[0], 3,shiftX,depth-corridorWidth,shiftY,frontWidth,corridorWidth,frontSideHeight*123.0f/250.0f,&normals,0,floor((backCorridorWidth)/(2*backWidth2))+1.0f,123.0f/250.0f,1.0f);
		}

		// roof
		ucore::Loop3D allRoof;
		float toltaH = frontSideHeight + frontSideHeight * extraStories * 104.0f / 250.0f;
		allRoof.push_back(QVector3D(width,frontTopDepth,toltaH));
		allRoof.push_back(QVector3D(width,depth,toltaH));
		allRoof.push_back(QVector3D(0,depth,toltaH));
		allRoof.push_back(QVector3D(0,frontTopDepth,toltaH));
		allRoof.push_back(QVector3D(dx/2.0f,frontTopDepth,toltaH));
		if((dx/2.0f)>corridorWidth){
			//brick
			allRoof.push_back(QVector3D(dx/2.0f,frontTopDepth+corridorWidth,toltaH));
			//central sq
			float centerCor=(dx/2.0f)-corridorWidth;
			allRoof.push_back(QVector3D((dx/2.0f)-centerCor,frontTopDepth+corridorWidth,toltaH));

			allRoof.push_back(QVector3D((dx/2.0f)-centerCor,depth-corridorWidth,toltaH));
			allRoof.push_back(QVector3D((dx/2.0f)+centerCor+frontWidth,depth-corridorWidth,toltaH));

			allRoof.push_back(QVector3D((dx/2.0f)+centerCor+frontWidth,frontTopDepth+corridorWidth,toltaH));
			//
			//brick
			allRoof.push_back(QVector3D(dx/2.0f+frontWidth,frontTopDepth+corridorWidth,toltaH));
		}else{
			allRoof.push_back(QVector3D(dx/2.0f,depth-corridorWidth,toltaH));
			allRoof.push_back(QVector3D(dx/2.0f+frontWidth,depth-corridorWidth,toltaH));

		}
		allRoof.push_back(QVector3D(dx/2.0f+frontWidth,frontTopDepth,toltaH));
		if(roofTex==-1){
			roofTex=bestTextureForRoof(width,depth);
		}
		//renderFlatRoof(allRoof, roofTextures[roofTex]);


			
		ucore::RenderableConcave* concave = new ucore::RenderableConcave(allRoof, roofTextures[roofTex]);
		renderables.push_back(concave);
	}
}


