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

#include "camera_3d.h"
#include "qmatrix4x4.h"
#include "qvector4d.h"
#include "qfile.h"

namespace ucore {

void Camera3D::applyCamTransform() {
	glLoadIdentity();		
	glTranslatef(-dx, -dy, -dz);	
	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);	
	glRotatef(zrot, 0.0, 0.0, 1.0);
	glTranslatef(-lookAtX, -lookAtY, -lookAtZ);
	//printf("trans %f %f %f rot %f %f %f look %f %f %f\n",dx,dy,dz,xrot,yrot,zrot,lookAtX,lookAtY,lookAtZ);
}

QVector4D getCamPos() {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	QMatrix4x4 mvMat(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	QVector4D eye(0.0f, 0.0f, 0.0f, 1.0f);
	return ((mvMat.transposed()).inverted())*eye;
}

QVector3D Camera3D::getCamPosition() {
	QVector4D pos = getCamPos();
	return pos.toVector3D();
}

float Camera3D::getCamElevation() {	
	return getCamPos().z();
}

void Camera3D::saveCameraPose(QString fileName) {
	QFile camFile(fileName);
	if (!camFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		printf("ERROR: Cannot open the file cam.txt for writing\n");
		return;
	}

	saveCameraPose(QTextStream(&camFile));		
	
	camFile.close();
}

void Camera3D::loadCameraPose(QString filename) {
	QFile camFile(filename);

	if (!camFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		printf("Can't open file camPose.cam\n");
	} else {
		loadCameraPose(QTextStream(&camFile));
		camFile.close();
	}
}

void  Camera3D::saveCameraPose(QTextStream& camStream) {
	camStream << this->dx << " " << this->dy << " " << this->dz << " " <<
		this->fovy << " " << this->lookAtX << " " << this->lookAtY << " " << this->lookAtZ << " " <<
		this->xrot << " " << this->yrot << " " << this->zrot<<" ";
}

bool  Camera3D::loadCameraPose(QTextStream& camStream) {
	if(camStream.atEnd())
		return false;
	camStream >> this->dx;
	camStream >> this->dy;
	camStream >> this->dz;
	camStream >> this->fovy;
	camStream >> this->lookAtX;
	camStream >> this->lookAtY;
	camStream >> this->lookAtZ;
	camStream >> this->xrot;
	camStream >> this->yrot;
	camStream >> this->zrot;
	return true;
}

} // namespace ucore