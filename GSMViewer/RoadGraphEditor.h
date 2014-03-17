#pragma once

#include <common/BBox.h>
#include <common/Polygon2D.h>
#include <util/PolygonBuilder.h>
#include <road/RoadGraph.h>
#include "ClipBoard.h"

class RoadGraphEditor {
public:
	static enum {
		MODE_DEFAULT = 0,
		MODE_VERTEX_SELECTED,
		MODE_VERTEX_MOVING,
		MODE_EDGE_SELECTED,
		MODE_DEFINING_AREA,
		MODE_AREA_SELECTED,
	};

public:
	RoadGraph roads;
	RoadGraph roadsOrig;

	int mode;

	PolygonBuilder selectedAreaBuilder;
	Polygon2D selectedArea;				// the selected area

	RoadVertexPtr selectedVertex;
	RoadVertexDesc selectedVertexDesc;
	RoadEdgePtr selectedEdge;
	RoadEdgeDesc selectedEdgeDesc;

	ClipBoard clipBoard;

	std::vector<RoadGraph> history;

public:
	RoadGraphEditor();
	~RoadGraphEditor();

	void clear();
	void openRoad(QString filename);
	void saveRoad(QString filename);
	void undo();
	void cut();
	bool deleteEdge();
	void simplify(float threshold);
	void reduce();
	void removeShortDeadend(float threshold);
	void planarify();

	void selectAll();

	void unselectRoads();

	bool selectVertex(const QVector2D& pt);
	bool selectEdge(const QVector2D& pt);

	// MODE_VERTEX_SELECTED | MODE_VERTEX_MOVING
	void startMovingVertex();
	void moveVertex(const QVector2D& pt);
	void moveVertex(const QVector2D& pt, float snap_threshold);
	void stopMovingVertex();
	void stopMovingVertex(float snap_threshold);

	bool splitEdge(const QVector2D& pt);
};

