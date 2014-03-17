#pragma once

#include "Camera.h"
#include <common/PolygonBuilder.h>
#include <road/RoadGraph.h>
#include <road/RoadGraphRenderer.h>
#include <road/feature/RoadFeature.h>
#include <road/feature/GridFeature.h>
#include <road/feature/RadialFeature.h>
#include <road/feature/GenericFeature.h>
#include <QGLWidget>
#include <QString>

class MainWindow;

class GLWidget : public QGLWidget {
public:
	static float MIN_Z;
	static float MAX_Z;

public:
	MainWindow* mainWin;
	Camera* camera;
	RoadGraph roads;
	RoadGraph origRoads;
	RoadGraphRenderer* renderer;
	QPoint lastPos;
	Polygon2D selectedArea;
	PolygonBuilder selectedAreaBuilder;

	RoadFeature roadFeature;
	/*
	std::vector<GridFeature> gridFeatures;
	std::vector<RadialFeature> radialFeatures;
	std::vector<GenericFeature> genericFeatures;
	*/

	// key status
	bool shiftPressed;
	bool controlPressed;
	bool altPressed;
	bool keyXPressed;

public:
	GLWidget(MainWindow *parent);
	~GLWidget();

	void drawScene();

public:
	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	void mouseTo2D(int x, int y, QVector2D &result);
};

