/*********************************************************************************
*
*		MTC Project - Geometry Project - OfficeRenderer Class
*
*
*		@desc Class containing the renderer for office buildings
*		@author igarciad
*
*********************************************************************************/

#include "Office.h"

namespace pb {
	#define NUM_SUBTYPE 10

	std::vector<ucore::Texture*> Office::textures;
	bool Office::texturesLoaded = false;
	std::vector<QString> Office::textureNames;

	/*********************************************************************************
	*	OfficeRenderer constructor
	*		Constructor for office renderers.  Initializes instance variables common to 
	*		all office renderers.
	*********************************************************************************/
	Office::Office(const ucore::Loop3D& footprint, int numStories, float height) : AbstractParametricBuilding(footprint, numStories, height) {
		if (textureNames.size() == 0) {
			for (int i = 1; i <= NUM_SUBTYPE; ++i) {
				textureNames.push_back("data/buildings/OF/window_" + QString::number(i) + ".jpg");//0
				textureNames.push_back("data/buildings/OF/window2_" + QString::number(i) + ".jpg");//1
				textureNames.push_back("data/buildings/OF/span_" + QString::number(i) + ".jpg");//2
				textureNames.push_back("data/buildings/OF/cspan_" + QString::number(i) + ".jpg");//3
			}
		}

		if (houseRoofTextures.size() > 0)
			roofTex = rand() % (houseRoofTextures.size() - 1);

		subType=-1;
		float subMins[NUM_SUBTYPE] ={10.5f, 3.5f, 3.5f, 0, 0, 0, 0, 36.0f, 36.0f, 36.0f}; // fill with subtype minimum widths

		while (subType == -1) {	
			subType = rand() % NUM_SUBTYPE;
			if (width < subMins[subType] || depth < subMins[subType]) subType = -1;
		}

		if (width > 35 && depth > 35) subType = rand() % 3 + 7;

		//scale logic
		float minDim = 3.0f;
		if (width < minDim || depth < minDim) {
			scale = min(width, depth) / minDim;
			width /= scale;
			depth /= scale;
		}
	}

	/*********************************************************************************
	*	OfficeRenderer destructor
	*		Destructor for office building renderers.  Nothing to do.
	*********************************************************************************/
	Office::~Office() {
	}

	/*********************************************************************************
	*	InitializeTextures
	*		Loads the textures for office buildings.
	*********************************************************************************/
	void Office::initializeTextures(ucore::TextureManager* textureManager) {
		if (!texturesLoaded) {
			AbstractParametricBuilding::initializeTextures(&textureNames, &textures, textureManager);
			texturesLoaded = true;
			AbstractParametricBuilding::loadRoofTextures(textureManager);
			roofTexturesLoaded = true;
		}

	}

