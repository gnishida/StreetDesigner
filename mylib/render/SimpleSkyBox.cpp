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

#include "SimpleSkyBox.h"
#include "TextureManager.h"
#include "Texture.h"

namespace ucore {

std::vector<Texture*> SimpleSkyBox::textures;

SimpleSkyBox::SimpleSkyBox(float sideLength) {
	this->sideLength = sideLength;
}

SimpleSkyBox::~SimpleSkyBox() {
}

void SimpleSkyBox::render(Camera3D* camera, TextureManager* textureManager) {
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP );

	//glColor4ub(113,112,117,255);//grey color of bottom 

	glEnable(GL_TEXTURE_2D);

	// Save Current Matrix
	glPushMatrix();

	glLoadIdentity();						
	glRotatef(camera->xrot, 1.0, 0.0, 0.0);
	glRotatef(camera->yrot, 0.0, 1.0, 0.0);	
	glRotatef(camera->zrot, 0.0, 0.0, 1.0);
	glTranslatef(-camera->lookAtX, -camera->lookAtY, -camera->lookAtZ);

	// First apply scale matrix
	glScalef(sideLength, sideLength, sideLength);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode (GL_MODELVIEW);
	float r = 1.005f; // If you have border issues change this to 1.005f

	// Common Axis Z - FRONT Side
	glBindTexture(GL_TEXTURE_2D, textureManager->get("data/textures/sky_frontav9.jpg")->getId());
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0, 0.0); glVertex3f(-r ,1.0f,-r);	
	glTexCoord2f(1.0, 0.0); glVertex3f( r ,1.0f,-r);
	glTexCoord2f(1.0, 1.0); glVertex3f( r,1.0f,r);
	glTexCoord2f(0.0, 1.0); glVertex3f(-r,1.0f,r); 		
	glEnd();

	// Common Axis Z - BACK side
	glBindTexture(GL_TEXTURE_2D, textureManager->get("data/textures/sky_backav9.jpg")->getId());
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0,0.0);  glVertex3f(-r,-1.0f,-r);
	glTexCoord2f(1.0,1.0);  glVertex3f(-r,-1.0f, r);
	glTexCoord2f(0.0,1.0);  glVertex3f( r,-1.0f, r); 
	glTexCoord2f(0.0,0.0);  glVertex3f( r,-1.0f,-r);
	glEnd();

	// Common Axis X - Left side
	glBindTexture(GL_TEXTURE_2D, textureManager->get("data/textures/sky_leftav9.jpg")->getId());
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0,0.0); glVertex3f(-1.0f, -r,-r);
	glTexCoord2f(1.0,0.0); glVertex3f(-1.0f,  r,-r);			
	glTexCoord2f(1.0,1.0); glVertex3f(-1.0f,  r, r);	
	glTexCoord2f(0.0,1.0); glVertex3f(-1.0f, -r, r); 		
	glEnd();

	// Common Axis X - Right side
	glBindTexture(GL_TEXTURE_2D, textureManager->get("data/textures/sky_rightav9.jpg")->getId());
	glBegin(GL_QUADS);		
	glTexCoord2f( 1.0,1.0); glVertex3f(1.0f, -r, r);	
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f,  r, r); 
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f,  r,-r);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, -r,-r);
	glEnd();

	// Common Axis Y - Draw Up side
	glBindTexture(GL_TEXTURE_2D, textureManager->get("data/textures/sky_topav9.jpg")->getId());
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0, 0.0); glVertex3f( r,  r,1.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f( r, -r,1.0f); 
	glTexCoord2f(0.0, 1.0); glVertex3f(-r, -r,1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-r,  r,1.0f);
	glEnd();

	// Load Saved Matrix
	glPopMatrix(); 	

	glDisable(GL_TEXTURE_2D);
}

} // namespace ucore