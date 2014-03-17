#include <common/Util.h>
#include <road/GraphUtil.h>
#include "RoadGraphEditor.h"

RoadGraphEditor::RoadGraphEditor() {
	clear();
}

RoadGraphEditor::~RoadGraphEditor() {
	clear();
}

/**
 * Clear the road graph, and free all the allocated memory as well.
 */
void RoadGraphEditor::clear() {
	roads.clear();

	mode = MODE_DEFAULT;

	selectedVertex = NULL;
	selectedEdge = NULL;
	selectedAreaBuilder.cancel();

	history.clear();
}

void RoadGraphEditor::openRoad(QString filename) {
	clear();

	GraphUtil::loadRoads(roads, filename);
}

void RoadGraphEditor::saveRoad(QString filename) {
	GraphUtil::saveRoads(roads, filename);
}

void RoadGraphEditor::undo() {
	if (history.size() == 0) return;

	GraphUtil::copyRoads(history[history.size() - 1], roads);
	history.pop_back();
	roads.setModified();

	selectedVertex = NULL;
	selectedEdge = NULL;
}

void RoadGraphEditor::cut() {
	if (mode != MODE_AREA_SELECTED) return;
	if (!selectedAreaBuilder.selected()) return;

	history.push_back(roads);

	// extract the roads within the area, and put it into the clipboard.
	clipBoard.copy(roads, selectedArea);

	GraphUtil::subtractRoads2(roads, selectedArea);
}

bool RoadGraphEditor::deleteEdge() {
	if (selectedEdge == NULL) return false;

	RoadGraph temp;
	history.push_back(roads);
	roads.graph[selectedEdgeDesc]->valid = false;
	selectedEdge = NULL;
	roads.setModified();

	return true;
}

void RoadGraphEditor::simplify(float threshold) {
	history.push_back(roads);
	GraphUtil::simplify(roads, threshold);
}

void RoadGraphEditor::reduce() {
	history.push_back(roads);
	GraphUtil::reduce(roads);
}

void RoadGraphEditor::removeShortDeadend(float threshold) {
	history.push_back(roads);
	GraphUtil::removeShortDeadend(roads, threshold);
}

void RoadGraphEditor::planarify() {
	GraphUtil::planarify(roads);
}

void RoadGraphEditor::selectAll() {
	BBox bbox = GraphUtil::getAABoundingBox(roads);
	boost::geometry::convert(bbox, selectedArea);

	mode = MODE_AREA_SELECTED;
}

void RoadGraphEditor::unselectRoads() {
	mode = MODE_DEFAULT;
}

bool RoadGraphEditor::selectVertex(const QVector2D& pt) {
	RoadVertexDesc desc;
	if (!GraphUtil::getVertex(roads, pt, roads.widthBase, desc)) {
		selectedVertex = NULL;
		return false;
	}

	selectedVertexDesc = desc;
	selectedVertex = roads.graph[desc];

	// clear the selction for other items
	selectedEdge = NULL;

	// update the mode
	mode = MODE_VERTEX_SELECTED;

	return true;
}

/**
 * 指定された点に近いエッジを選択する。
 * 近くにエッジがない場合は、falseを返却する。
 */
bool RoadGraphEditor::selectEdge(const QVector2D& pt) {
	RoadEdgeDesc desc;
	if (!GraphUtil::getEdge(roads, pt, roads.widthBase, desc)) {
		selectedEdge = NULL;
		return false;
	}

	selectedEdgeDesc = desc;
	selectedEdge = roads.graph[selectedEdgeDesc];

	// clear the selction for other items
	selectedVertex = NULL;

	mode = MODE_EDGE_SELECTED;

	return true;
}

void RoadGraphEditor::startMovingVertex() {
	//history.push_back(roads);

	mode = MODE_VERTEX_MOVING;
}

void RoadGraphEditor::moveVertex(const QVector2D& pt) {
	GraphUtil::moveVertex(roads, selectedVertexDesc, pt);
}

void RoadGraphEditor::moveVertex(const QVector2D& pt, float snap_threshold) {
	// if there is a vertex close to this point, snap to it.
	RoadVertexDesc desc;
	if (GraphUtil::getVertex(roads, pt, snap_threshold, selectedVertexDesc, desc)) {
		GraphUtil::moveVertex(roads, selectedVertexDesc, roads.graph[desc]->pt);
	} else {
		GraphUtil::moveVertex(roads, selectedVertexDesc, pt);
	}
}

void RoadGraphEditor::stopMovingVertex() {
	mode = MODE_VERTEX_SELECTED;
}

void RoadGraphEditor::stopMovingVertex(float snap_threshold) {
	// if there is a vertex close to this point, snap to it.
	RoadVertexDesc desc;
	if (GraphUtil::getVertex(roads, roads.graph[selectedVertexDesc]->pt, snap_threshold, selectedVertexDesc, desc)) {
		GraphUtil::snapVertex(roads, selectedVertexDesc, desc);
	}
}

bool RoadGraphEditor::splitEdge(const QVector2D& pt) {
	RoadEdgeDesc e_desc;
	if (GraphUtil::getEdge(roads, pt, roads.widthBase, e_desc)) {
		GraphUtil::splitEdge(roads, e_desc, pt);
		return true;
	} else {
		return false;
	}
}
