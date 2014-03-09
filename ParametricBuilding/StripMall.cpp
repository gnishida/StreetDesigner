/*********************************************************************************
*
*		MTC Project - Geometry Project - RS Class
*
*
*		@desc Class containing the renderer for RS buildings
*		@author Ignacio Garcia Dorado
*
*********************************************************************************/

#include "StripMall.h"
#include "../Core/RenderableTriangleList.h"

namespace pb {
	#define NUM_SUBTYPE 7
	#define SUBTYPE_3_COLORS 5
	#define SUBTYPE_3_NUMSHOPS 5
	#define SUBTYPE_6_COVERS 3


	std::vector<ucore::Texture*> StripMall::textures;
	bool StripMall::texturesLoaded = false;
	std::vector<QString> StripMall::textureNames;

	/*********************************************************************************
	*	RSRenderer constructor
	*		Constructor for RS renderers.  Initializes instance variables common to 
	*		all RS renderers.
	*********************************************************************************/
	StripMall::StripMall(const ucore::Loop3D& footprint, int numStories, float height) : AbstractParametricBuilding(footprint, numStories, height) {
		if(textureNames.size()<1){

			//subtype 1
			textureNames.push_back("data/buildings/RS/0_back.jpg");//0
			textureNames.push_back("data/buildings/RS/0_front.jpg");//1
			textureNames.push_back("data/buildings/RS/0_right.jpg");//2
			textureNames.push_back("data/buildings/RS/0_right2.jpg");//3
			textureNames.push_back("data/buildings/RS/0_left.jpg");//4
			textureNames.push_back("data/buildings/RS/0_horizontal.jpg");//5
			textureNames.push_back("data/buildings/RS/0_stickTex.jpg");//6
			textureNames.push_back("data/buildings/RS/0_horizontal2.jpg");//7

			//subtype 2
			textureNames.push_back("data/buildings/RS/3_back.jpg");//0
			textureNames.push_back("data/buildings/RS/3_bottomFront.jpg");//1
			textureNames.push_back("data/buildings/RS/3_stickTex.jpg");//2
			textureNames.push_back("data/buildings/RS/3_roof.jpg");//3
			textureNames.push_back("data/buildings/RS/3_top.jpg");//4
			textureNames.push_back("data/buildings/RS/3_roof2.jpg");//5
			textureNames.push_back("data/buildings/RS/3_roof3.jpg");//6

			//subtype 3

			textureNames.push_back("data/buildings/RS/bottomT.jpg");//0
			textureNames.push_back("data/buildings/RS/topEdge.jpg");//1
			textureNames.push_back("data/buildings/RS/top.jpg");//2
			textureNames.push_back("data/buildings/RS/mid.jpg");//3
			textureNames.push_back("data/buildings/RS/topBack.jpg");//4
			textureNames.push_back("data/buildings/RS/bottomBack.jpg");//5

			for(int i=1;i<=SUBTYPE_3_COLORS;i++){
				textureNames.push_back("data/buildings/RS/corner"+QString::number(i)+".jpg");//5->(5+SUBTYPE_3_CORNERS)
				textureNames.push_back("data/buildings/RS/bottomB"+QString::number(i)+".jpg");//0
				textureNames.push_back("data/buildings/RS/bottomD"+QString::number(i)+".jpg");//5->(5+SUBTYPE_3_CORNERS)
			}
			for(int i=1;i<=SUBTYPE_3_NUMSHOPS;i++){
				textureNames.push_back("data/buildings/RS/shop"+QString::number(i)+".jpg");
			}

			textureNames.push_back("data/buildings/RS/topEdge2.jpg");//SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+6+0
			textureNames.push_back("data/buildings/RS/top2.jpg");//1
			textureNames.push_back("data/buildings/RS/topBack2.jpg");//2

			//subtype 4
			//21+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+3+0
			textureNames.push_back("data/buildings/RS/rs_urban_front.jpg");//0
			textureNames.push_back("data/buildings/RS/rs_urban_sides.jpg");//1
			textureNames.push_back("data/buildings/RS/rs_urban_sides2.jpg");//2
			textureNames.push_back("data/buildings/RS/rs_urban_backDoor.jpg");//3
			textureNames.push_back("data/buildings/RS/rs_urban_frontTop.jpg");//4
			textureNames.push_back("data/buildings/RS/rs_urban_frontBack.jpg");//5


			textureNames.push_back("data/buildings/RS/rs_urban_front1.jpg");//6
			textureNames.push_back("data/buildings/RS/rs_urban_frontTop1.jpg");//7

			textureNames.push_back("data/buildings/RS/rs_urban_front2.jpg");//8
			textureNames.push_back("data/buildings/RS/rs_urban_frontTop2.jpg");//9

			for(int i=0;i<SUBTYPE_6_COVERS;i++){
				textureNames.push_back("data/buildings/RS/rs_urban_cover"+QString::number(i)+".jpg");//10
			}

		}

		roofTex=-1;
		subType=-1;
		float subMins[NUM_SUBTYPE] ={7.5f, 7.0f, 0/*5.0f*/, 0/*5.0f*/,10.0f,10.0f,10.0f}; // fill with subtype minimum widths
		while (subType == -1) {	
			subType=qrand()%NUM_SUBTYPE;
			if(width < subMins[subType] || depth < subMins[subType]) subType=-1;
		}

		//scale logic
		float minDim=5.0f;
		if (width < minDim || depth < minDim) {
			scale = min(width, depth) / minDim;
			width /= scale;
			depth /= scale;
		}
	}

