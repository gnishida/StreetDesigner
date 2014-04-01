#pragma once

#include <QDockWidget>
#include "ui_ControlWidget.h"

class MainWindow;

class ControlWidget : public QDockWidget {
Q_OBJECT

private:
	Ui::ControlWidget ui;
	MainWindow* mainWin;

public:
	ControlWidget(MainWindow* mainWin);

	//void setRoadVertex(RoadVertexDesc vertexDesc, RoadVertexPtr selectedVertex);
	//void setRoadEdge(RoadEdgePtr selectedEdge);

public slots:
	void generateKDE();
	void generateUShape();
	void perturb();
	void clear();
	void mergeRoads();
	void connectRoads();
};

