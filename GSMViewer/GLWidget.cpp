#include "GLWidget.h"
#include "MainWindow.h"
#include <road/GraphUtil.h>
#include <gl/GLU.h>
#include <vector>

float GLWidget::MIN_Z = 150.0f;
float GLWidget::MAX_Z = 11520.0f;
//float GLWidget::MAX_Z = 2880.0f;

GLWidget::GLWidget(MainWindow* mainWin) : QGLWidget(QGLFormat(QGL::SampleBuffers), (QWidget*)mainWin) {
	this->mainWin = mainWin;

	//roads = new RoadGraph();
	editor = new RoadGraphEditor();
	renderer = new RendererHelper();

	// set up the camera
	camera = new Camera();
	camera->setLookAt(0.0f, 0.0f, 0.0f);
	camera->setTranslation(0.0f, 0.0f, MIN_Z);

	// initialize the width and others
	editor->roads.setZ(MIN_Z);

	showArea = false;

	// initialize the key status
	shiftPressed = false;
	controlPressed = false;
	altPressed = false;
	keyXPressed = false;
}

GLWidget::~GLWidget() {
}

void GLWidget::drawScene() {
	// draw the road graph
	editor->roads.generate2DMesh();
	renderer->render(editor->roads.renderables);

	// define the height for other items
	float height = (float)((int)(camera->dz * 0.012f)) * 0.1f * 1.5f;

	// draw the selected area
	if (editor->selectedAreaBuilder.selected()) {
		renderer->renderArea(editor->selectedAreaBuilder.polygon(), QColor(0, 0, 255), GL_LINE_STIPPLE, height);
	}

	// draw the selected vertex
	if (editor->selectedVertex != NULL) {
		renderer->renderPoint(editor->selectedVertex->pt, QColor(0, 0, 255), height);
	}

	// draw the selected edge
	if (editor->selectedEdge != NULL) {
		renderer->renderPolyline(editor->selectedEdge->polyline, QColor(0, 0, 255), GL_LINE_STRIP, height);
	}
}