	/*********************************************************************************
	*	RSRenderer destructor
	*		Destructor for RS building renderers.  Nothing to do.
	*********************************************************************************/
	StripMall::~StripMall() {
	}

	void StripMall::_generateMeshVertices(ucore::TextureManager* textureManager) {
		AbstractParametricBuilding::loadRoofTextures(textureManager);

		for (int i = 0; i < textureNames.size(); i++) {
			renderables.push_back(new ucore::RenderableTriangleList(textureManager->get(textureNames[i])));
		}
		for (int i = 0; i < roofTextures.size(); i++) {
			renderables.push_back(new ucore::RenderableTriangleList(roofTextures[i]));
		}

		qsrand(width * depth);

		float dx=width;
		float dy=depth;
			
		float dyOrig=dy;
		float dxOrig=dx;

		if(subType==1){
			float coverDepth=2.0f;
			float frontHeight=4.5f;
			float frontTexWidth=frontHeight*1559/200;
			float shop[5];
			shop[0]=frontTexWidth*371/1559;
			shop[1]=frontTexWidth*292/1559;
			shop[2]=frontTexWidth*308/1559;
			shop[3]=frontTexWidth*298/1559;
			shop[4]=frontTexWidth*290/1559;

			float backWidth=frontHeight*341/303;

			float rightDepth=frontHeight*608/200;
			float right2Depth=frontHeight*38/186;

			float leftDepth=frontHeight*44/186;

			int numShop[5]={0};
			bool end=false;
			while(end==false){
				for(int s=0;s<5;s++){
					if(dx>shop[s]){
						dx-=shop[s];
						numShop[s]++;
					}
					else
						end=true;
				}
			}

			float shiftX=0;

			//front
			float frontTex=numShop[0]*371.0f/1559.0f+numShop[1]*292.0f/1559.0f+numShop[2]*308.0f/1559.0f+numShop[3]*298.0f/1559.0f+numShop[4]*290.0f/1559.0f;

			renderFace(renderables[1], 3, shiftX, coverDepth, 0, dxOrig, dyOrig-coverDepth, frontHeight, &normals, frontTex, 1);//front
			renderFace(renderables[0], 1, shiftX, coverDepth, 0, dxOrig, dyOrig-coverDepth, frontHeight, &normals, float(dxOrig/backWidth)+1, 1);//back

			//horizontal
			for(int f=1;f<7;f++){//front right hor box
				if(f<5){
					if(f==1||f==3)//front
						renderFace(renderables[5], f, shiftX,0,frontHeight*117/200,dxOrig,coverDepth,frontHeight*15/200,&normals, 0, floor(dxOrig/0.34f)+1, 0, 21.0f/44.0f);
					else//sides
						renderFace(renderables[5], f, shiftX,0,frontHeight*117/200,dxOrig,coverDepth,frontHeight*15/200,&normals, 0, floor(coverDepth/0.34f)+1, 0, 21.0f/44.0f);
				}
				else
					renderFace(renderables[5], f, shiftX,0,frontHeight*117/200,dxOrig,coverDepth,frontHeight*15/200,&normals, 0, floor(dxOrig/0.34f)+1, 21.0/44.0f, 1);
			}

			//XX drawRoofTriangle(renderables[7], shiftX, 0,frontHeight*132/200,dxOrig, 2*coverDepth,frontHeight*27/200,&normals, textures[6],0,textures[7],true,true,floor(dxOrig/0.34f)+1.0f);//turned half

			// STICKS
			float stickShift=0.0f;
			float stickWidth=0.4f;
			float span=dxOrig;
			int numTotShop=0;
			for(int s=0;s<5;s++){
				span-=numShop[s]*shop[s];
				numTotShop+=numShop[s];
			}
			span/=numTotShop;
			// last
			for(int f=1;f<5;f++){//front right hor box
				renderFace(renderables[6], f,shiftX,0,0,stickWidth,stickWidth,frontHeight*117/200,&normals,1.0f,1.0f);
			}
			for(int s=0;s<numShop[0];s++){//choose 0 because it will have the most number of shops
				for(int sN=0;sN<5;sN++){
					if(s==0&&sN==0){stickShift+=shop[sN]+span;continue;
					}
					if(numShop[sN]>s){

						for(int f=1;f<5;f++){//front right hor box
							renderFace(renderables[6], f,shiftX+stickShift+stickWidth/2.0f,0,0,stickWidth,stickWidth,frontHeight*117/200,&normals,1.0f,1.0f);
						}
						stickShift+=shop[sN]+span;
					}
					else
						break;
				}
			}
			// last
			for(int f=1;f<5;f++){//front right hor box
				renderFace(renderables[6], f,dxOrig-stickWidth,0,0,stickWidth,stickWidth,frontHeight*117/200,&normals,1.0f,1.0f);
			}

			//right
			if((dyOrig-coverDepth)>=rightDepth){
				renderFace(renderables[2], 2,shiftX,coverDepth,0,dxOrig,rightDepth,frontHeight,&normals,1.0f,1.0f);//right
				renderFace(renderables[3], 2,shiftX,coverDepth+rightDepth,0,dxOrig,dyOrig-coverDepth-rightDepth,frontHeight,&normals,floor((dyOrig-coverDepth-rightDepth)/right2Depth)+1.0f,1.0f);//right
			}else{
				renderFace(renderables[3], 2,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,frontHeight,&normals,floor((dyOrig-coverDepth)/right2Depth)+1.0f,1.0f);//right
			}
			//left
			renderFace(renderables[4], 4,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,frontHeight,&normals,floor((dyOrig-coverDepth)/leftDepth)+1.0f,1.0f);//right

			//draw roof
			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig-coverDepth);
			}
			renderFace(renderables[textureNames.size() + roofTex], 5, shiftX, coverDepth, frontHeight, dxOrig, dyOrig-coverDepth, 0, &normals, 1, 1);//roof

		}
		if(subType==0){
			float texShift=8;

			float letters=0.2f/1.4f;
			float coverDepth=3.65f/1.4f+letters;
			float frontHeight=7.3152f/1.4f;
			float bottomFrontHeigh=4.81f/1.4f;
			float horizHeigh=1.37f/1.4f;
			float frontTexWidth=bottomFrontHeigh*922/74;

			float shop[5];
			shop[0]=frontTexWidth*206/922;
			shop[1]=frontTexWidth*206/922;
			shop[2]=frontTexWidth*157/922;
			shop[3]=frontTexWidth*187/922;
			shop[4]=frontTexWidth*170/922;
			int numShop[5]={0};
			bool end=false;
			while(end==false){
				for(int s=0;s<5;s++){
					if(dx>shop[s]){
						dx-=shop[s];
						numShop[s]++;
					}
					else
						end=true;
				}
			}

			float backWidth=frontHeight*58/150;
			float shiftX=0;

			//front
			float frontTex=numShop[0]*206.0f/922.0f+numShop[1]*206.0f/922.0f+numShop[2]*157/922.0f+numShop[3]*187.0f/922.0f+numShop[4]*170.0f/922.0f;

			renderFace(renderables[texShift+1], 3,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,bottomFrontHeigh,&normals, frontTex, 1);//front

			//horizontal
			for(int f=1;f<7;f++){//front right hor box
				if(f<5){
					if(f==1||f==3)//front
						renderFace(renderables[texShift+3], f,shiftX,letters,bottomFrontHeigh,dxOrig,coverDepth-letters,horizHeigh,&normals, 0, floor(dxOrig/1.37f)+1, 0, 21.0f/44.0f);
					else//sides
						renderFace(renderables[texShift+3], f,shiftX,letters,bottomFrontHeigh,dxOrig,coverDepth-letters,horizHeigh,&normals, 0, ((coverDepth-letters)/1.37f), 0, 21.0f/44.0f);
				}
				else
					renderFace(renderables[texShift+3], f,shiftX,letters,bottomFrontHeigh,dxOrig,coverDepth-letters,horizHeigh,&normals, 1, floor(dxOrig/1.37f)+1, 21.0/44.0f, 1);
			}

			//XX drawRoofTriangle(renderables[texShift+3], shiftX,letters,bottomFrontHeigh+horizHeigh,dxOrig, coverDepth-letters,frontHeight-(bottomFrontHeigh+horizHeigh), &normals, textures[texShift+3],0,textures[texShift+6],true,true,floor(dxOrig/1.37f)+1.0f);//turned half
			for(int f=2;f<6;f++){//box
				if(f==2||f==4)//sides
					renderFace(renderables[texShift+3], f,shiftX,(letters+coverDepth)/2.0f,bottomFrontHeigh+horizHeigh,dxOrig,(coverDepth-letters)/2.0f,frontHeight-(bottomFrontHeigh+horizHeigh),&normals,(coverDepth-letters)/(2.0f*1.37f),(coverDepth-letters)/(1.37f), 0, 1);
				else 
					renderFace(renderables[texShift+5], f,shiftX,(letters+coverDepth)/2.0f,bottomFrontHeigh+horizHeigh,dxOrig,(coverDepth-letters)/2.0f,frontHeight-(bottomFrontHeigh+horizHeigh),&normals, 0, floor(dxOrig/1.37f)+1, 0, 1);
			}

			//letters
			float letterHeigh=1.60f;
			float letterWidth[8]={6.35f,6.62f,5.66f,7.62f,9.45f,16.7f,16.18f};
			float texLetterWidth[8]={284.0f,282.0f,278.0f,282.0f,339.0f,380.0f,452.0f};

			int numLetter[8]={0};
			end=false;
			float dxLetter=dxOrig;
			int numLetters=0;
			while(end==false){
				for(int s=0;s<8;s++){
					if(dxLetter>letterWidth[s]){
						dxLetter-=letterWidth[s];
						numLetter[s]++;
						numLetters++;
					}
					else
						end=true;
				}
			}
			float spanLetter=dxLetter/numLetters;
			float shiftLetters=0;
			float texS=0.0f;
			for(int l=0;l<numLetters-1;l++){
				float lettersWidth=letterWidth[l%8]+spanLetter;
				float gap=(lettersWidth-texLetterWidth[l%8]*letterHeigh/70.0f)/2.0f;

				if((l%8)==0){
					texS=0;
				}
				for(int f=1;f<7;f++){//front right hor box
					if(f<5){
						if(f==1||f==3)//front
							renderFace(renderables[texShift+4], f,shiftLetters+gap,0,bottomFrontHeigh,texLetterWidth[l%8]*letterHeigh/70.0f,letters,horizHeigh,&normals, texS, texS+texLetterWidth[l%8]/2295.0f, 0, 1);
						else//sides
							renderFace(renderables[texShift+6], f,shiftLetters+gap,0,bottomFrontHeigh,texLetterWidth[l%8]*letterHeigh/70.0f,letters,horizHeigh,&normals, 0, floor(letters/1.37f), 0, 21.0f/44.0f);
					}
					else
						renderFace(renderables[texShift+5], f,shiftLetters+gap,0,bottomFrontHeigh,texLetterWidth[l%8]*letterHeigh/70.0f,letters,horizHeigh,&normals, 1, (texLetterWidth[l%8]*letterHeigh/70.0f/1.37f), 21.0/44.0f, 1);
				}
				texS+=texLetterWidth[l%8]/2295.0f;
				shiftLetters+=lettersWidth;
			}



			// STICKS

			float stickWidth=0.55f;
			// last
			for(int f=1;f<5;f++){//front right hor box
				renderFace(renderables[texShift+2], f, shiftX, letters, 0, stickWidth, stickWidth, bottomFrontHeigh, &normals, 1, 1);
			}
			int numSticks=floor((dxOrig-(0.4f*2))/5.6f);
			float span=(dxOrig-(0.4f*2))/(numSticks-1);//-2 fist last +1
			float stickShift=span;
			for(int s=0;s<numSticks-1;s++){//choose 0 because it will have the most number of shops
				for(int f=1;f<5;f++){//front right hor box
					renderFace(renderables[texShift+2], f,shiftX+stickShift+stickWidth/2.0f,letters,0,stickWidth,stickWidth,bottomFrontHeigh,&normals, 1, 1);
				}
				stickShift+=span;
			}
			// last
			for(int f=1;f<5;f++){//front right hor box
				renderFace(renderables[texShift+2], f,dxOrig-stickWidth,letters,0,stickWidth,stickWidth,bottomFrontHeigh,&normals, 1, 1);
			}

			renderFace(renderables[texShift], 1,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,frontHeight,&normals, float(dxOrig/backWidth)+1, 1);//back
			renderFace(renderables[texShift], 2,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,frontHeight,&normals, float((dyOrig-coverDepth)/backWidth)+1, 1);//left
			renderFace(renderables[texShift], 4,shiftX,coverDepth,0,dxOrig,dyOrig-coverDepth,frontHeight,&normals, float((dyOrig-coverDepth)/backWidth)+1, 1);//right

			//draw roof
			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig-coverDepth);
			}
			renderFace(renderables[textureNames.size() + roofTex], 5,shiftX,coverDepth,frontHeight,dxOrig,dyOrig-coverDepth,0,&normals, 1, 1);//roof

		} //sub 0

		/////////////////////////////////////////////////////////////////////////////////////////////
		// SUBTYPE 2
		int texOff=15;
		if(subType==2||subType==3){
			bool twoTops=false;
			if(dxOrig>30.0f&&stories>=2){
				twoTops=true;
			}
			int subType2Off=6;
			float storiesHeight=3.6f;
			float bottomHeightB=3.2f;
			float bottomHeightT=0.8f;
			float bottomHeight=bottomHeightB+bottomHeightT;

			float bottomT=bottomHeightT*50/110.0f;
			float shopT=bottomHeightT*729.0f/110.0f;

			float bottomB=bottomHeightB*297/329.0f;

			float top=storiesHeight*362/609.0f;
			if(subType==3){
				storiesHeight=3.2f;
				top=storiesHeight*306/448.0f;
			}

			float border=0.3f;
			float borderHeight=0.5f;


			float shiftY=0.0f;
			float shiftX=0.0f;

			if(stories<2)
				stories=2;



			//FLOOR 0
			shiftX=border;
			float cornerSize=2.0f;

			int currColor=qrand()%5;
			int initColor=currColor;//save it to use it in the last color

			//bottomB
			//front
			renderFace(renderables[texOff+subType2Off+3*(currColor)], 7,shiftX,border,0,cornerSize,cornerSize,bottomHeight, &normals, 1, 1);//corner
			int frontWindows=floor((dx-2*cornerSize-2*border)/bottomB)+1.0f;
			float windowSize=bottomB+((dx-2*cornerSize-2*border)-frontWindows*bottomB)/frontWindows;
			while(frontWindows>0){
				int windWithSameColor=3+qrand()%2;
				if(windWithSameColor>frontWindows) windWithSameColor=frontWindows;
				//brand
				if(windWithSameColor>=3){
					float shopTPos=(windowSize*(windWithSameColor)-shopT)/2;
					for(int face=2;face<7;face++){
						if(face==2||face==4){
							renderFace(renderables[texOff+subType2Off+15+1], face,shiftX+cornerSize+shopTPos,border-0.2f,bottomHeightB,shopT,0.2f,bottomHeightT,&normals, 0.1f, 1);//+1 because zara has nice side
						}
						else
							renderFace(renderables[texOff+subType2Off+15+qrand()%SUBTYPE_3_NUMSHOPS], face,shiftX+cornerSize+shopTPos,border-0.2f,bottomHeightB,shopT,0.2f,bottomHeightT,&normals, 1, 1);//block
					}
				}

				//windows and door
				renderFace(renderables[texOff+subType2Off+1+3*(currColor)], 3,shiftX+cornerSize,border,shiftY,windowSize*(windWithSameColor-1),dy-2*border,bottomHeightB,&normals, windWithSameColor-1, 1);//block
				shiftX+=windowSize*(windWithSameColor-1);
				renderFace(renderables[texOff+subType2Off+2+3*(currColor)], 3,shiftX+cornerSize,border,shiftY,windowSize,dy-2*border,bottomHeightB,&normals, 1, 1);//block
				shiftX+=windowSize;
				frontWindows-=windWithSameColor;
				if(frontWindows>0){
					int newColor=qrand()%5;
					if(newColor==currColor) currColor=(currColor+1)%5;
					else currColor=newColor;
				}

			}
			renderFace(renderables[texOff+subType2Off+3*(currColor)], 8,dx-border-cornerSize,border,0,cornerSize,cornerSize,bottomHeight,&normals, 1, 1);//corner
			shiftX=border;
			//right
			float rightShift=0.0f;
			int rightWindows=floor((dy-cornerSize-2*border)/bottomB)+1.0f;
			windowSize=bottomB+((dy-cornerSize-2*border)-rightWindows*bottomB)/rightWindows;
			while(rightWindows>0){
				int windWithSameColor=3+qrand()%2;
				if(windWithSameColor>rightWindows) windWithSameColor=rightWindows;

				renderFace(renderables[texOff+subType2Off+1+3*(currColor)], 2,shiftX,border+rightShift+cornerSize,shiftY,dx-2*border,windowSize*(windWithSameColor-1),bottomHeightB,&normals, windWithSameColor-1, 1);//block
				rightShift+=windowSize*(windWithSameColor-1);
				renderFace(renderables[texOff+subType2Off+2+3*(currColor)], 2,shiftX,border+rightShift+cornerSize,shiftY,dx-2*border,windowSize,bottomHeightB,&normals, 1, 1);//block
				rightShift+=windowSize;
				rightWindows-=windWithSameColor;
				if(rightWindows>0){
					int newColor=qrand()%5;
					if(newColor==currColor) currColor=(currColor+1)%5;
					else currColor=newColor;
				}
			}


			shiftX=border;
			//back
			renderFace(renderables[texOff+5], 1,shiftX,border,shiftY,dx-2*border,dy-2*border,bottomHeightB,&normals, floor((dx-2*cornerSize)/bottomB)+1, 1);//block

			shiftX=border;
			//left
			currColor=initColor;
			rightShift=0.0f;
			rightWindows=floor((dy-cornerSize-2*border)/bottomB)+1.0f;
			windowSize=bottomB+((dy-cornerSize-2*border)-rightWindows*bottomB)/rightWindows;
			while(rightWindows>0){
				int windWithSameColor=3+qrand()%2;
				if(windWithSameColor>rightWindows) windWithSameColor=rightWindows;

				renderFace(renderables[texOff+subType2Off+1+3*(currColor)], 4,shiftX,border+rightShift+cornerSize,shiftY,dx-2*border,windowSize*(windWithSameColor-1),bottomHeightB,&normals, windWithSameColor-1, 1);//block
				rightShift+=windowSize*(windWithSameColor-1);
				renderFace(renderables[texOff+subType2Off+2+3*(currColor)], 4,shiftX,border+rightShift+cornerSize,shiftY,dx-2*border,windowSize,bottomHeightB,&normals, 1, 1);//block
				rightShift+=windowSize;
				rightWindows-=windWithSameColor;
				if(rightWindows>0){
					int newColor=qrand()%5;
					if(newColor==currColor) currColor=(currColor+1)%5;
					else currColor=newColor;
				}
			}

			shiftY+=bottomHeightB;
			//bottomT
			renderFace(renderables[texOff], 1,shiftX,border,shiftY,dx-2*border,dy-2*border,bottomHeightT, &normals, floor(dx/bottomT)+1, 1);//block
			renderFace(renderables[texOff], 3,shiftX+cornerSize,border,shiftY,dx-2*border-2*cornerSize,dy-2*border,bottomHeightT, &normals, floor(dx/bottomT)+1, 1);//block

			renderFace(renderables[texOff], 2,shiftX,border+cornerSize,shiftY,dx-2*border,dy-2*border-cornerSize,bottomHeightT,&normals, floor(dy/bottomT)+1, 1);//block
			renderFace(renderables[texOff], 4,shiftX,border+cornerSize,shiftY,dx-2*border,dy-2*border-cornerSize,bottomHeightT,&normals, floor(dy/bottomT)+1, 1);//block

			shiftY+=bottomHeightT;

			// MIDDLE
			shiftX=0;
			for(int i=1;i<7;i++){
				if(i<5)
					renderFace(renderables[texOff+3], i,shiftX,0,shiftY,dxOrig,dyOrig,borderHeight,&normals, 1, 1);//block
				else
					renderFace(renderables[texOff+3], i,shiftX,0,shiftY,dxOrig,dyOrig,borderHeight,&normals, 0.2f, 0.2f);//block
			}

			shiftY+=borderHeight;
			////////////////////////////////////
			if(twoTops==false){ // ONE TOP
				//FLOOR 1+
				shiftX=border;
				int frontTex=texOff+2;
				int backTex=texOff+4;
				if(subType==3){
					frontTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+1;
					backTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+2;
				}
				renderFace(renderables[backTex], 1,shiftX,border,shiftY,dx-2*border,dy-2*border,storiesHeight*(stories-1),&normals, floor(5*dx/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 3,shiftX+cornerSize,border,shiftY,dx-2*border-2*cornerSize,dy-2*border,storiesHeight*(stories-1),&normals, floor((dx-2*border-2*cornerSize)/top)+1,stories-1);//block

				renderFace(renderables[frontTex], 2,shiftX,border+cornerSize,shiftY,dx-2*border,dy-2*border-cornerSize,storiesHeight*(stories-1),&normals, floor((dy-2*border-cornerSize)/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 4,shiftX,border+cornerSize,shiftY,dx-2*border,dy-2*border-cornerSize,storiesHeight*(stories-1),&normals, floor((dy-2*border-cornerSize)/top)+1, stories-1);//block

				renderFace(renderables[frontTex], 7,shiftX,border,shiftY,cornerSize,cornerSize,storiesHeight*(stories-1), &normals, 1, stories-1);//corner
				renderFace(renderables[frontTex], 8,dx-border-cornerSize,border,shiftY,cornerSize,cornerSize,storiesHeight*(stories-1), &normals, 1, stories-1);//corner


				shiftY+=storiesHeight*(stories-1);

				// TOP EDGE
				//draw roof
				if(roofTex==-1){
					roofTex=bestTextureForRoof(dxOrig-2*border,dyOrig-2*border);
				}
				int boderTex=texOff+1;
				if(subType==3){
					boderTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+0;
				}
				shiftX=0;
				for(int i=1;i<7;i++){
					if(i==5) 
						renderFace(renderables[textureNames.size() + roofTex], i,shiftX,0,shiftY,dxOrig,dyOrig,borderHeight, &normals, 1, 1);//roof
					else if(i<5)
						renderFace(renderables[boderTex], i,shiftX,0,shiftY,dxOrig,dyOrig,borderHeight, &normals, dxOrig*(i%2==1)+dxOrig*(i%2==0), 1);//block
					else
						renderFace(renderables[boderTex], i,shiftX,0,shiftY,dxOrig,dyOrig,borderHeight, &normals, 0.2f, 0.2f);//block
				}
			}
			else{// 
				////////////////////////////////////
				// TWO TOP

				//FLOOR 1+
				//left
				int typeLeft=qrand()%2;
				shiftX=border;
				storiesHeight=3.6f;
				top=storiesHeight*362/609.0f;
				int frontTex=texOff+2;
				int backTex=texOff+4;
				if(typeLeft==0){
					frontTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+1;
					backTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+2;
					storiesHeight=3.2f;
					top=storiesHeight*306/448.0f;
				}
				renderFace(renderables[backTex], 1,shiftX,border,shiftY,(dx-2*border)/2.0f,dy-2*border,storiesHeight*(stories-1),&normals, floor(2.5*dx/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 3,shiftX+cornerSize,border,shiftY,(dx-2*border-2*cornerSize)/2.0f,dy-2*border,storiesHeight*(stories-1),&normals, floor((dx/2-border-cornerSize)/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 2,shiftX,border,shiftY,(dx-2*border)/2.0f,dy-2*border,storiesHeight*(stories-1),&normals, floor((dy-2*border-cornerSize)/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 4,shiftX,border+cornerSize,shiftY,(dx-2*border)/2.0f,dy-2*border-cornerSize,storiesHeight*(stories-1),&normals, floor((dy-2*border-cornerSize)/top)+1,stories-1);//block

				renderFace(renderables[frontTex], 7,shiftX,border,shiftY,cornerSize,cornerSize,storiesHeight*(stories-1),&normals, 1, stories-1);//corner

				shiftY+=storiesHeight*(stories-1);
				// TOP EDGE
				//draw roof
				if(roofTex==-1){
					roofTex=bestTextureForRoof((dxOrig)/2.0f,dyOrig-2*border);
				}
				int boderTex=texOff+1;
				if(typeLeft==0){
					boderTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+0;
				}
				shiftX=0;
				for(int i=1;i<7;i++){
					if(i==5) 
						renderFace(renderables[textureNames.size() + roofTex], i,shiftX,0,shiftY,(dxOrig)/2.0f,dyOrig,borderHeight, &normals, 1, 1);//roof
					else if(i<5)
						renderFace(renderables[boderTex], i,shiftX,0,shiftY,(dxOrig)/2.0f,dyOrig,borderHeight, &normals, dxOrig*(i%2==1)+dxOrig*(i%2==0), 1);//block
					else
						renderFace(renderables[boderTex], i,shiftX,0,shiftY,(dxOrig)/2.0f,dyOrig,borderHeight, &normals, 0.2f, 0.2f);//block
				}
				shiftY-=storiesHeight*(stories-1);
				//////////////////////////
				//right
				storiesHeight=3.6f;
				top=storiesHeight*362/609.0f;
				frontTex=texOff+2;
				backTex=texOff+4;
				if(typeLeft==1){
					frontTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+1;
					backTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+2;
					storiesHeight=3.2f;
					top=storiesHeight*306/448.0f;
				}
				shiftX+=(dx)/2.0f;
				renderFace(renderables[backTex], 1,shiftX,border,shiftY,(dx-2*border)/2.0f,dy-2*border,storiesHeight*(stories-1),&normals, floor(2.5f*dx/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 3,shiftX,border,shiftY,(dx-2*border-2*cornerSize)/2.0f,dy-2*border,storiesHeight*(stories-1),&normals, floor((dx/2.0f-border-cornerSize)/top)+1, stories-1);//block

				renderFace(renderables[frontTex], 2,shiftX,border+cornerSize,shiftY,(dx-2*border)/2.0f,dy-2*border-cornerSize,storiesHeight*(stories-1), &normals, floor((dy-2*border-cornerSize)/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 4,shiftX,border,shiftY,(dx-2*border)/2.0f,dy-2*border,storiesHeight*(stories-1), &normals, floor((dy-2*border-cornerSize)/top)+1, stories-1);//block
				renderFace(renderables[frontTex], 8,dx-border-cornerSize,border,shiftY,cornerSize,cornerSize,storiesHeight*(stories-1), &normals, 1, stories-1);//corner


				shiftY+=storiesHeight*(stories-1);
				// TOP EDGE
				//draw roof
				if(roofTex==-1){
					roofTex=bestTextureForRoof((dxOrig)/2.0f,dyOrig-2*border);
				}
				boderTex=texOff+1;
				if(typeLeft==1){
					boderTex=texOff+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+subType2Off+0;
				}

				for(int i=1;i<7;i++){
					if(i==5) 
						renderFace(renderables[textureNames.size() + roofTex], i, shiftX, 0, shiftY, dxOrig*0.5, dyOrig, borderHeight, &normals, 1, 1);//roof
					else if(i<5)
						renderFace(renderables[boderTex], i, shiftX, 0, shiftY, dxOrig*0.5, dyOrig,borderHeight, &normals, dxOrig*(i%2==1)+dxOrig*(i%2==0), 1);//block
					else
						renderFace(renderables[boderTex], i, shiftX, 0, shiftY, dxOrig*0.5, dyOrig,borderHeight, &normals, 0.2, 0.2);//block
				}

			}
		}//sub 2,3
		if(subType==4||subType==5||subType==6){
			const int numShops=3;//num of types of shops

			float coverDepth=2.0f;
			float shopHeigh=5.4f;
			int texNum[]={0,6,8};
			int texNumTop[]={4,7,9};
			int shopNum=subType-4;//to be 0-2//qrand()%numShops;

			float front[]={shopHeigh*751/400.0f,shopHeigh*665/600.0f,shopHeigh*760/437.0f};
			float coverH[]={2.4f,2.1f,2.0f};//height where the cover starts
			float coverSize[]={1.0f,0.9f,0.8f};//height of the cover

			float currentDx=dxOrig;
			int whichShop[numShops]={0};
			int order=1;
			if(currentDx>front[shopNum]){
				whichShop[shopNum]=order;
				order++;
				currentDx-=front[shopNum];
			}
			while(currentDx>front[1]){//smallest
				int randSh = rand() % numShops;
				if(currentDx>front[randSh]&&whichShop[randSh]==0){
					whichShop[randSh]=order;
					order++;
					currentDx-=front[randSh];
				}
				if(order>numShops||((currentDx<front[0]) || (currentDx<front[2]))){//finish if we had 1 and there is not room for the others
					break;
				}
			}
			float shiftX=currentDx/2.0f;
			int processOrder=1;

			while(true){
				shopNum=-1;
				for(int i=0;i<numShops;i++){
					if(whichShop[i]==processOrder){
						shopNum=i;
						processOrder++;
						break;
					}
				}
				if(shopNum==-1){
					break;
				}
				qsrand(width);
				subType=qrand()%3+4;
				if(subType==4){

					int offTexSub4=21+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+3;

					float shiftY=0.0f;
					float shopHeigh=5.4f;
					float back1=shopHeigh*400/505.0f;

					roofTex=bestTextureForRoof(front[shopNum],dyOrig);
						
					renderFace(renderables[offTexSub4+texNum[shopNum]], 3, shiftX, coverDepth, shiftY, front[shopNum], dyOrig-coverDepth, shopHeigh, &normals, 1, 1);//front
					renderFace(renderables[offTexSub4+1], 1, shiftX, coverDepth, shiftY, front[shopNum], dyOrig-coverDepth,shopHeigh, &normals, floor(front[shopNum]/back1)+1, 1);//back
					//door
					int randPos=qrand()%(int)floor(front[shopNum]/back1);
					for(int fac=1;fac<6;fac++){
						if(fac==1)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh, &normals, 1, 1);//sides
						if(fac==2||fac==4)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh, &normals, 0.05f, 1);//sides
						if(fac==5)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh, &normals, 1, -0.05f);//sides
					}


					renderFace(renderables[offTexSub4+1], 2,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, floor(dyOrig/back1)+1, 1);//sides
					renderFace(renderables[offTexSub4+2], 4,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, floor(dyOrig/back1)+1, 1);//sides


					renderFace(renderables[textureNames.size() + roofTex], 5,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, 1, 1);//sides
					shiftX+=front[shopNum];
				}//sub 4

				if(subType==5){

					int offTexSub4=21+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+3;
					float shiftY=0.0f;
					float roofHeight=0.8f;
					float shopHeightB=shopHeigh-roofHeight;


					float back1=shopHeightB*400/505.0f;


					roofTex=bestTextureForRoof(front[shopNum],dyOrig);


					renderFace(renderables[offTexSub4+texNum[shopNum]], 3,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeightB, &normals, 1, 1-(roofHeight/shopHeigh));//front
					renderFace(renderables[offTexSub4+1], 1,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeightB,&normals, floor(front[shopNum]/back1)+1, 1-(roofHeight/shopHeigh));//back

					//roof
					//XX drawRoofTriangle(shiftX,coverDepth,shopHeightB,front[shopNum],(dyOrig-coverDepth)/2.0f,roofHeight,&normals,textures[offTexSub4+texNumTop[shopNum]],0,textures[offTexSub4+5],false);//dyOrig/1.5f,dxOrig/1.0f);//1,1 workes great 1.5f because the tex is 1.5x1.0
					//XX drawRoofTriangle(shiftX,coverDepth+(dyOrig-coverDepth)/2.0f,shopHeightB,front[shopNum],(dyOrig-coverDepth)/2.0f,roofHeight,&normals,textures[offTexSub4+5],0,textures[offTexSub4+5],false);//dyOrig/1.5f,dxOrig/1.0f);//1,1 workes great 1.5f because the tex is 1.5x1.0
					//door
					int randPos=qrand()%(int)floor(front[shopNum]/back1);
					for(int fac=1;fac<5;fac++){
						if(fac==1)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeightB, &normals, 1, 1);//sides
						if(fac==2||fac==4)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeightB, &normals, 0.05f, 1);//sides
					}


					renderFace(renderables[offTexSub4+1], 2,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeightB,&normals, floor(dyOrig/back1)+1, 1);//sides
					renderFace(renderables[offTexSub4+2], 4,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeightB,&normals, floor(dyOrig/back1)+1, 1);//sides


					renderFace(renderables[textureNames.size() + roofTex], 5,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeightB,&normals, 1, 1);//sides
					shiftX+=front[shopNum];
				}//sub 5

				if(subType==6){

					int offTexSub4=21+SUBTYPE_3_COLORS*3+SUBTYPE_3_NUMSHOPS+3;
					float shiftY=0.0f;
					float shopHeigh=5.4f;
					float front1=shopHeigh*751/400.0f;
					float back1=shopHeigh*400/505.0f;


					roofTex=bestTextureForRoof(front1,dyOrig);


					renderFace(renderables[offTexSub4+texNum[shopNum]], 3,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh, &normals, 1, 1);//front
					renderFace(renderables[offTexSub4+1], 1,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh, &normals, floor(front1/back1)+1, 1);//back
					//door
					int randPos=qrand()%(int)floor(front[shopNum]/back1);
					for(int fac=1;fac<6;fac++){
						if(fac==1)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh,&normals, 1, 1);//sides
						if(fac==2||fac==4)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh,&normals, 0.05f, 1);//sides
						if(fac==5)
							renderFace(renderables[offTexSub4+3], fac,shiftX+(randPos)*back1,dyOrig,shiftY,back1,0.2f,shopHeigh,&normals, 1, -0.05f);//sides
					}


					renderFace(renderables[offTexSub4+1], 2,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, floor(dyOrig/back1)+1, 1);//sides
					renderFace(renderables[offTexSub4+2], 4,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, floor(dyOrig/back1)+1, 1);//sides


					renderFace(renderables[textureNames.size() + roofTex], 5,shiftX,coverDepth,shiftY,front[shopNum],dyOrig-coverDepth,shopHeigh,&normals, 1, 1);//sides

					//cover
					int coverColor=qrand()%SUBTYPE_6_COVERS;
					//XX drawRoofTriangle(shiftX,0,coverH[shopNum],front[shopNum], 2*coverDepth,coverSize[shopNum],&normals, textures[offTexSub4+10+coverColor],0,textures[offTexSub4+10+coverColor],true,true,front[shopNum]);//turned half
					shiftX+=front[shopNum];
				}//sub 6
			}
		}
	}
}
