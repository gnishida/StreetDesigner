/*********************************************************************************
*
*		MTC Project - Geometry Project - IH Class
*
*
*		@desc Class containing the renderer for IH buildings
*		@author Ignacio Garcia Dorado
*
*********************************************************************************/

#include "IHFawley.h"

namespace pb {
	#define NUM_SUBTYPE 3

	std::vector<ucore::Texture*> IHFawley::textures;
	bool IHFawley::texturesLoaded = false;
	std::vector<QString> IHFawley::textureNames;
	//GLUquadricObj* IHFawley::cylinder=0;

	/*********************************************************************************
	*	IHFawleyRenderer constructor
	*		Constructor for IH renderers.  Initializes instance variables common to 
	*		all IH renderers.
	*********************************************************************************/
	IHFawley::IHFawley(const ucore::Loop3D& footprint, int numStories, float height) : AbstractParametricBuilding(footprint, numStories, height) {
		if (textureNames.size() == 0) {
			textureNames.push_back("data/buildings/IHFawley/top.jpg");//1
			textureNames.push_back("data/buildings/IHFawley/tube.jpg");//2
			textureNames.push_back("data/buildings/IHFawley/buld2.jpg");//4
			textureNames.push_back("data/buildings/IHFawley/roof1.jpg");//6
			textureNames.push_back("data/buildings/IHFawley/roof2.jpg");//7
			textureNames.push_back("data/buildings/IHFawley/roof3.jpg");//8

			for(int i=0;i<NUM_SUBTYPE;i++){
				textureNames.push_back("data/buildings/IHFawley/chimney_"+QString::number(i)+".jpg");//0
				textureNames.push_back("data/buildings/IHFawley/buld1_"+QString::number(i)+".jpg");//1
				textureNames.push_back("data/buildings/IHFawley/buld3_"+QString::number(i)+".jpg");//2
				textureNames.push_back("data/buildings/IHFawley/buld12_"+QString::number(i)+".jpg");//3
			}
		}

		/*if(cylinder==0){
			cylinder = gluNewQuadric();
			gluQuadricTexture(cylinder, GL_TRUE);
		}*/

		roofTex=-1;
		srand(width * depth);
		subType = rand() % NUM_SUBTYPE;
		numBlockX = rand();
		numBlockY = rand();
		backTex = NUM_SUBTYPE * 5 + rand() % 2;
	}

	/*********************************************************************************
	*	IHFawleyRenderer destructor
	*		Destructor for IH building renderers.  Nothing to do.
	*********************************************************************************/
	IHFawley::~IHFawley() {
	}

	/*********************************************************************************
	*	InitializeTextures
	*		Loads the textures for IH buildings.
	*********************************************************************************/
	void IHFawley::initializeTextures(ucore::TextureManager* textureManager) {
		if (!texturesLoaded) {
			AbstractParametricBuilding::initializeTextures(&textureNames, &textures, textureManager);
			texturesLoaded = true;
		}
	}

