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

#include "GLWidget3D.h"
#include <common/Util.h>
#include <render/Terrain.h>
#include <render/SimpleSkyBox.h>
#include <road/GraphUtil.h>
#include "MainWindow.h"
#include <gl/GLU.h>



GLWidget3D::GLWidget3D(MainWindow* mainWin) : QGLWidget(QGLFormat(QGL::SampleBuffers), (QWidget*)mainWin) {
	this->mainWin = mainWin;

	spaceRadius=30000.0;
	farPlaneToSpaceRadiusFactor=5.0f;//N 5.0f

	rotationSensitivity = 0.4f;
	zoomSensitivity = 10.0f;

	controlPressed=false;
	shiftPressed=false;
	altPressed=false;
	keyMPressed=false;

	myCam.setRotation(0, 0, 0);
	myCam.setTranslation(0, 0, 8000);
}

GLWidget3D::~GLWidget3D() {
}

QSize GLWidget3D::minimumSizeHint() const {
	return QSize(200, 200);
}

QSize GLWidget3D::sizeHint() const {
	return QSize(400, 400);
}

void GLWidget3D::resetMyCam() {
	myCam.resetCamera();
	updateGL();
}

void GLWidget3D::changeXRotation(float angle) {
	myCam.changeXRotation(angle);
}

void GLWidget3D::changeYRotation(float angle) {
	myCam.changeYRotation(angle);
}

void GLWidget3D::changeZRotation(float angle) {	
	myCam.changeZRotation(angle);
}

void GLWidget3D::changeXYZTranslation(float dx, float dy, float dz) {
	//if (fabs(myCam.dz) < 2.0f * this->spaceRadius || dz < 0) {
		myCam.changeXYZTranslation(dx, dy, dz);
	//}				
}

void GLWidget3D::mousePressEvent(QMouseEvent *event) {
	QVector2D pos;

	this->setFocus();

	lastPos = event->pos();
	mouseTo2D(event->x(), event->y(), pos);

	switch (mainWin->mode) {
	case MainWindow::MODE_AREA_SELECT:
		if (altPressed) {
			float change = 5;
			if (event->buttons() & Qt::RightButton) {
				change = -change;
			}

			mainWin->urbanGeometry->terrain->addValue(pos.x(), pos.y(), change);
			//mainWin->urbanGeometry->adaptToTerrain();

			updateGL();
		} else {
			mainWin->urbanGeometry->areas.selectArea(pos);
		}
		break;
	case MainWindow::MODE_AREA_CREATE:
		std::cout << pos.x() << "," << pos.y() << std::endl;

		if (!mainWin->urbanGeometry->areaBuilder.selecting()) {
			mainWin->urbanGeometry->areaBuilder.start(pos);
			setMouseTracking(true);
		}
		
		if (mainWin->urbanGeometry->areaBuilder.selecting()) {
			mainWin->urbanGeometry->areaBuilder.addPoint(pos);
		}

		mainWin->urbanGeometry->areas.selectedIndex = -1;

		break;
	case MainWindow::MODE_DEBUG:
		if (GraphUtil::getVertex(mainWin->urbanGeometry->roads, pos, 10, selectedVertexDesc)) {
			selectedVertex = mainWin->urbanGeometry->roads.graph[selectedVertexDesc];
			mainWin->propertyWidget->setRoadVertex(mainWin->urbanGeometry->roads, selectedVertexDesc, selectedVertex);
			mainWin->propertyWidget->resetRoadEdge();
		} else {
			if (GraphUtil::getEdge(mainWin->urbanGeometry->roads, pos, 10, selectedEdgeDesc)) {
				selectedEdge = mainWin->urbanGeometry->roads.graph[selectedEdgeDesc];
				mainWin->propertyWidget->resetRoadVertex();
				mainWin->propertyWidget->setRoadEdge(selectedEdge);
			}
		}
		break;
	}
}

void GLWidget3D::mouseReleaseEvent(QMouseEvent *event) {
	event->ignore();

	updateGL();

	return;
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *event) {
	QVector2D pos;
	mouseTo2D(event->x(), event->y(), pos);

	float dx = (float)(event->x() - lastPos.x());
	float dy = (float)(event->y() - lastPos.y());
	float camElevation = myCam.getCamElevation();

	switch (mainWin->mode) {
	case MainWindow::MODE_AREA_SELECT:
		if (altPressed) {	// editing
			float change = 5;
			if (event->buttons() & Qt::RightButton) {
				change = -change;
			}
			mainWin->urbanGeometry->terrain->addValue(pos.x(), pos.y(), change);
			//mainWin->urbanGeometry->adaptToTerrain();
		} else if (event->buttons() & Qt::LeftButton) {	// Rotate
			changeXRotation(rotationSensitivity * dy);
			changeZRotation(-rotationSensitivity * dx);    
			lastPos = event->pos();
		} else if (event->buttons() & Qt::MidButton) {
			changeXYZTranslation(-dx, dy, 0);
			lastPos = event->pos();
		} else if (event->buttons() & Qt::RightButton) {	// Zoom
			changeXYZTranslation(0, 0, -zoomSensitivity * dy);
			lastPos = event->pos();
		}
		break;
	case MainWindow::MODE_AREA_CREATE:
		if (mainWin->urbanGeometry->areaBuilder.selecting()) {	// Move the last point of the selected polygonal area
			mainWin->urbanGeometry->areaBuilder.moveLastPoint(pos);
		}

		break;
	}

	updateGL();
}