void GLWidget::showStatusMessage() {
	QString strMode;

	switch (editor->mode) {
	case RoadGraphEditor::MODE_DEFAULT:
		strMode = "MODE_DEFAULT";
		break;
	case RoadGraphEditor::MODE_VERTEX_SELECTED:
		strMode = "MODE_VERTEX_SELECTED";
		break;
	case RoadGraphEditor::MODE_VERTEX_MOVING:
		strMode = "MODE_VERTEX_MOVING";
		break;
	case RoadGraphEditor::MODE_EDGE_SELECTED:
		strMode = "MODE_EDGE_SELECTED";
		break;
	case RoadGraphEditor::MODE_DEFINING_AREA:
		strMode = "MODE_DEFINING_AREA";
		break;
	case RoadGraphEditor::MODE_AREA_SELECTED:
		strMode = "MODE_AREA_SELECTED";
		break;
	}

	QString strZ = QString("Z: %1").arg(camera->dz);

	mainWin->setStatusMessage(strZ, strMode);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Event handlers

void GLWidget::keyPressEvent(QKeyEvent *e) {
	mainWin->onMenuEdit();

	shiftPressed = false;
	controlPressed = false;
	altPressed = false;

	switch (e->key()) {
	case Qt::Key_Shift:
		shiftPressed = true;
		break;
	case Qt::Key_Control:
		controlPressed = true;
		break;
	case Qt::Key_Alt:
		altPressed = true;
		break;
	case Qt::Key_X:
		keyXPressed = true;
		break;
	}

	showStatusMessage();
}

void GLWidget::keyReleaseEvent(QKeyEvent* e) {
	if (e->isAutoRepeat()) {
		e->ignore();
		return;
	}

	switch (e->key()) {
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	case Qt::Key_Control:
		controlPressed = false;
		break;
	case Qt::Key_Alt:
		altPressed = false;
		break;
	case Qt::Key_X:
		keyXPressed = false;
		break;
	}

	showStatusMessage();
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
	this->setFocus();

	lastPos = e->pos();
	mouseTo2D(e->x(), e->y(), &last2DPos);

	if (e->buttons() & Qt::LeftButton) {
		//mainWin->ui.statusBar->showMessage(QString("clicked (%1, %2)").arg(pos.x()).arg(pos.y()));

		if (editor->mode == RoadGraphEditor::MODE_AREA_SELECTED) {
			/*if (editor->selectedArea->hitTest(last2DPos)) {
			} else {
				editor->unselectRoads();
			}*/
		} else {
			if (keyXPressed) {
				editor->splitEdge(last2DPos);
			}

			// if the vertex is close to the point, the vertex is selected
			if (editor->selectVertex(last2DPos)) {
				editor->startMovingVertex();
			} else if (editor->selectEdge(last2DPos)) {				
			} else {
				/*if (!editor->selectedAreaBuilder.selecting()) {
					editor->selectedAreaBuilder.start(last2DPos);
					setMouseTracking(true);
					editor->mode = RoadGraphEditor::MODE_DEFINING_AREA;
				}*/

				// if neither a vertex nor a edge is selected, then the selection mode starts
				//editor->startDefiningArea(last2DPos);
			}

			mainWin->controlWidget->setRoadVertex(editor->selectedVertexDesc, editor->selectedVertex);
			mainWin->controlWidget->setRoadEdge(editor->selectedEdge);
		}
	}

	showStatusMessage();
	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	float dx = (float)(e->x() - lastPos.x());
	float dy = (float)(e->y() - lastPos.y());
	
	lastPos = e->pos();

	switch (editor->mode) {
	case RoadGraphEditor::MODE_VERTEX_MOVING:
		if (controlPressed) {
			float snap_threshold = camera->dz * 0.03f;
			editor->stopMovingVertex(snap_threshold);
		} else {
			editor->stopMovingVertex();
		}
		break;
	case RoadGraphEditor::MODE_DEFINING_AREA:
		//editor->stopDefiningArea();
		break;
	}

	e->ignore();

	showStatusMessage();

	setCursor(Qt::ArrowCursor);
	updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
	float dx = (float)(e->x() - lastPos.x());
	float dy = (float)(e->y() - lastPos.y());
	lastPos = e->pos();
	//float camElevation = camera->getCamElevation();

	QVector2D pos;
	mouseTo2D(e->x(), e->y(), &pos);
	float dx2D = pos.x() - last2DPos.x();
	float dy2D = pos.y() - last2DPos.y();
	last2DPos = pos;

	if (e->buttons() & Qt::LeftButton) {
		switch (editor->mode) {
		case RoadGraphEditor::MODE_VERTEX_MOVING:
			if (controlPressed) {
				float snap_threshold = camera->dz * 0.03f;

				// move the selected vertex
				editor->moveVertex(last2DPos, snap_threshold);
			} else {
				// move the selected vertex
				editor->moveVertex(last2DPos);
			}
			break;
		case RoadGraphEditor::MODE_DEFINING_AREA:
			// update the selection box
			/*if (editor->selectedAreaBuilder.selecting()) {	// Move the last point of the selected polygonal area
				editor->selectedAreaBuilder.moveLastPoint(pos);
			}*/
			break;
		}
	} else if (e->buttons() & Qt::MidButton) {   // Shift the camera
		camera->changeXYZTranslation(-dx * camera->dz * 0.001f, dy * camera->dz * 0.001f, 0);
	} else if (e->buttons() & Qt::RightButton) { // Zoom the camera
		setCursor(Qt::SizeVerCursor);

		camera->changeXYZTranslation(0, 0, -dy * camera->dz * 0.02f);
		if (camera->dz < MIN_Z) camera->dz = MIN_Z;
		if (camera->dz > MAX_Z) camera->dz = MAX_Z;

		// tell the Z coordinate to the road graph so that road graph updates rendering related variables.
		editor->roads.setZ(camera->dz);

		lastPos = e->pos();
	}

	last2DPos = pos;
	showStatusMessage();

	updateGL();
}

void GLWidget::mouseDoubleClickEvent(QMouseEvent *e) {
	setMouseTracking(false);

	if (editor->selectedAreaBuilder.selecting()) {
		editor->selectedAreaBuilder.end();
		editor->selectedArea = editor->selectedAreaBuilder.polygon();

		editor->mode = RoadGraphEditor::MODE_DEFAULT;
	}
}

void GLWidget::initializeGL() {
	qglClearColor(QColor(233, 229, 220));
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
	static GLfloat lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat lightDiffuse[4] = { 0.75f, 0.75f, 0.5f, 1.0f };

	glEnable(GL_COLOR_MATERIAL);	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	static GLfloat materialSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat materialEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialEmission);
}

void GLWidget::resizeGL(int width, int height) {
	height = height ? height : 1;

	glViewport(0, 0, (GLint)width, (GLint)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 5.0f, 90000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->applyCamTransform();
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	camera->applyCamTransform();	

	drawScene();	
}

/**
 * Convert the screen space coordinate (x, y) to the model space coordinate.
 */
void GLWidget::mouseTo2D(int x,int y, QVector2D *result) {
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

	result->setX(posX);
	result->setY(posY);
}
