#pragma once

#include <QDockWidget>
#include "ui_PropertyWidget.h"
#include <road/RoadGraph.h>

class MainWindow;

class PropertyWidget : public QDockWidget {
Q_OBJECT

private:
	Ui::PropertyWidget ui;
	MainWindow* mainWin;

public:
	PropertyWidget(MainWindow* mainWin);

	void setRoadVertex(RoadGraph &roads, RoadVertexDesc vertexDesc, RoadVertexPtr selectedVertex);
	void setRoadEdge(RoadEdgePtr selectedEdge);
	void resetRoadVertex();
	void resetRoadEdge();

public slots:
	void searchVertex();
};