void GLWidget3D::mouseDoubleClickEvent(QMouseEvent *e) {
	setMouseTracking(false);

	switch (mainWin->mode) {
	case MainWindow::MODE_AREA_CREATE:
		mainWin->urbanGeometry->areaBuilder.end();
		mainWin->urbanGeometry->areas.add(RoadAreaPtr(new RoadArea(mainWin->urbanGeometry->areaBuilder.polygon())));
		mainWin->urbanGeometry->areas.selectLastArea();
		mainWin->urbanGeometry->areas.selectedArea()->adaptToTerrain(mainWin->urbanGeometry->terrain);
		//mainWin->urbanGeometry->areas.selectedArea()->roads.setZ(myCam.dz);

		mainWin->mode = MainWindow::MODE_AREA_SELECT;
		mainWin->ui.actionAreaSelect->setChecked(true);
		mainWin->ui.actionAreaCreate->setChecked(false);
		break;
	}
}

void GLWidget3D::initializeGL() {
	qglClearColor(QColor(113, 112, 117));
	glClearDepth(1.0f);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//---- lighting ----
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	static GLfloat lightPosition[4] = { 0.0f, 0.0f, 100.0f, 0.0f };
	static GLfloat lightAmbient[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat lightDiffuse[4]  = { 0.75f, 0.75f, 0.5f, 1.0f };

	glEnable(GL_COLOR_MATERIAL);	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);				
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	static GLfloat materialSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat materialEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };				
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialEmission);

	textureManager = mylib::TextureManager::getInstance(this);
	textureManager->initializeTextures(this, "data/textures.txt");
	skyBox = new mylib::SimpleSkyBox(2.0f * this->spaceRadius);
}

void GLWidget3D::resizeGL(int width, int height) {
	height = height?height:1;

	glViewport( 0, 0, (GLint)width, (GLint)height );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(myCam.fovy,(GLfloat)width/(GLfloat)height, 5.0f, farPlaneToSpaceRadiusFactor*spaceRadius);// N 1.0f

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	myCam.applyCamTransform();
}

void GLWidget3D::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
	glDepthFunc(GL_LESS);							// The Type Of Depth Testing To Do
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	myCam.applyCamTransform();	

	drawScene();		
}

void GLWidget3D::drawScene() {
	static GLfloat lightPosition[4] = { 10.0f, 100.0f, 100.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	mainWin->urbanGeometry->render(textureManager);

	//Sky
	skyBox->render(&myCam, textureManager);

	//disable depth buffer 
	glDepthFunc(GL_ALWAYS);
	glDisable(GL_TEXTURE_2D);

	glFlush();
}	

void GLWidget3D::keyPressEvent( QKeyEvent *e ){
	//printf("k\n");
	float sensitivityFactor;
	float camElevation = myCam.getCamElevation();
	sensitivityFactor = (0.01f+0.01*camElevation)*spaceRadius*0.001;

	shiftPressed=false;
	controlPressed=false;
	altPressed=false;
	keyMPressed=false;

	switch( e->key() ){
	case Qt::Key_Shift:
		shiftPressed=true;
		//clientMain->statusBar()->showMessage("Shift pressed");
		break;
	case Qt::Key_Control:
		controlPressed=true;
		break;
	case Qt::Key_Alt:
		altPressed=true;
		break;
	case Qt::Key_Escape:
		mainWin->urbanGeometry->areaBuilder.cancel();
		updateGL();
		break;
	case Qt::Key_R:
		std::cout << "Reseting camera pose\n";
		myCam.resetCamera();
		break;
	case Qt::Key_W:
		changeXYZTranslation(0.0f, -sensitivityFactor, 0.0f);
		break;
	case Qt::Key_S:
		changeXYZTranslation(0.0f, sensitivityFactor, 0.0f);
		break;
	case Qt::Key_D:
		changeXYZTranslation(sensitivityFactor, 0.0f, 0.0f);
		break;
	case Qt::Key_A:
		changeXYZTranslation(-sensitivityFactor, 0.0f, 0.0f);
		break;
	case Qt::Key_Q:
		changeXYZTranslation(0.0f, 0.0f, -sensitivityFactor);
		break;
	case Qt::Key_Z:
		changeXYZTranslation(0.0f, 0.0f, sensitivityFactor);
		break;
	case Qt::Key_C:
		printf("Save camera\n");
		myCam.saveCameraPose("data/camPose.cam");

		break;	
	case Qt::Key_V:
		printf("Load Camera\n");
		myCam.loadCameraPose("data/camPose.cam");
		break;
	case Qt::Key_M:
		keyMPressed=true;
		setMouseTracking(true);
		break;
	default:
		;
	}
}

void GLWidget3D::keyReleaseEvent(QKeyEvent* e) {
	if (e->isAutoRepeat()) {
		e->ignore();
		return;
	}
	switch (e->key()) {
	case Qt::Key_Escape:
		break;
	case Qt::Key_Shift:
		shiftPressed=false;
		break;
	case Qt::Key_Control:
		controlPressed=false;
		break;
	case Qt::Key_Alt:
		altPressed=false;
	case Qt::Key_M:
		keyMPressed=false;
		setMouseTracking(false);
		updateGL();//remove circle
	default:
		;
	}
}

void GLWidget3D::mouseTo2D(int x,int y, QVector2D &result) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	// retrieve the matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// retrieve the projected z-buffer of the origin
	GLdouble origX, origY, origZ;
	gluProject(0, 0, 0, modelview, projection, viewport, &origX, &origY, &origZ);

	// set up the projected point
	GLfloat winX = (float)x;
	GLfloat winY = (float)viewport[3] - (float)y;
	GLfloat winZ = origZ;
	
	// unproject the image plane coordinate to the model space
	GLdouble posX, posY, posZ;
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	result.setX(posX);
	result.setY(posY);
}


