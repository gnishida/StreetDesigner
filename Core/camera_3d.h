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

#pragma once

#include "qvector3d.h"

#include "qtextstream.h"

#include <QtOpenGL>


namespace ucore {

class Camera3D {
public:
	float xrot;
	float yrot;
	float zrot;

	float dx;
	float dy;
	float dz;

	float lookAtX;
	float lookAtY;
	float lookAtZ;

	float fovy;

	Camera3D() {
		this->resetCamera();	
	}

	~Camera3D() {
	}

	void resetCamera() {
		xrot = 310.0;
		yrot = 0.0;
		zrot = 315.0;
		dx = 0.0;
		dy = 0.0;
		dz = 800.0;
		lookAtX = 0.0;
		lookAtY = 0.0;
		lookAtZ = 0.0;
		fovy = 45.0f;
	}

	void applyCamTransform();

	QVector3D getCamPosition();
	float getCamElevation();

	void saveCameraPose(QString fileName);
	void loadCameraPose(QString fileName);

	void saveCameraPose(QTextStream& cam);
	bool loadCameraPose(QTextStream& cam);

	void setRotation(float x, float y, float z) {
		setXRotation(x);
		setYRotation(y);
		setZRotation(z);		
	}

	void setXRotation(float angle) {
		qNormalizeAngle(angle);
		xrot = angle;			
	}

	void setYRotation(float angle) {
		qNormalizeAngle(angle);
		yrot = angle;			
	}

	void setZRotation(float angle) {
		qNormalizeAngle(angle);
		zrot = angle;			
	}

	void changeXRotation(float angle) {
		setXRotation(xrot+angle);
	}

	void changeYRotation(float angle) {
		setYRotation(yrot+angle);
	}

	void changeZRotation(float angle) {
		setZRotation(zrot+angle);
	}

	void setTranslation(float x, float y, float z) {
		dx = x;
		dy = y;
		dz = z;
	}

	void changeXYZTranslation(float x, float y, float z) {
		dx += x;
		dy += y;
		dz += z;
	}

	void setLookAt(float x, float y, float z) {
		lookAtX = x;
		lookAtY = y;
		lookAtZ = z;
	}

	void setFOVy(float inFOVy) {
		fovy = inFOVy;
	}

protected:
	void qNormalizeAngle(float &angle) {
		while (angle < 0)
			angle += 360.0;
		while (angle > 360.0)
			angle -= 360.0;
	}
};

} // namespace ucore