	void Office::_generateMeshVertices() {
		/*
		float dx= width;
		float dy= depth;

		if(dx<0)dx*=-1.0f;
		if(dy<0)dy*=-1.0f;

		int curStories=stories;
		if(curStories<2)
			curStories=2;





		if(subType==0)
		{

			float dyOrig=dy;
			float dxOrig=dx;
			float leftWindow=3.5f;
			float leftHeigh=4.6;

			float curvedWindow=7.0f;

			float shiftY;
			float shiftX;

			int numleftWindows,i;

			float length;

			ucore::Polygon3D roof[4];
			ucore::Polygon3D* currRoof;

			if(curStories<2) curStories=2;

			// DRAW
			for(int f=0;f<4;f++){
				shiftY=0;
				if(f==0){
					length=dxOrig;
					currRoof=&roof[0];
				}
				if(f==1){
					length=dxOrig;
					currRoof=&roof[2];
				}
				if(f==2){
					length=dyOrig;
					currRoof=&roof[3];
				}
				if(f==3){
					length=dyOrig;
					currRoof=&roof[1];
				}

				//calculate the number of windows and blocks

				numleftWindows=1;
				length-=(leftWindow);
				if(length>leftWindow){

					numleftWindows+=floor(length/(leftWindow));
					length-=floor(length/(leftWindow))*leftWindow;
				}

				//transform view to the proper face
				if(f==1){
					glPushMatrix();
					glTranslatef(dxOrig,dyOrig,0);
					glRotatef(180.0f,0,0,1.0f);
				}
				if(f==2){
					glPushMatrix();
					glTranslatef(0,dyOrig,0);
					glRotatef(270.0f,0,0,1.0f);
				}
				if(f==3){
					glPushMatrix();
					glTranslatef(dxOrig,0,0);
					glRotatef(90.0f,0,0,1.0f);
				}

				shiftX=0;

				float spanWin=length;
				//LEFT

				if(f==0)
				{

					renderFace(7,new QVector3D(0,curvedWindow,shiftY),curvedWindow/2.0f,curvedWindow/2.0f,leftHeigh,&normals,&textures,subType*4+1,1.0f,1.0f);//curved1
					for(int face=1;face<5;face++){
						renderFace(face,new QVector3D(0,curvedWindow,shiftY),0.5f,0.5f,leftHeigh,&normals,&textures,subType*4+3,1.0f,1.0f);//column

					}
					renderFace(7,new QVector3D(0,3*curvedWindow/4.0f,shiftY+leftHeigh),curvedWindow/2.0f,curvedWindow/4.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+0,1.0f,1.0f*(curStories-1));//curved1
					currRoof->contour.push_back(QVector3D(shiftX,curvedWindow,shiftY));
					shiftX+=curvedWindow/2.0f;

					renderFace(7,new QVector3D(shiftX,curvedWindow/2.0f,shiftY),curvedWindow/2.0f,curvedWindow/2.0f,leftHeigh,&normals,&textures,subType*4+1,0.75f,1.0f);//curved2
					renderFace(7,new QVector3D(shiftX,curvedWindow/2.0f,shiftY+leftHeigh),curvedWindow/4.0f,curvedWindow/4.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+0,0.75f,1.0f*(curStories-1));//curved2
					currRoof->contour.push_back(QVector3D(shiftX,3*curvedWindow/4.0f,shiftY));
					shiftX+=curvedWindow/4.0f;

					renderFace(7,new QVector3D(shiftX,0,shiftY+leftHeigh),curvedWindow/4.0f,curvedWindow/2.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+0,1.0f,1.0f*(curStories-1));//curved3
					currRoof->contour.push_back(QVector3D(shiftX,curvedWindow/2.0f,shiftY));
					shiftX+=curvedWindow/4.0f;
					renderFace(7,new QVector3D(shiftX,0,shiftY),curvedWindow/2.0f,curvedWindow/2.0f,leftHeigh,&normals,&textures,subType*4+1,1.0f,1.0f);//curved3
					for(int face=1;face<5;face++){
						renderFace(face,new QVector3D(shiftX,0,shiftY),0.5f,0.5f,leftHeigh,&normals,&textures,subType*4+3,1.0f,1.0f);//column

					}
					//	spanWin-=curvedWindow;
					numleftWindows-=2;
					renderFace(3,new QVector3D(shiftX,0,shiftY+leftHeigh),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+0,1.0f,1.0f*(curStories-1));//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}
				if(f==3 || f==1)
				{
					renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}
				if(f==2)
				{
					numleftWindows-=2;

				}


				if(numleftWindows>1)
					spanWin=spanWin/(numleftWindows-1);
				else
				{
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+2,1.0,1.0f*curStories);//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
				}


				for(i=1;i<numleftWindows;i++){
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+2,1.0,1.0f*curStories);//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
					renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}
				if(f==2)
				{
					renderFace(3,new QVector3D(shiftX,0,shiftY+leftHeigh),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+0,1.0f,1.0f*(curStories-1));//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}

				shiftY+=leftHeigh*curStories;

				if(f!=0){
					glPopMatrix();

				}
			}

			//draw roof
			QVector3D *p;
			float x,y;
			int j;
			MTC::misctools::Polygon3D allRoof;
			for(int i=0;i<4;i++){
				for(j=0;j<roof[i].contour.size();j++){
					p=&roof[i].contour.at(j);
					if(i!=0){
						x=p->x();
						y=p->y();
						if(i==1||i==3){//swap x and y
							float swap=x;
							x=y;
							y=swap;
							if(i==1){
								x=dxOrig-x;
							}
						}
						if(i==2){
							y=dyOrig-y;
							x=dxOrig-x;
						}
						if(i==3){
							y=dyOrig-y;
						}
						p->setX(x);
						p->setY(y);
					}
					p->setZ(shiftY);
				}

			}
			float permiter[4]={0,0,0,0};
			for(int i=0;i<4;i++){
				for(int j=0;j<roof[i].contour.size();j++){
					allRoof.contour.push_back(roof[i].contour.at(j));
					permiter[i]+=(roof[i].contour.at(j)-roof[i].contour.at((j+1)%roof[i].contour.size())).length();
				}
			}

			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig);
			}

			renderFlatRoof(&allRoof,&roofTextures,roofTex);
			glPopMatrix();
			return;
		}

		if(subType==1)
		{

			float dyOrig=dy;
			float dxOrig=dx;
			float leftWindow=3.5f;
			float leftHeigh=5.6;
			float frstHeight=4.6f;
			float roofHeight=1.76f;

			loadBuildingTransformation();

			float shiftY;
			float shiftX;

			int numleftWindows,i;

			float length;

			MTC::misctools::Polygon3D roof[4];
			MTC::misctools::Polygon3D* currRoof;

			if(curStories<2)
				curStories=2;

			// DRAW
			for(int f=0;f<4;f++){
				shiftY=0;
				if(f==0){
					length=dxOrig;
					currRoof=&roof[0];
				}
				if(f==1){
					length=dxOrig;
					currRoof=&roof[2];
				}
				if(f==2){
					length=dyOrig;
					currRoof=&roof[3];
				}
				if(f==3){
					length=dyOrig;
					currRoof=&roof[1];
				}

				//calculate the number of windows and blocks

				numleftWindows=1;
				length-=(leftWindow);
				if(length>leftWindow){

					numleftWindows+=floor(length/(leftWindow));
					length-=floor(length/(leftWindow))*leftWindow;
				}



				//transform view to the proper face
				if(f==1){
					glPushMatrix();
					glTranslatef(dxOrig,dyOrig,0);
					glRotatef(180.0f,0,0,1.0f);
				}
				if(f==2){
					glPushMatrix();
					glTranslatef(0,dyOrig,0);
					glRotatef(270.0f,0,0,1.0f);
				}
				if(f==3){
					glPushMatrix();
					glTranslatef(dxOrig,0,0);
					glRotatef(90.0f,0,0,1.0f);
				}

				shiftX=0;

				float spanWin=length;
				//LEFT

				renderFace(3,new QVector3D(shiftX,0,leftHeigh*(curStories-1)+frstHeight),leftWindow,1.0f,roofHeight,&normals,&textures,subType*4+1,1.0f,0.312f);//window roof
				renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,1.0f);//window 1st
				renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+1,1.0f,1.0f*(curStories-1));//window
				currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
				shiftX+=leftWindow;



				if(numleftWindows>1)
					spanWin=spanWin/(numleftWindows-1);
				else
				{
					renderFace(3,new QVector3D(shiftX,0,leftHeigh*(curStories-1)+frstHeight),spanWin,1.0f,roofHeight,&normals,&textures,subType*4+2,1.0f,0.312f);//window roof
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,1.0f);//windowSpan
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),spanWin,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+2,1.0,1.0f*(curStories-1));//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
				}


				for(i=1;i<numleftWindows;i++){
					renderFace(3,new QVector3D(shiftX,0,leftHeigh*(curStories-1)+frstHeight),spanWin,1.0f,roofHeight,&normals,&textures,subType*4+2,1.0f,0.312f);//window roof
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,1.0f);//windowSpan
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),spanWin,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+2,1.0,1.0f*(curStories-1));//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
					renderFace(3,new QVector3D(shiftX,0,leftHeigh*(curStories-1)+frstHeight),leftWindow,1.0f,roofHeight,&normals,&textures,subType*4+1,1.0f,0.312f);//window roof
					renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+3*(i%2),1.0f,1.0f);//window
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+1,1.0f,1.0f*(curStories-1));//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}


				shiftY+=leftHeigh*(curStories-1)+frstHeight+roofHeight;

				if(f!=0){
					glPopMatrix();

				}
			}

			//draw roof
			QVector3D *p;
			float x,y;
			int j;
			MTC::misctools::Polygon3D allRoof;
			for(int i=0;i<4;i++){
				for(j=0;j<roof[i].contour.size();j++){
					p=&roof[i].contour.at(j);
					if(i!=0){
						x=p->x();
						y=p->y();
						if(i==1||i==3){//swap x and y
							float swap=x;
							x=y;
							y=swap;
							if(i==1){
								x=dxOrig-x;
							}
						}
						if(i==2){
							y=dyOrig-y;
							x=dxOrig-x;
						}
						if(i==3){
							y=dyOrig-y;
						}
						p->setX(x);
						p->setY(y);
					}
					p->setZ(shiftY);
				}

			}
			float permiter[4]={0,0,0,0};
			for(int i=0;i<4;i++){
				for(int j=0;j<roof[i].contour.size();j++){
					allRoof.contour.push_back(roof[i].contour.at(j));
					permiter[i]+=(roof[i].contour.at(j)-roof[i].contour.at((j+1)%roof[i].contour.size())).length();
				}
			}

			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig);
			}

			renderFlatRoof(&allRoof,&roofTextures,roofTex);
			glPopMatrix();
			return;
		}


		if(subType==2)
		{

			float dyOrig=dy;
			float dxOrig=dx;
			float leftWindow=3.5f;
			float leftHeigh=2.6;
			float frstHeight=4.6f;
			float roofHeight=0.8f;

			loadBuildingTransformation();

			float shiftY;
			float shiftX;


			int numleftWindows,i;

			float length;

			MTC::misctools::Polygon3D roof[4];
			MTC::misctools::Polygon3D* currRoof;

			if(curStories<2)
				curStories=2;

			// DRAW
			for(int f=0;f<4;f++){
				shiftY=0;
				if(f==0){
					length=dxOrig;
					currRoof=&roof[0];
				}
				if(f==1){
					length=dxOrig;
					currRoof=&roof[2];
				}
				if(f==2){
					length=dyOrig;
					currRoof=&roof[3];
				}
				if(f==3){
					length=dyOrig;
					currRoof=&roof[1];
				}

				//calculate the number of windows and blocks

				numleftWindows=1;
				length-=(leftWindow);
				if(length>leftWindow){

					numleftWindows+=floor(length/(leftWindow));
					length-=floor(length/(leftWindow))*leftWindow;
				}



				//transform view to the proper face
				if(f==1){
					glPushMatrix();
					glTranslatef(dxOrig,dyOrig,0);
					glRotatef(180.0f,0,0,1.0f);
				}
				if(f==2){
					glPushMatrix();
					glTranslatef(0,dyOrig,0);
					glRotatef(270.0f,0,0,1.0f);
				}
				if(f==3){
					glPushMatrix();
					glTranslatef(dxOrig,0,0);
					glRotatef(90.0f,0,0,1.0f);
				}

				shiftX=0;

				float spanWin=length;
				//LEFT

				renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,1.0f);//window 1st
				renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+1,1.0f,1.0f*(curStories-1));//window
				currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
				shiftX+=leftWindow;



				if(numleftWindows>1)
					spanWin=spanWin/(numleftWindows-1);
				else
				{
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,1.0f);//windowSpan
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),spanWin,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+2,1.0,1.0f*(curStories-1));//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
				}


				for(i=1;i<numleftWindows;i++){
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,1.0f);//windowSpan
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),spanWin,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+2,1.0,1.0f*(curStories-1));//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=spanWin;
					renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,1.0f);//window
					renderFace(3,new QVector3D(shiftX,0,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*(curStories-1),&normals,&textures,subType*4+1,1.0f,1.0f*(curStories-1));//window
					currRoof->contour.push_back(QVector3D(shiftX,0,shiftY));
					shiftX+=leftWindow;
				}

				renderFace(3,new QVector3D(0,0,leftHeigh*(curStories-1)+frstHeight),shiftX,1.0f,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

				shiftY+=leftHeigh*(curStories-1)+frstHeight+roofHeight;

				if(f!=0){
					glPopMatrix();

				}
			}

			//draw roof
			QVector3D *p;
			float x,y;
			int j;
			MTC::misctools::Polygon3D allRoof;
			for(int i=0;i<4;i++){
				for(j=0;j<roof[i].contour.size();j++){
					p=&roof[i].contour.at(j);
					if(i!=0){
						x=p->x();
						y=p->y();
						if(i==1||i==3){//swap x and y
							float swap=x;
							x=y;
							y=swap;
							if(i==1){
								x=dxOrig-x;
							}
						}
						if(i==2){
							y=dyOrig-y;
							x=dxOrig-x;
						}
						if(i==3){
							y=dyOrig-y;
						}
						p->setX(x);
						p->setY(y);
					}
					p->setZ(shiftY);
				}

			}
			float permiter[4]={0,0,0,0};
			for(int i=0;i<4;i++){
				for(int j=0;j<roof[i].contour.size();j++){
					allRoof.contour.push_back(roof[i].contour.at(j));
					permiter[i]+=(roof[i].contour.at(j)-roof[i].contour.at((j+1)%roof[i].contour.size())).length();
				}
			}

			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig);
			}

			drawRoof(new QVector3D(0,0,shiftY),dx,shiftX,2.0f,&normals,&roofTextures,roofTex,roofTex);
			glPopMatrix();
			return;
		}

		if(subType==3 || subType==4 || subType==5 || subType==6)
		{

			float dyOrig=dy;
			float dxOrig=dx;



			float baseHeigh=4.67f;
			float backWidth=baseHeigh*30/100;
			if(curStories<2)
				curStories=2;

			// DRAW


			float front1=baseHeigh*145/100;//2
			float front2=baseHeigh*70/100; //0
			float front3=baseHeigh*196/100; //1
			float side1=baseHeigh*101/100;//0
			float side2=baseHeigh*495/100;//1

			// BLOCKS NUMBER
			int numBlockX=1;//=(rand())%((int)floor(dxOrig/(front2))+1);//max num from 1 to max TODO -> rand() (int)(floor(dxOrig)
			int numBlockY=1;//=(rand())%((int)floor(dyOrig/side1)+1);//max num from 1 to max TODO -> rand() (int)(floor(dyOrig)

			int bdgProb=rand()%8;

			if(bdgProb==4)
			{
				if(dx>dy)
					numBlockX=2;
				else
					numBlockY=2;
			}
			else if(bdgProb==5)
			{
				if(dx>dy)
					numBlockX=3;
				else
					numBlockY=3;
			}
			else if(bdgProb==7 || bdgProb==6)
			{
				numBlockX=numBlockY=2;
			}

			float blockX=dxOrig/numBlockX;
			float blockY=dyOrig/numBlockY;

			float blockXOrig=blockX;
			float blockYOrig=blockY;

			int buildsToRemoved=min(numBlockX,numBlockY)-1;
			if(min(numBlockX,numBlockY)<2)buildsToRemoved=0;
			int indRemoved=rand()%(numBlockX*numBlockY-buildsToRemoved+1);
			int backTex=NUM_SUBTYPE*5+rand()%2;
			//FRONT NUMBERS

			int numFront1=0;
			int numFront2=0;
			int numFront3=0;
			if(blockX<front1){
				numFront2=floor(blockX/front2)+1;
				blockX-=numFront2*front2;
			}else{
				if(blockX<front3){
					numFront1=1;
					blockX-=numFront1*front1;
				}else{
					while(blockX>front2){
						if(blockX>front3){
							numFront3++;
							blockX-=front3;
						}
						if(blockX>front1){
							numFront1++;
							blockX-=front1;
						}
						if(blockX>front2){
							numFront2++;
							blockX-=front2;
						}
					}
				}
			}
			//SIDE NUMBERS

			int numSide1;
			int numSide2=0;
			if(blockY>side2){
				numSide2=1;
				blockY-=side2;
			}
			numSide1=floor(blockY/side1)+1;
			blockY-=numSide1*side1;

			loadBuildingTransformation();

			//if there is just one building use the whole space
			float xShift=0;
			float yShift;
			float frontShift;
			int randstories=curStories;
			int intrusion=rand()%3;
			if(numFront1==0)
				intrusion=0;

			for(int bX=0;bX<numBlockX;bX++){
				yShift=0;
				for(int bY=0;bY<numBlockY;bY++){
					if((bX*numBlockY+bY)>=indRemoved && (bX*numBlockY+bY)<(indRemoved+buildsToRemoved)){//skip ramdonly
						yShift+=blockYOrig;
						continue;
					}
					frontShift=0;
					//11 front
					float span=blockX/(numFront1+numFront2+numFront3);
					for(int i=0;i<numFront1;i++){
						renderFace(3,new QVector3D(xShift+frontShift,yShift,0),front1+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
						renderFace(1,new QVector3D(xShift+frontShift,yShift,0),front1+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st

						renderFace(3,new QVector3D(xShift+frontShift,yShift,baseHeigh),front1+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.5,1.0f*(randstories-1));
						renderFace(1,new QVector3D(xShift+frontShift,yShift,baseHeigh),front1+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.5,1.0f*(randstories-1));

						frontShift+=front1+span;
					}
					for(int i=0;i<numFront2;i++){
						if(intrusion==0)
						{
							renderFace(3,new QVector3D(xShift+frontShift,yShift,0),front2+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(1,new QVector3D(xShift+frontShift,yShift,0),front2+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st

							renderFace(3,new QVector3D(xShift+frontShift,yShift,baseHeigh),front2+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
							renderFace(1,new QVector3D(xShift+frontShift,yShift,baseHeigh),front2+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
						}
						else if(intrusion==1)
						{
							renderFace(8,new QVector3D(xShift+frontShift,yShift,0),(front2+span)/2,(front2+span)/2,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(7,new QVector3D(xShift+frontShift+(front2+span)/2,yShift,0),(front2+span)/2,(front2+span)/2,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(1,new QVector3D(xShift+frontShift,yShift,0),front2+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st

							renderFace(8,new QVector3D(xShift+frontShift,yShift,baseHeigh),(front2+span)/2,(front2+span)/2,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.5,1.0f*(randstories-1));
							renderFace(7,new QVector3D(xShift+frontShift+(front2+span)/2,yShift,baseHeigh),(front2+span)/2,(front2+span)/2,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.5,1.0f*(randstories-1));
							renderFace(1,new QVector3D(xShift+frontShift,yShift,baseHeigh),front2+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.5,1.0f*(randstories-1));
						}
						else
						{
							renderFace(8,new QVector3D(xShift+frontShift,yShift,0),(front2+span)/4,(front2+span)/4,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(7,new QVector3D(xShift+frontShift+(front2+span)/4,yShift,0),(front2+span)/4,(front2+span)/4,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(8,new QVector3D(xShift+frontShift+(front2+span)/2,yShift,0),(front2+span)/4,(front2+span)/4,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(7,new QVector3D(xShift+frontShift+3*(front2+span)/4,yShift,0),(front2+span)/4,(front2+span)/4,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
							renderFace(1,new QVector3D(xShift+frontShift,yShift,0),front2+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st

							renderFace(8,new QVector3D(xShift+frontShift,yShift,baseHeigh),(front2+span)/4,(front2+span)/4,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
							renderFace(7,new QVector3D(xShift+frontShift+(front2+span)/4,yShift,baseHeigh),(front2+span)/4,(front2+span)/4,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
							renderFace(8,new QVector3D(xShift+frontShift+(front2+span)/2,yShift,baseHeigh),(front2+span)/4,(front2+span)/4,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
							renderFace(7,new QVector3D(xShift+frontShift+3*(front2+span)/4,yShift,baseHeigh),(front2+span)/4,(front2+span)/4,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f,1.0f*(randstories-1));
							renderFace(1,new QVector3D(xShift+frontShift,yShift,baseHeigh),front2+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,1.0f,1.0f*(randstories-1));
						}
						frontShift+=front2+span;
					}
					for(int i=0;i<numFront3;i++){
						renderFace(3,new QVector3D(xShift+frontShift,yShift,0),front3+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st
						renderFace(1,new QVector3D(xShift+frontShift,yShift,0),front3+span,blockYOrig,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);//1st

						renderFace(3,new QVector3D(xShift+frontShift,yShift,baseHeigh),front3+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,1.0f,1.0f*(randstories-1));
						renderFace(1,new QVector3D(xShift+frontShift,yShift,baseHeigh),front3+span,blockYOrig,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,1.0f,1.0f*(randstories-1));

						frontShift+=front3+span;
					}

					// 3 sides
					float spanS=blockY/(numSide1+numSide2);
					for(int i=2;i<=4;i=i+2){
						if(numSide1>0)
						{	
							renderFace(i,new QVector3D(xShift,yShift,0),blockXOrig,numSide1*(side1+spanS),baseHeigh,&normals,&textures,4*subType+3,1.0f*numSide1,1.0f);//1st
							renderFace(i,new QVector3D(xShift,yShift,baseHeigh),blockXOrig,numSide1*(side1+spanS),baseHeigh*(randstories-1),&normals,&textures,4*subType+1,0.25f*numSide1,1.0f*(randstories-1));
						}
						if(numSide2)
						{
							renderFace(i,new QVector3D(xShift,yShift+numSide1*(side1+spanS),0),blockXOrig,side2+spanS,baseHeigh,&normals,&textures,4*subType+3,1.0f,1.0f);	
							renderFace(i,new QVector3D(xShift,yShift+numSide1*(side1+spanS),baseHeigh),blockXOrig,side2+spanS,baseHeigh*(randstories-1),&normals,&textures,4*subType+1,1.0f,1.0f*(randstories-1));
						}
					}

					if(roofTex==-1){
						roofTex=bestTextureForRoof(blockXOrig,blockYOrig);
					}

					//4 roof
					renderFace(5,new QVector3D(xShift,yShift,0),blockXOrig,blockYOrig,baseHeigh*randstories,&normals,&BuildingRenderer::roofTextures,roofTex,1.0f,1.0f);//roof
					yShift+=blockYOrig;
					if(!(bdgProb==6))
						randstories=(rand()%(curStories/2))+curStories/2+1;
					else
						bdgProb=7;

				}
				xShift+=frontShift;
			}
			glPopMatrix();
			return;
		}
		if(subType==7 || subType==8 || subType==9)
		{

			float dyOrig=dy;
			float dxOrig=dx;
			float leftWindow=4.5f;
			float leftHeigh=3.6;
			float frstHeight=4.6;
			float roofHeight=0.8f;
			float garageOffset=0.0f;
			float baseTex=1.0f;

			float shiftY;
			float shiftX;
			float shiftXX;

			int numleftWindows,i;

			float length;

			MTC::misctools::Polygon3D roof[4];
			MTC::misctools::Polygon3D* currRoof;
			MTC::misctools::Polygon3D smallRoof;


			if(curStories>10)
			{
				garageOffset=3.0f*leftWindow;
				baseTex=floor((curStories/5.0f)/frstHeight)+2;
				frstHeight*=curStories/5.0f;
				curStories-=baseTex;
			}
			else
				curStories--;
			if(curStories<1)
				curStories=1;
			int corner=rand()%3;
			int cornerWin;
			int minWins=0;

			float minLength=(dx>dy?dy:dx);
			if(minLength>leftWindow){

				minWins+=floor(minLength/(leftWindow));
				minLength-=floor(minLength/(leftWindow))*leftWindow;
			}

			if(garageOffset!=0.0f)
			{
				cornerWin=(rand()%(int)(ceil((minWins-6)/3.0f)));
			}
			else
			{
				cornerWin=(rand()%(int)(ceil(minWins/3.0f)));
			}
			if(cornerWin<1)
				cornerWin=1;
			//qDebug() << minWins << "\n";
			if(garageOffset!=0 && minWins==8 && corner==1)
				corner=2;


			// DRAW
			for(int f=0;f<4;f++){
				shiftY=0;
				if(f==0){
					length=dxOrig;
					currRoof=&roof[0];
				}
				if(f==1){
					length=dxOrig;
					currRoof=&roof[2];
				}
				if(f==2){
					length=dyOrig;
					currRoof=&roof[3];
				}
				if(f==3){
					length=dyOrig;
					currRoof=&roof[1];
				}

				//calculate the number of windows and blocks
				float origlength=length;
				numleftWindows=1;
				length-=(leftWindow);
				if(length>leftWindow){

					numleftWindows+=floor(length/(leftWindow));
					length-=floor(length/(leftWindow))*leftWindow;
				}



				//transform view to the proper face
				if(f==1){
					glPushMatrix();
					glTranslatef(dxOrig,dyOrig,0);
					glRotatef(180.0f,0,0,1.0f);
				}
				if(f==2){
					glPushMatrix();
					glTranslatef(0,dyOrig,0);
					glRotatef(270.0f,0,0,1.0f);
				}
				if(f==3){
					glPushMatrix();
					glTranslatef(dxOrig,0,0);
					glRotatef(90.0f,0,0,1.0f);
				}

				shiftX=0;
				shiftXX=garageOffset;
				float spanWin=length;
				float spanWin2=length;
				//LEFT


				if(corner==0)
				{
					for(int i =0;i<cornerWin;i++)
					{
						renderFace(4,new QVector3D(0,shiftX,shiftY),leftWindow,leftWindow,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(4,new QVector3D(garageOffset,shiftXX,shiftY+frstHeight),leftWindow,leftWindow,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						renderFace(4,new QVector3D(garageOffset,shiftXX,leftHeigh*curStories+frstHeight),leftWindow,leftWindow,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

						renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(3,new QVector3D(shiftXX,garageOffset,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						renderFace(3,new QVector3D(shiftXX,garageOffset,leftHeigh*curStories+frstHeight),leftWindow,leftWindow,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

						currRoof->contour.push_back(QVector3D(shiftX+garageOffset,garageOffset,leftHeigh*curStories+frstHeight+roofHeight));

						shiftX+=leftWindow;
						shiftXX+=leftWindow;
					}
				}
				else if(corner==1)
				{
					for(int i =0;i<cornerWin;i++)
					{
						renderFace(4,new QVector3D(0,shiftX,shiftY),leftWindow,leftWindow,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(3,new QVector3D(shiftXX,garageOffset+leftWindow*cornerWin,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						renderFace(3,new QVector3D(shiftXX,garageOffset+leftWindow*cornerWin,leftHeigh*curStories+frstHeight),leftWindow,1.0f,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

						renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(4,new QVector3D(garageOffset+leftWindow*(cornerWin),shiftXX,shiftY+frstHeight),leftWindow,leftWindow,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						renderFace(4,new QVector3D(garageOffset+leftWindow*(cornerWin),shiftXX,leftHeigh*curStories+frstHeight),leftWindow,leftWindow,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

						shiftX+=leftWindow;
						shiftXX+=leftWindow;
					}
					currRoof->contour.push_back(QVector3D(garageOffset,garageOffset+cornerWin*leftWindow,leftHeigh*curStories+frstHeight+roofHeight));
					currRoof->contour.push_back(QVector3D(garageOffset+cornerWin*leftWindow,garageOffset+cornerWin*leftWindow,leftHeigh*curStories+frstHeight+roofHeight));
					currRoof->contour.push_back(QVector3D(garageOffset+cornerWin*leftWindow,garageOffset,leftHeigh*curStories+frstHeight+roofHeight));
				}
				else if(corner==2)
				{
					for(int i =0;i<cornerWin;i++)
					{
						renderFace(4,new QVector3D(0,shiftX,shiftY),leftWindow,leftWindow,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window 1st
						renderFace(7,new QVector3D(i*leftWindow+garageOffset,(cornerWin-1-i)*leftWindow+garageOffset,shiftY+frstHeight),leftWindow,leftWindow,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						renderFace(7,new QVector3D(i*leftWindow+garageOffset,(cornerWin-1-i)*leftWindow+garageOffset,leftHeigh*curStories+frstHeight),leftWindow,leftWindow,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

						currRoof->contour.push_back(QVector3D(shiftX+garageOffset,(cornerWin-i)*leftWindow+garageOffset,leftHeigh*curStories+frstHeight+roofHeight));

						shiftX+=leftWindow;
						shiftXX+=leftWindow;
					}
				}
				//numleftWindows--;
				numleftWindows=numleftWindows-(cornerWin*2)+1;

				if(numleftWindows>1)
				{
					spanWin=spanWin/(numleftWindows-1);
					if(garageOffset!=0.0f)
						spanWin2=spanWin2/(numleftWindows-6);
					else
						spanWin2=spanWin;

				}
				else
				{
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,baseTex);//windowSpan
					renderFace(3,new QVector3D(shiftXX,garageOffset,shiftY+frstHeight),spanWin2,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+2,1.0,1.0f*curStories);//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX+garageOffset,garageOffset,leftHeigh*curStories+frstHeight+roofHeight));
					shiftX+=spanWin;
					shiftXX+=spanWin2;
				}


				for(i=1;i<numleftWindows;i++){
					renderFace(3,new QVector3D(shiftX,0,shiftY),spanWin,1.0f,frstHeight,&normals,&textures,subType*4+3,0.2f,baseTex);//windowSpan
					currRoof->contour.push_back(QVector3D(shiftX+garageOffset,garageOffset,leftHeigh*curStories+frstHeight+roofHeight));
					shiftX+=spanWin;
					renderFace(3,new QVector3D(shiftX,0,shiftY),leftWindow,1.0f,frstHeight,&normals,&textures,subType*4+0,1.0f,baseTex);//window
					currRoof->contour.push_back(QVector3D(shiftX+garageOffset,garageOffset,leftHeigh*curStories+frstHeight+roofHeight));
					shiftX+=leftWindow;

					if(i<numleftWindows-6 || !garageOffset)
					{
						renderFace(3,new QVector3D(shiftXX,garageOffset,shiftY+frstHeight),spanWin2,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+2,1.0,1.0f*curStories);//windowSpan
						shiftXX+=spanWin2;
						renderFace(3,new QVector3D(shiftXX,garageOffset,shiftY+frstHeight),leftWindow,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+1,1.0f,1.0f*curStories);//window
						shiftXX+=leftWindow;
					}

				}

				if(shiftXX<origlength-garageOffset-cornerWin*leftWindow)
				{
					renderFace(3,new QVector3D(shiftXX,garageOffset,shiftY+frstHeight),origlength-garageOffset-leftWindow*cornerWin-shiftXX,1.0f,leftHeigh*curStories,&normals,&textures,subType*4+2,1.0,1.0f*curStories);//windowSpan
					shiftXX=origlength-garageOffset-leftWindow*cornerWin;

				}

				renderFace(3,new QVector3D(garageOffset+cornerWin*leftWindow,garageOffset,leftHeigh*curStories+frstHeight),shiftXX-garageOffset-cornerWin*leftWindow,1.0f,roofHeight,&normals,&textures,subType*4+3,1.0f,0.09f);//window roof

				shiftY+=leftHeigh*curStories+frstHeight+roofHeight;

				if(f!=0){
					glPopMatrix();

				}
			}

			//draw roof
			QVector3D *p;
			float x,y;
			int j;
			MTC::misctools::Polygon3D allRoof;
			for(int i=0;i<4;i++){
				for(j=0;j<roof[i].contour.size();j++){
					p=&roof[i].contour.at(j);
					if(i!=0){
						x=p->x();
						y=p->y();
						if(i==1||i==3){//swap x and y
							float swap=x;
							x=y;
							y=swap;
							if(i==1){
								x=dxOrig-x;
							}
						}
						if(i==2){
							y=dyOrig-y;
							x=dxOrig-x;
						}
						if(i==3){
							y=dyOrig-y;
						}
						p->setX(x);
						p->setY(y);
					}
					p->setZ(leftHeigh*curStories+frstHeight+roofHeight);
				}

			}
			float permiter[4]={0,0,0,0};
			for(int i=0;i<4;i++){
				for(int j=0;j<roof[i].contour.size();j++){
					allRoof.contour.push_back(roof[i].contour.at(j));
					permiter[i]+=(roof[i].contour.at(j)-roof[i].contour.at((j+1)%roof[i].contour.size())).length();
				}
			}


			if(roofTex==-1){
				roofTex=bestTextureForRoof(dxOrig,dyOrig);
			}

			renderFlatRoof(&allRoof,&roofTextures,roofTex);

			smallRoof.contour.push_back(QVector3D(dx,0,frstHeight));
			smallRoof.contour.push_back(QVector3D(dx,dy,frstHeight));
			smallRoof.contour.push_back(QVector3D(0,dy,frstHeight));
			smallRoof.contour.push_back(QVector3D(0,0,frstHeight));

			renderFlatRoof(&smallRoof,&roofTextures,roofTex);

			glPopMatrix();
		}
		*/
	}
}

