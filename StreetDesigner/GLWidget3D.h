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

#include "VBORenderManager.h"

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <render/camera_3d.h>
#include <render/TextureManager.h>
#include <render/SimpleSkyBox.h>
#include "RoadAreaSet.h"


class MainWindow;

class GLWidget3D : public QGLWidget {
protected:
	MainWindow* mainWin;
	mylib::Camera3D myCam;
	mylib::TextureManager* textureManager;
	mylib::SimpleSkyBox* skyBox;
	bool shiftPressed;
	bool controlPressed;
	bool altPressed;
	bool keyMPressed;
	QPoint lastPos;
	float farPlaneToSpaceRadiusFactor;
	float spaceRadius;
	float rotationSensitivity;
	float zoomSensitivity;

	RoadVertexDesc selectedVertexDesc;
	RoadVertexPtr selectedVertex;
	RoadEdgeDesc selectedEdgeDesc;
	RoadEdgePtr selectedEdge;
	bool vertexSelected;
	bool edgeSelected;

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
	void selectVertex(RoadGraph &roads, RoadVertexDesc v_desc);
	void selectEdge(RoadGraph &roads, RoadEdgeDesc e_desc);

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

