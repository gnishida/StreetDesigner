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

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <render/camera_3d.h>
#include <render/TextureManager.h>
#include <render/SimpleSkyBox.h>
#include <road/RoadAreaSet.h>


class MainWindow;

class GLWidget3D : public QGLWidget {
protected:
	MainWindow* mainWin;
	ucore::Camera3D myCam;
	ucore::TextureManager* textureManager;
	ucore::SimpleSkyBox* skyBox;
	bool shiftPressed;
	bool controlPressed;
	bool altPressed;
	bool keyMPressed;
	QPoint lastPos;
	float farPlaneToSpaceRadiusFactor;
	float spaceRadius;
	float rotationSensitivity;
	float zoomSensitivity;

public:
	GLWidget3D(MainWindow *parent);
	~GLWidget3D();

	void updateMe();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

	void resetMyCam();

	void setLightPosition(double altitude, double azimuth);
	void setLightPosition(GLfloat x, GLfloat y, GLfloat z);

	void mouseTo2D(int x, int y, QVector2D &result);

	void drawScene();

protected:
	void initializeGL();

	void resizeGL(int width, int height);
	void paintGL();    

	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);

	void changeXRotation(float angle);
	void changeYRotation(float angle);
	void changeZRotation(float angle);
	void changeXYZTranslation(float dx, float dy, float dz);
};