	void IHFawley::_generateMeshVertices() {
		float dx=width;
		float dy=depth;	
			
		bool rotated=false;
		if((dx/dy)<(60.0f/80.0f)){
			//glPushMatrix();
			//glTranslatef(0,dy,0);
			//glRotatef(270.0f,0,0,1.0f);

			rotated=true;
			float sw=dx;
			dx=dy;
			dy=sw;
				
		}

		float blockX=dx;
		float blockY=dy;
		float scale=1.0f;

		if(dx>30.0f){
			scale=dx/30.0f;
		}
		if(dy<((1.05f/2+1.05f+4.0f+2.8f)*scale)){
			scale=dy/((1.05f/2+1.05f+4.0f+2.8f)*scale);
		}
			
		float towerRadius=1.05f*scale;
		float towerHeigh=18.0f*scale;

		float distBetweenTow=((rand()%25)+towerRadius*4.1f)*scale;//25.0f*scale;
		float tubesWidth=0.8f*scale;
		float distTubes=4.0f*scale;
			
			
		float buld1TexWidth=3.3f*scale;
		float buld1Height=3.2f*scale;
		float buld1Y=2.8f*scale;
		float roofBuld1Width=5.5f*scale;
		float margin1X=1.0f*scale;

		float buld2TexWidth=3.3f*scale;
		float buld2Height=3.8f*scale;
		float buld2Y=2.5f*scale;
		float roofBuld2Width=1.8f*scale;

		float buld3TexWidth=3.3f*scale;
		float buld3Height=2.6f*scale;
		float buld3Y=6.0f*scale;
		float roofBuld3Width=2.0f*scale;
		float margin3X=1.0f*scale;

		int numY=1;
		bool drawBld2,drawBld3,drawBld3Dupl;
		if(floor(dy/(towerRadius/2+towerRadius+distTubes+buld1Y))>=2.0f){
			blockY=dy/2.0f;
			numY=2;
		}
			
		if(blockY>(towerRadius/2+towerRadius+distTubes+buld1Y+buld2Y)){
			drawBld2=true;
		}
		else
			drawBld2=false;
		if(blockY>(towerRadius/2+towerRadius+distTubes+buld1Y+buld2Y+buld3Y)){
			drawBld3=true;
			drawBld3Dupl=true;
		}else{
			if(numY==2 && dy>=(towerRadius+2*towerRadius+2*distTubes+2*buld1Y+2*buld2Y+buld3Y) ){
				drawBld3=true;
				drawBld3Dupl=false;
			}else{
				drawBld3=false;
				drawBld3Dupl=false;
			}
		}

		numY = 1;
		for(int sidesY=0;sidesY<numY;sidesY++){
			/*if(sidesY==1){
				glPushMatrix();
				glTranslatef(dx,dy,0);
				glRotatef(180.0f,0,0,1.0f);
			}*/
			//Cylinders and Tubes
			int numCylinders=floor(blockX/distBetweenTow)+1;
			numCylinders=numCylinders>20?20:numCylinders;
			float shiftY=0;
			for(int i=0;i<numCylinders;i++){
				//cylinder
				//glPushMatrix();
				//glTranslatef((i+1)*blockX/(numCylinders+1),towerRadius/2,0);
				renderCylinder((i+1)*blockX/(numCylinders+1), towerRadius/2, 0, towerRadius, towerRadius/2.0f,towerHeigh, 10,10,textures[6+subType*4+0],textures[0]);//1.5
				//glPopMatrix();
				//tube
				for(int f=0;f<6;f++){
					renderFace(f,(i+1)*blockX/(numCylinders+1)-tubesWidth/2.0f,towerRadius/2,0,tubesWidth,towerRadius+distTubes,tubesWidth,&normals,textures[1],1.0f,1.0f);
				}
			}

			shiftY+=towerRadius/2+towerRadius+distTubes;
			//Bld1
			for(int f=1;f<5;f++){
				if(f==2||f==4)
					renderFace(f,margin1X,shiftY,0,blockX-margin1X*2,buld1Y,buld1Height,&normals,textures[6+subType*4+3],1.0f,1.0f);
				else
					renderFace(f,margin1X,shiftY,0,blockX-margin1X*2,buld1Y,buld1Height,&normals,textures[6+subType*4+1],floor(blockX/buld1TexWidth)+1.0f,1.0f);
			}
			renderFace(5,margin1X,shiftY,0,blockX-margin1X*2,buld1Y,buld1Height,&normals,textures[3],floor(blockX/roofBuld1Width)+1.0f,1.0f);
			shiftY+=buld1Y;

			//Bld2
			if(drawBld2){
				for(int f=1;f<5;f++){
					if(f==2||f==4)
						renderFace(f,0,shiftY,0,blockX,buld2Y,buld2Height,&normals,textures[2],1.0f,1.0f);
					else
						renderFace(f,0,shiftY,0,blockX,buld2Y,buld2Height,&normals,textures[2],floor(blockX/buld2TexWidth)+1.0f,1.0f);
				}
				renderFace(5,0,shiftY,0,blockX,buld2Y,buld2Height,&normals,textures[4],floor(blockX/roofBuld2Width)+1.0f,1.0f);
				shiftY+=buld2Y;
			}

			//Bld3
			if((drawBld3&&sidesY==0)||(drawBld3Dupl&&sidesY==1)){
				for(int f=1;f<5;f++){
					if(f==2||f==4)
						renderFace(f,margin3X,shiftY,0,blockX-margin3X*2,buld3Y,buld3Height,&normals,textures[6+subType*4+2],1.0f,1.0f);
					else
						renderFace(f,margin3X,shiftY,0,blockX-margin3X*2,buld3Y,buld3Height,&normals,textures[6+subType*4+2],floor(blockX/buld3TexWidth)+1.0f,1.0f);
				}
				renderFace(5,margin3X,shiftY,0,blockX-margin3X*2,buld3Y,buld3Height,&normals,textures[5],floor(blockX/roofBuld3Width)+1.0f,1.0f);
				shiftY+=buld3Y;
			}
			if(sidesY==1){
				//glPopMatrix();
			}

		}//for
		if(rotated){
			//glPopMatrix();
		}

		//glPopMatrix();//pos in scene
	}
}


