﻿#include <time.h>
#include <QList>
#include <QSet>
#include <QDebug>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include "../common/common.h"
#include "../common/Util.h"
#include "GraphUtil.h"

/**
 * Return the number of vertices.
 *
 */
int GraphUtil::getNumVertices(RoadGraph& roads, bool onlyValidVertex) {
	if (!onlyValidVertex) {
		return boost::num_vertices(roads.graph);
	}

	int count = 0;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		count++;
	}

	return count;
}

int GraphUtil::getNumVertices(RoadGraph& roads, const QVector2D& pos, float radius) {
	float radius2 = radius * radius;

	int count = 0;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if ((roads.graph[*vi]->pt - pos).lengthSquared() <= radius2) {
			count++;
		}
	}

	return count;
}

/**
 * Return the number of vertices which are connected to the specified vertex.
 */
int GraphUtil::getNumConnectedVertices(RoadGraph& roads, RoadVertexDesc start, bool onlyValidVertex) {
	int count = 1;

	QList<RoadVertexDesc> queue;
	queue.push_back(start);

	QList<RoadVertexDesc> visited;
	visited.push_back(start);

	while (!queue.empty()) {
		RoadVertexDesc v = queue.front();
		queue.pop_front();

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
			if (onlyValidVertex && !roads.graph[*ei]->valid) continue;

			RoadVertexDesc u = boost::target(*ei, roads.graph);
			if (onlyValidVertex && !roads.graph[u]->valid) continue;

			if (visited.contains(u)) continue;

			visited.push_back(u);
			queue.push_back(u);
			count++;
		}
	}

	return count;
}

/**
 * Return the index-th vertex.
 */
RoadVertexDesc GraphUtil::getVertex(RoadGraph& roads, int index, bool onlyValidVertex) {
	int count = 0;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		if (count == index) return *vi;

		count++;
	}

	throw "Index exceeds the number of vertices.";
}

/**
 * Find the closest vertex from the specified point. 
 */
RoadVertexDesc GraphUtil::getVertex(RoadGraph& roads, const QVector2D& pt, bool onlyValidVertex) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		float dist = (roads.graph[*vi]->getPt() - pt).lengthSquared();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}

/**
 * Find the closest vertex from the specified point. The specified vertex should be ignored.
 */
RoadVertexDesc GraphUtil::getVertex(RoadGraph& roads, const QVector2D& pt, RoadVertexDesc ignore, bool onlyValidVertex) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (*vi == ignore) continue;
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		float dist = (roads.graph[*vi]->getPt() - pt).lengthSquared();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}

/**
 * 近隣頂点を探す。
 * ただし、方向ベクトルがangle方向からしきい値を超えてる場合、その頂点はスキップする。
 */
RoadVertexDesc GraphUtil::getVertex(RoadGraph& roads, const QVector2D& pt, float angle, float angle_threshold, bool onlyValidVertex) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		QVector2D vec = roads.graph[*vi]->getPt() - pt;
		float angle2 = atan2f(vec.y(), vec.x());
		if (Util::diffAngle(angle, angle2) > angle_threshold) continue;

		float dist = vec.lengthSquared();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}

/**
 * Find the closest vertex from the specified point. 
 * If the closet vertex is within the threshold, return true. Otherwise, return false.
 */
bool GraphUtil::getVertex(RoadGraph& roads, const QVector2D& pos, float threshold, RoadVertexDesc& desc, bool onlyValidVertex) {
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		float dist = (roads.graph[*vi]->getPt() - pos).lengthSquared();
		if (dist < min_dist) {
			min_dist = dist;
			desc = *vi;
		}
	}

	if (min_dist <= threshold * threshold) return true;
	else return false;
}

/**
 * Find the closest vertex from the specified vertex. 
 * If the closet vertex is within the threshold, return true. Otherwise, return false.
 */
bool GraphUtil::getVertex(RoadGraph& roads, RoadVertexDesc v, float threshold, RoadVertexDesc& desc, bool onlyValidVertex) {
	return getVertex(roads, roads.graph[v]->pt, threshold, v, desc, onlyValidVertex);
}

/**
 * Find the closest vertex from the specified point. 
 * If the closet vertex is within the threshold, return true. Otherwise, return false.
 */
bool GraphUtil::getVertex(RoadGraph& roads, const QVector2D& pos, float threshold, RoadVertexDesc ignore, RoadVertexDesc& desc, bool onlyValidVertex) {
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;
		if (*vi == ignore) continue;

		float dist = (roads.graph[*vi]->getPt() - pos).lengthSquared();
		if (dist < min_dist) {
			min_dist = dist;
			desc = *vi;
		}
	}

	if (min_dist <= threshold * threshold) return true;
	else return false;
}

/**
 * 指定した領域の中に、指定した位置に最も近い頂点を探す。
 * 指定した領域の中に、１つも頂点がない場合は、falseを返却する。
 */
bool GraphUtil::getVertexInArea(RoadGraph &roads, const QVector2D &pos, const BBox &area, RoadVertexDesc &desc) {
	bool found = false;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (!area.contains(pos)) continue;

		float dist = (roads.graph[*vi]->pt - pos).lengthSquared();
		if (dist < min_dist) {
			min_dist = dist;
			desc = *vi;
			found = true;
		}
	}

	return found;
}

/**
 * Add a vertex.
 * Note: The specified vertex v is used for this vertex instead of copying it.
 */
RoadVertexDesc GraphUtil::addVertex(RoadGraph& roads, RoadVertexPtr v) {
	//RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(*v));
	RoadVertexDesc new_v_desc = boost::add_vertex(roads.graph);
	roads.graph[new_v_desc] = v;

	roads.setModified();

	return new_v_desc;
}

/**
 * Move the vertex to the specified location.
 * The outing edges are also moved accordingly.
 */
void GraphUtil::moveVertex(RoadGraph& roads, RoadVertexDesc v, const QVector2D& pt) {
	// Move the outing edges
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		Polyline2D polyline = roads.graph[*ei]->polyline;
		if ((polyline[0] - roads.graph[v]->pt).lengthSquared() < (polyline[0] - roads.graph[tgt]->pt).lengthSquared()) {
			std::reverse(polyline.begin(), polyline.end());
		}

		movePolyline(roads, polyline, roads.graph[tgt]->pt, pt);

		roads.graph[*ei]->polyline = polyline;
	}

	// Move the vertex
	roads.graph[v]->pt = pt;

	roads.setModified();
}

/**
 * Return the degree of the specified vertex.
 */
int GraphUtil::getDegree(RoadGraph& roads, RoadVertexDesc v, bool onlyValidEdge) {
	if (onlyValidEdge) {
		int count = 0;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
			if (roads.graph[*ei]->valid) count++;
		}
		return count;
	} else {
		return boost::degree(v, roads.graph);
	}
}

/**
 * Return the list of vertices.
 */
std::vector<RoadVertexDesc> GraphUtil::getVertices(RoadGraph* roads, bool onlyValidVertex) {
	std::vector<RoadVertexDesc> ret;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads->graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads->graph[*vi]->valid) continue;

		ret.push_back(*vi);
	}

	return ret;
}

/**
 * Remove the isolated vertices.
 * Note that this function does not change neither the vertex desc nor the edge desc.
 */
void GraphUtil::removeIsolatedVertices(RoadGraph& roads, bool onlyValidVertex) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (onlyValidVertex && !roads.graph[*vi]->valid) continue;

		if (getDegree(roads, *vi, onlyValidVertex) == 0) {
			roads.graph[*vi]->valid = false;
		}
	}
}

/**
 * Snap v1 to v2.
 */
void GraphUtil::snapVertex(RoadGraph& roads, RoadVertexDesc v1, RoadVertexDesc v2) {
	if (v1 == v2) return;

	moveVertex(roads, v1, roads.graph[v2]->pt);

	if (hasEdge(roads, v1, v2)) {
		RoadEdgeDesc e = getEdge(roads, v1, v2);

		// if the edge is too short, remove it. (This might be contraversial...)
		if (roads.graph[e]->getLength() < 1.0f) {
			roads.graph[e]->valid = false;
		}
	}

	// Snap all the outing edges from v1
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v1, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc v1b = boost::target(*ei, roads.graph);

		// invalidate the old edge
		roads.graph[*ei]->valid = false;

		if (v1b == v2) continue;
		if (hasEdge(roads, v2, v1b)) continue;
		//if (hasCloseEdge(roads, v2, v1b)) continue;	// <-- In this case, snap the edge to the other instead of discard it.

		// add a new edge
		addEdge(roads, v2, v1b, roads.graph[*ei]);
	}

	// invalidate v1
	roads.graph[v1]->valid = false;

	roads.setModified();
}

/**
 * Return the central vertex.
 */
RoadVertexDesc GraphUtil::getCentralVertex(RoadGraph& roads) {
	BBox box = getAABoundingBox(roads);
	return getVertex(roads, box.midPt());
}

float GraphUtil::getDensity(RoadGraph& roads, const QVector2D& pos, float radius) {
	float radius2 = radius * radius;

	int count = 0;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if ((roads.graph[*vi]->pt - pos).lengthSquared() <= radius2) {
			count++;
		}
	}

	return (float)count / radius2 / M_PI;
}

/**
 * Return the index-th edge.
 */
RoadEdgeDesc GraphUtil::getEdge(RoadGraph& roads, int index, bool onlyValidEdge) {
	int count = 0;
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]) continue;

		if (index == count) return *ei;
		count++;
	}

	throw "No edge found for the specified index.";
}

/**
 * Return the total lengths of the edges outing from the specified vertex.
 */
float GraphUtil::getTotalEdgeLength(RoadGraph& roads, RoadVertexDesc v) {
	float ret = 0.0f;

	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		ret += roads.graph[*ei]->getLength();
	}

	return ret;
}

/**
 * Remove the edge. 
 * Remove the vertex that has smaller number of degrees.
 */
/*void GraphUtil::collapseEdge(RoadGraph* roads, RoadEdgeDesc e) {
	RoadVertexDesc v1 = boost::source(e, roads->graph);
	RoadVertexDesc v2 = boost::target(e, roads->graph);
	if (v1 == v2) return;

	// invalidate the edge
	roads->graph[e]->valid = false;

	if (getDegree(roads, v1) < getDegree(roads, v2)) {
		snapVertex(roads, v1, v2);
	} else {
		snapVertex(roads, v2, v1);
	}
}*/

/**
 * Return the number of edges.
 */
int GraphUtil::getNumEdges(RoadGraph& roads, bool onlyValidEdge) {
	if (!onlyValidEdge) {
		return boost::num_edges(roads.graph);
	}

	int count = 0;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (roads.graph[*ei]->valid) count++;
	}

	return count;
}

int GraphUtil::getNumEdges(RoadGraph& roads, RoadVertexDesc v, bool onlyValidEdge) {
	int count = 0;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = out_edges(v, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		count++;
	}

	return count;
}

/**
 * 与えられた頂点から出るエッジのうち、指定されたタイプのエッジの数を返却する。
 */
int GraphUtil::getNumEdges(RoadGraph& roads, RoadVertexDesc v, int roadType, bool onlyValidEdge) {
	int count = 0;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = out_edges(v, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		
		if (!isRoadTypeMatched(roads.graph[*ei]->type, roadType)) continue;

		count++;
	}

	return count;
}

/**
 * Add an edge.
 * This function creates a straight line of edge.
 */
RoadEdgeDesc GraphUtil::addEdge(RoadGraph& roads, RoadVertexDesc src, RoadVertexDesc tgt, unsigned int type, unsigned int lanes, bool oneWay, bool link, bool roundabout) {
	roads.setModified();

	// エッジを新規追加する
	RoadEdgePtr e = RoadEdgePtr(new RoadEdge(type, lanes, oneWay, link, roundabout));
	e->addPoint(roads.graph[src]->getPt());
	e->addPoint(roads.graph[tgt]->getPt());

	std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(src, tgt, roads.graph);
	roads.graph[edge_pair.first] = e;

	return edge_pair.first;
}

/**
 * Add an edge.
 * This function creates a edge which is copied from the reference edge.
 */
RoadEdgeDesc GraphUtil::addEdge(RoadGraph& roads, RoadVertexDesc src, RoadVertexDesc tgt, RoadEdgePtr ref_edge) {
	roads.setModified();

	RoadEdgePtr e = RoadEdgePtr(new RoadEdge(*ref_edge));
	e->valid = true;

	std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(src, tgt, roads.graph);
	roads.graph[edge_pair.first] = e;

	return edge_pair.first;
}

RoadEdgeDesc GraphUtil::addEdge(RoadGraph& roads, const Polyline2D &polyline, unsigned int type, unsigned int lanes, bool oneWay , bool link, bool roundabout) {
	RoadVertexPtr v1 = RoadVertexPtr(new RoadVertex(polyline[0]));
	RoadVertexDesc desc1 = addVertex(roads, v1);
	RoadVertexPtr v2 = RoadVertexPtr(new RoadVertex(polyline.last()));
	RoadVertexDesc desc2 = addVertex(roads, v2);

	RoadEdgeDesc e_desc = addEdge(roads, desc1, desc2, type, lanes, oneWay, link, roundabout);
	roads.graph[e_desc]->polyline = polyline;

	return e_desc;
}

/**
 * Check if there is an edge between two vertices.
 */
bool GraphUtil::hasEdge(RoadGraph& roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge) {
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(desc1, roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		if (tgt == desc2) return true;
	}

	for (boost::tie(ei, eend) = boost::out_edges(desc2, roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		if (tgt == desc1) return true;
	}

	return false;
}

/**
 * Return the edge between src and tgt.
 */
RoadEdgeDesc GraphUtil::getEdge(RoadGraph& roads, RoadVertexDesc src, RoadVertexDesc tgt, bool onlyValidEdge) {
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(src, roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		if (boost::target(*ei, roads.graph) == tgt) return *ei;
	}

	for (boost::tie(ei, eend) = boost::out_edges(tgt, roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		if (boost::target(*ei, roads.graph) == src) return *ei;
	}

	throw "No edge found.";
}

/**
 * Sort the points of the polyline of the edge in such a way that the first point is the location of the src vertex.
 */
/*
std::vector<QVector2D> GraphUtil::getOrderedPolyLine(RoadGraph& roads, RoadEdgeDesc e) {
	std::vector<QVector2D> ret = roads.graph[e]->getPolyLine();

	RoadVertexDesc src = boost::source(e, roads.graph);
	RoadVertexDesc tgt = boost::target(e, roads.graph);
	if ((roads.graph[src]->getPt() - roads.graph[e]->getPolyLine()[0]).lengthSquared() <= (roads.graph[tgt]->getPt() - roads.graph[e]->getPolyLine()[0]).lengthSquared()) {
		return ret;
	} else {
		std::reverse(ret.begin(), ret.end());
		return ret;
	}
}
*/

/**
 * Sort the points of the polyline of the edge in such a way that the first point is the location of the src vertex.
 */
void GraphUtil::getOrderedPolyLine(RoadGraph& roads, RoadEdgeDesc e, std::vector<QVector2D>& polyline) {
	polyline = roads.graph[e]->polyline;

	RoadVertexDesc src = boost::source(e, roads.graph);
	RoadVertexDesc tgt = boost::target(e, roads.graph);
	if ((roads.graph[src]->getPt() - roads.graph[e]->polyline[0]).lengthSquared() > (roads.graph[tgt]->getPt() - roads.graph[e]->polyline[0]).lengthSquared()) {
		std::reverse(polyline.begin(), polyline.end());
	}
}

/**
 * Sort the points of the polyline of the edge in such a way that the first point is the location of the src vertex.
 */
void GraphUtil::orderPolyLine(RoadGraph& roads, RoadEdgeDesc e, RoadVertexDesc src) {
	RoadVertexDesc tgt;

	RoadVertexDesc s = boost::source(e, roads.graph);
	RoadVertexDesc t = boost::target(e, roads.graph);

	if (s == src) {
		tgt = t;
	} else {
		tgt = s;
	}

	// If the order is opposite, reverse the order.
	if ((roads.graph[src]->getPt() - roads.graph[e]->polyline[0]).lengthSquared() > (roads.graph[tgt]->getPt() - roads.graph[e]->polyline[0]).lengthSquared()) {
		std::reverse(roads.graph[e]->polyline.begin(), roads.graph[e]->polyline.end());
	}
}

/**
 * Move the edge to the specified location.
 * src_posは、エッジeのsource頂点の移動先
 * tgt_posは、エッジeのtarget頂点の移動先
 */
void GraphUtil::moveEdge(RoadGraph& roads, RoadEdgeDesc e, QVector2D& src_pos, QVector2D& tgt_pos) {
	RoadVertexDesc src = boost::source(e, roads.graph);
	RoadVertexDesc tgt = boost::target(e, roads.graph);

	int n = roads.graph[e]->polyline.size();

	if ((roads.graph[e]->polyline[0] - roads.graph[src]->pt).lengthSquared() < (roads.graph[e]->polyline[0] - roads.graph[tgt]->pt).lengthSquared()) {
		QVector2D src_diff = src_pos - roads.graph[e]->polyline[0];
		QVector2D tgt_diff = tgt_pos - roads.graph[e]->polyline.last();

		for (int i = 1; i < n - 1; i++) {
			roads.graph[e]->polyline[i] += src_diff + (tgt_diff - src_diff) * (float)i / (float)(n - 1);
		}
		roads.graph[e]->polyline[0] = src_pos;
		roads.graph[e]->polyline[n - 1] = tgt_pos;
	} else {
		QVector2D src_diff = src_pos - roads.graph[e]->polyline.last();
		QVector2D tgt_diff = tgt_pos - roads.graph[e]->polyline[0];

		for (int i = 1; i < n - 1; i++) {
			roads.graph[e]->polyline[i] += tgt_diff + (src_diff - tgt_diff) * (float)i / (float)(n - 1);
		}
		roads.graph[e]->polyline[0] = tgt_pos;
		roads.graph[e]->polyline[n - 1] = src_pos;
	}

	roads.setModified();
}

/**
 * Polylineを指定した始点、終点になるよう変形する。
 * スケールが大きく変化する場合は、スケーリング＆回転で対応。
 * スケールがあまり変化しない場合は、Linear Interpolationにより変形させる。
 */
void GraphUtil::movePolyline(RoadGraph& roads, Polyline2D &polyline, const QVector2D& src_pos, const QVector2D& tgt_pos) {
	float scale = (tgt_pos - src_pos).length() / (polyline.last() - polyline[0]).length();

	if (scale > 2.0f || scale < 0.5f) {
		// スケーリング＆回転で、ポリラインを指定された座標にフィットさせる
		float rotation_degree = Util::rad2deg(Util::diffAngle(polyline.last() - polyline[0], tgt_pos- src_pos, false));

		polyline.scale(scale);
		polyline.rotate(rotation_degree, QVector2D(0, 0));
		polyline.translate(src_pos - polyline[0]);
	} else {
		// 指定された座標に、Linear Interpolationにより移動する
		Polyline2D temp_polyline = polyline;
		QVector2D offset = src_pos - temp_polyline[0];
		QVector2D diff_ratio = (tgt_pos - temp_polyline.last() - offset) / temp_polyline.length();
		for (int i = 1; i < temp_polyline.size() - 1; ++i) {
			polyline[i] += offset + diff_ratio * temp_polyline.length(i);
		}
		polyline[0] = src_pos;
		polyline[polyline.size() - 1] = tgt_pos;
	}
}

/**
 * Remove all the dead-end edges.
 */
bool GraphUtil::removeDeadEnd(RoadGraph& roads) {
	bool removed = false;

	bool removedOne = true;
	while (removedOne) {
		removedOne = false;
		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
			if (!roads.graph[*vi]->valid) continue;

			if (getDegree(roads, *vi) == 1) {
				// invalidate all the outing edges.
				RoadOutEdgeIter ei, eend;
				for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
					roads.graph[*ei]->valid = false;
				}

				// invalidate the vertex as well.
				roads.graph[*vi]->valid = false;

				removedOne = true;
				removed = true;
			}
		}
	}

	if (removed) {
		roads.setModified();
	}

	return removed;
}

/**
 * Remove the isolated edges.
 * Note that this function does not change neither the vertex desc nor the edge desc.
 */
void GraphUtil::removeIsolatedEdges(RoadGraph& roads, bool onlyValidEdge) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (getDegree(roads, src, onlyValidEdge) == 1 && getDegree(roads, tgt, onlyValidEdge) == 1) {
			roads.graph[*ei]->valid = false;
			roads.graph[src]->valid = false;
			roads.graph[tgt]->valid = false;
		}
	}

	roads.setModified();
}

/**
 * Split the edge at the specified point.
 */
RoadVertexDesc GraphUtil::splitEdge(RoadGraph &roads, RoadEdgeDesc edge_desc, const QVector2D& pt) {
	RoadEdgePtr edge = roads.graph[edge_desc];

	// find which point along the polyline is the closest to the specified split point.
	int index;
	QVector2D pos;
	float min_dist = std::numeric_limits<float>::max();
	for (int i = 0; i < roads.graph[edge_desc]->polyline.size() - 1; i++) {
		QVector2D vec = roads.graph[edge_desc]->polyline[i + 1] - roads.graph[edge_desc]->polyline[i];
		float length = vec.length();
		for (int j = 0; j < length; j += 1.0f) {
			QVector2D pt2 = roads.graph[edge_desc]->polyline[i] + vec * (float)j / length;
			float dist = (pt2 - pt).lengthSquared();
			if (dist < min_dist) {
				min_dist = dist;
				index = i;
				pos = pt2;
			}
		}
	}

	RoadVertexDesc src = boost::source(edge_desc, roads.graph);
	RoadVertexDesc tgt = boost::target(edge_desc, roads.graph);

	// add a new vertex at the specified point on the edge
	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(pos));
	RoadVertexDesc v_desc = boost::add_vertex(roads.graph);
	roads.graph[v_desc] = v;

	// add the first edge
	RoadEdgePtr e1 = RoadEdgePtr(new RoadEdge(edge->type, edge->lanes, edge->oneWay));
	if ((edge->polyline[0] - roads.graph[src]->pt).lengthSquared() < (edge->polyline[0] - roads.graph[tgt]->pt).lengthSquared()) {
		for (int i = 0; i <= index; i++) {
			e1->addPoint(edge->polyline[i]);
		}
		e1->addPoint(pos);
	} else {
		e1->addPoint(pos);
		for (int i = index + 1; i < edge->polyline.size(); i++) {
			e1->addPoint(edge->polyline[i]);
		}
	}
	std::pair<RoadEdgeDesc, bool> edge_pair1 = boost::add_edge(src, v_desc, roads.graph);
	roads.graph[edge_pair1.first] = e1;

	// add the second edge
	RoadEdgePtr e2 = RoadEdgePtr(new RoadEdge(edge->type, edge->lanes, edge->oneWay));
	if ((edge->polyline[0] - roads.graph[src]->pt).lengthSquared() < (edge->polyline[0] - roads.graph[tgt]->pt).lengthSquared()) {
		e2->addPoint(pos);
		for (int i = index + 1; i < edge->polyline.size(); i++) {
			e2->addPoint(edge->polyline[i]);
		}
	} else {
		for (int i = 0; i <= index; i++) {
			e2->addPoint(edge->polyline[i]);
		}
		e2->addPoint(pos);
	}
	std::pair<RoadEdgeDesc, bool> edge_pair2 = boost::add_edge(v_desc, tgt, roads.graph);
	roads.graph[edge_pair2.first] = e2;

	// remove the original edge
	roads.graph[edge_desc]->valid = false;

	return v_desc;
}

/**
 * Check if there is an edge outing from v1 that is too close to the line v1 - v2.
 */
bool GraphUtil::hasCloseEdge(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2, float angle_threshold) {
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v1, roads->graph); ei != eend; ++ei) {
		if (!roads->graph[*ei]->valid) continue;

		RoadVertexDesc tgt = boost::target(*ei, roads->graph);

		float angle = Util::diffAngle(roads->graph[tgt]->pt - roads->graph[v1]->pt, roads->graph[v2]->pt - roads->graph[v1]->pt);
		if (angle < angle_threshold) return true;
	}

	return false;
}

/**
 * Check if the poly line intersects with the existing road segments.
 */
bool GraphUtil::isIntersect(RoadGraph &roads, std::vector<QVector2D>& polyline) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if (isIntersect(roads, roads.graph[*ei]->polyline, polyline)) return true;
	}

	return false;
}

bool GraphUtil::isIntersect(RoadGraph &roads, std::vector<QVector2D>& polyline, RoadEdgeDesc ignoreEdge) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (*ei == ignoreEdge) continue;

		if (isIntersect(roads, roads.graph[*ei]->polyline, polyline)) return true;
	}

	return false;
}

/**
 * Check if the two poly lines intersect with each other.
 */
bool GraphUtil::isIntersect(RoadGraph &roads, std::vector<QVector2D>& polyline1, std::vector<QVector2D>& polyline2) {
	for (int i = 0; i < polyline1.size() - 1; i++) {
		for (int j = 0; j < polyline2.size() - 1; j++) {
			float tab, tcd;
			QVector2D intPoint;
			if (Util::segmentSegmentIntersectXY(polyline1[i], polyline1[i + 1], polyline2[j], polyline2[j + 1], &tab, &tcd, true, intPoint)) {
				return true;
			}
		}
	}

	return false;
}

/**
 * Simplify a polyline.
 */
std::vector<QVector2D> GraphUtil::simplifyPolyLine(std::vector<QVector2D>& polyline, float threshold) {
	std::vector<QVector2D> ret;
	
	typedef boost::geometry::model::d2::point_xy<double> xy;
	boost::geometry::model::linestring<xy> line;
	for (int i = 0; i < polyline.size(); i++) {
		line.push_back(xy(polyline[i].x(), polyline[i].y()));
	}

	boost::geometry::model::linestring<xy> simplified;
	boost::geometry::simplify(line, simplified, threshold);

	for (int i = 0; i < simplified.size(); i++) {
		ret.push_back(QVector2D(simplified[i].x(), simplified[i].y()));
	}

	return ret;
}

/**
 * Remove the short edges.
 */
void GraphUtil::removeShortEdges(RoadGraph& roads, float threshold) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if (roads.graph[*ei]->getLength() <= threshold) {
			roads.graph[*ei]->valid = false;
		}
	}
}

/**
 * Remove the link edges.
 */
void GraphUtil::removeLinkEdges(RoadGraph& roads) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (roads.graph[*ei]->link) roads.graph[*ei]->valid = false;
	}
}

/**
 * Make the road graph realistic.
 * 1) remove isolated vertices.
 * 2) remove isolated edges.
 */
void GraphUtil::realize(RoadGraph& roads) {
	removeIsolatedVertices(roads);
	removeIsolatedEdges(roads);
}

/**
 * Make the edge finer by inserting more points along the polyline.
 */
Polyline2D GraphUtil::finerEdge(RoadGraph& roads, RoadEdgeDesc e, float step) {
	Polyline2D polyline;

	for (int i = 0; i < roads.graph[e]->polyline.size() - 1; i++) {
		QVector2D vec = roads.graph[e]->polyline[i + 1] - roads.graph[e]->polyline[i];
		float length = vec.length();
		for (int j = 0; j < length; j += step) {
			polyline.push_back(roads.graph[e]->polyline[i] + vec * (float)j / length);
		}
	}
	polyline.push_back(roads.graph[e]->polyline.last());

	return polyline;
}

/**
 * 指定された点から、指定されたエッジへの距離を返却する。
 */
float GraphUtil::distance(RoadGraph& roads, const QVector2D& pt, RoadEdgeDesc e, QVector2D &closestPt) {
	float min_dist = std::numeric_limits<float>::max();

	for (int i = 0; i < roads.graph[e]->polyline.size() - 1; ++i) {
		QVector2D closePt;
		float dist = Util::pointSegmentDistanceXY(roads.graph[e]->polyline[i], roads.graph[e]->polyline[i + 1], pt, closePt);
		if (dist < min_dist) {
			min_dist = dist;
			closestPt = closePt;
		}
	}

	return min_dist;
}

/**
 * Load the road from a file.
 */
void GraphUtil::loadRoads(RoadGraph& roads, const QString& filename, int roadType) {
	roads.clear();

	FILE* fp = fopen(filename.toUtf8().data(), "rb");

	QMap<uint, RoadVertexDesc> idToDesc;

	// Read the number of vertices
	unsigned int nVertices;
	fread(&nVertices, sizeof(unsigned int), 1, fp);

	// Read each vertex's information: desc, x, and y.
	for (int i = 0; i < nVertices; i++) {
		RoadVertexDesc id;
		float x, y;
		fread(&id, sizeof(RoadVertexDesc), 1, fp);
		fread(&x, sizeof(float), 1, fp);
		fread(&y, sizeof(float), 1, fp);

		RoadVertexPtr vertex = RoadVertexPtr(new RoadVertex(QVector2D(x, y)));

		RoadVertexDesc desc = boost::add_vertex(roads.graph);
		roads.graph[desc] = vertex;

		idToDesc[id] = desc;
	}

	// Read the number of edges
	unsigned int nEdges;
	fread(&nEdges, sizeof(unsigned int), 1, fp);

	// Read each edge's information: the descs of two vertices, road type, the number of lanes, the number of points along the polyline, and the coordinate of each point along the polyline.
	for (int i = 0; i < nEdges; i++) {
		unsigned int type, lanes, oneWay, link, roundabout;
		RoadVertexDesc id1, id2;
		fread(&id1, sizeof(RoadVertexDesc), 1, fp);
		fread(&id2, sizeof(RoadVertexDesc), 1, fp);

		RoadVertexDesc src = idToDesc[id1];
		RoadVertexDesc tgt = idToDesc[id2];

		fread(&type, sizeof(unsigned int), 1, fp);
		fread(&lanes, sizeof(unsigned int), 1, fp);
		fread(&oneWay, sizeof(unsigned int), 1, fp);
		fread(&link, sizeof(unsigned int), 1, fp);
		fread(&roundabout, sizeof(unsigned int), 1, fp);

		RoadEdgePtr edge = RoadEdgePtr(new RoadEdge(type, lanes, oneWay == 1, link == 1, roundabout == 1));

		unsigned int nPoints;
		fread(&nPoints, sizeof(unsigned int), 1, fp);

		for (int j = 0; j < nPoints; j++) {
			float x, y;
			fread(&x, sizeof(float), 1, fp);
			fread(&y, sizeof(float), 1, fp);

			edge->addPoint(QVector2D(x, y));
		}

		// 指定されたタイプの道路エッジのみを読み込む
		if (isRoadTypeMatched(edge->type, roadType)) {
			std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(src, tgt, roads.graph);
			roads.graph[edge_pair.first] = edge;
		}
	}

	fclose(fp);

	roads.setModified();
}

/**
 * Save the road to a file.
 */
void GraphUtil::saveRoads(RoadGraph& roads, const QString& filename) {
	FILE* fp = fopen(filename.toUtf8().data(), "wb");
	
	int nVertices = getNumVertices(roads);//boost::num_vertices(roads.graph);
	fwrite(&nVertices, sizeof(int), 1, fp);

	// 各頂点につき、ID、X座標、Y座標を出力する
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// isolatedの頂点は、保存しない
		if (getDegree(roads, *vi) == 0) continue;

		RoadVertexPtr v = roads.graph[*vi];
	
		RoadVertexDesc desc = *vi;
		float x = v->getPt().x();
		float y = v->getPt().y();
		fwrite(&desc, sizeof(RoadVertexDesc), 1, fp);
		fwrite(&x, sizeof(float), 1, fp);
		fwrite(&y, sizeof(float), 1, fp);
	}

	int nEdges = getNumEdges(roads);//boost::num_edges(roads.graph);
	fwrite(&nEdges, sizeof(int), 1, fp);

	// 各エッジにつき、２つの頂点の各ID、道路タイプ、レーン数、一方通行か、ポリラインを構成するポイント数、各ポイントのX座標とY座標を出力する
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadEdgePtr edge = roads.graph[*ei];

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		fwrite(&src, sizeof(RoadVertexDesc), 1, fp);
		fwrite(&tgt, sizeof(RoadVertexDesc), 1, fp);
		
		unsigned int type = edge->type;
		fwrite(&type, sizeof(unsigned int), 1, fp);

		unsigned int lanes = edge->lanes;
		fwrite(&lanes, sizeof(unsigned int), 1, fp);

		// oneWay? (1 / 0)
		unsigned int oneWay;
		if (edge->oneWay) {
			oneWay = 1;
		} else {
			oneWay = 0;
		}
		fwrite(&oneWay, sizeof(unsigned int), 1, fp);

		// link? (1 / 0)
		unsigned int link;
		if (edge->link) {
			link = 1;
		} else {
			link = 0;
		}
		fwrite(&link, sizeof(unsigned int), 1, fp);

		// roundabout? (1 / 0)
		unsigned int roundabout;
		if (edge->roundabout) {
			roundabout = 1;
		} else {
			roundabout = 0;
		}
		fwrite(&roundabout, sizeof(unsigned int), 1, fp);

		int nPoints = edge->polyline.size();
		fwrite(&nPoints, sizeof(int), 1, fp);

		for (int i = 0; i < edge->polyline.size(); i++) {
			float x = edge->polyline[i].x();
			float y = edge->polyline[i].y();
			fwrite(&x, sizeof(float), 1, fp);
			fwrite(&y, sizeof(float), 1, fp);
		}
	}

	fclose(fp);
}

/**
 * Copy the road graph.
 * Note: This function copies all the vertices and edges including the invalid ones. Thus, their IDs will be preserved.
 */
void GraphUtil::copyRoads(RoadGraph& srcRoads, RoadGraph& dstRoads) {
	dstRoads.clear();

	QMap<RoadVertexDesc, RoadVertexDesc> conv;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(srcRoads.graph); vi != vend; ++vi) {
		// Add a vertex
		RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(*srcRoads.graph[*vi]));
		new_v->valid = srcRoads.graph[*vi]->valid;
		RoadVertexDesc new_v_desc = boost::add_vertex(dstRoads.graph);
		dstRoads.graph[new_v_desc] = new_v;

		conv[*vi] = new_v_desc;
	}

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(srcRoads.graph); ei != eend; ++ei) {
		RoadVertexDesc src = boost::source(*ei, srcRoads.graph);
		RoadVertexDesc tgt = boost::target(*ei, srcRoads.graph);

		RoadVertexDesc new_src = conv[src];
		RoadVertexDesc new_tgt = conv[tgt];

		// Add an edge
		RoadEdgePtr new_e = RoadEdgePtr(new RoadEdge(*srcRoads.graph[*ei]));
		std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(new_src, new_tgt, dstRoads.graph);
		dstRoads.graph[edge_pair.first] = new_e;
	}

	dstRoads.setModified();
}

/**
 * Merge the 2nd road to the 1st road. As a result, the roads1 will be updated containing all the vertices and edges of roads2.
 */
void GraphUtil::mergeRoads(RoadGraph& roads1, RoadGraph& roads2) {
	QMap<RoadVertexDesc, RoadVertexDesc> conv;

	// copy vertices from the 2nd road to the 1st road
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads2.graph); vi != vend; ++vi) {
		if (!roads2.graph[*vi]->valid) continue;

		RoadVertexPtr v1 = RoadVertexPtr(new RoadVertex(*roads2.graph[*vi]));
		RoadVertexDesc v1_desc = boost::add_vertex(roads1.graph);
		roads1.graph[v1_desc] = v1;

		conv[*vi] = v1_desc;
	}

	// copy edges from the 2nd road to the 1st road
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads2.graph); ei != eend; ++ei) {
		if (!roads2.graph[*ei]->valid) continue;

		RoadVertexDesc src2 = boost::source(*ei, roads2.graph);
		RoadVertexDesc tgt2 = boost::target(*ei, roads2.graph);

		RoadVertexDesc src1 = conv[src2];
		RoadVertexDesc tgt1 = conv[tgt2];

		addEdge(roads1, src1, tgt1, roads2.graph[*ei]);
	}

	roads1.setModified();
}

/**
 * Connect roads1 to roads2 as much as possible.
 * roads1 will be updated. roads2 will be ramained as it is.
 */
void GraphUtil::connectRoads(RoadGraph& roads1, RoadGraph& roads2, float connect_threshold) {
	QMap<RoadVertexDesc, RoadVertexDesc> conv;

	// copy vertices from the 2nd road to the 1st road
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads2.graph); vi != vend; ++vi) {
		if (!roads2.graph[*vi]->valid) continue;

		RoadVertexPtr v1 = RoadVertexPtr(new RoadVertex(*roads2.graph[*vi]));
		RoadVertexDesc v1_desc = boost::add_vertex(roads1.graph);
		roads1.graph[v1_desc] = v1;

		conv[*vi] = v1_desc;
	}

	// copy edges from the 2nd road to the 1st road
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads2.graph); ei != eend; ++ei) {
		if (!roads2.graph[*ei]->valid) continue;

		RoadVertexDesc src2 = boost::source(*ei, roads2.graph);
		RoadVertexDesc tgt2 = boost::target(*ei, roads2.graph);

		if (isIntersect(roads1, roads2.graph[*ei]->polyline)) continue;

		RoadVertexDesc src1 = conv[src2];
		RoadVertexDesc tgt1 = conv[tgt2];

		addEdge(roads1, src1, tgt1, roads2.graph[*ei]);
	}

	// for each roads2 vertex, try to find the close vertex of roads1 to connect
	for (boost::tie(vi, vend) = boost::vertices(roads2.graph); vi != vend; ++vi) {
		if (!roads2.graph[*vi]->valid) continue;
		if (getDegree(roads1, conv[*vi]) > 1) continue;

		RoadVertexDesc v1_desc;
		if (getVertex(roads1, roads1.graph[conv[*vi]]->pt, connect_threshold, conv[*vi], v1_desc)) {
			if (!conv.contains(v1_desc)) {
				addEdge(roads1, v1_desc, conv[*vi], 1, 1, false);	// to be updated!!!
				continue;
			}
		}

		RoadEdgeDesc e1_desc;
		if (getEdge(&roads1, *vi, connect_threshold, e1_desc)) {
			RoadVertexDesc src = boost::source(e1_desc, roads1.graph);
			RoadVertexDesc tgt = boost::target(e1_desc, roads1.graph);

			if (!conv.contains(src) || !conv.contains(tgt)) {
				v1_desc = splitEdge(roads1, e1_desc, roads2.graph[*vi]->pt);
				addEdge(roads1, v1_desc, conv[*vi], 1, 1, false);	// to be updated!!!
			}
		}
	}

	roads1.setModified();
}

/**
 * Return the axix aligned bounding box of the road graph.
 */
BBox GraphUtil::getAABoundingBox(RoadGraph& roads) {
	BBox box;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		box.addPoint(roads.graph[*vi]->getPt());
	}

	return box;
}

/**
 * Return the bounding box of the road graph.
 * 
 * The bounding box is not necessarily aligned to X/Y-axis.
 * Algorithm: Rotate the road graph from -90 degree to 90 degree by 5 degree per step, compute the axix aligned bounding box, and find the minimum one in terms of its area.
 * The raod graph is updated to be rotated based on the bounding box in the end.
 */
BBox GraphUtil::getBoudingBox(RoadGraph& roads, float theta1, float theta2, float theta_step) {
	float min_area = std::numeric_limits<float>::max();
	float min_theta;
	BBox min_box;

	for (float theta = theta1; theta <= theta2; theta += theta_step) {
		RoadGraph rotated_roads;
		copyRoads(roads, rotated_roads);
		rotate(rotated_roads, theta);
		BBox box = getAABoundingBox(rotated_roads);
		if (box.dx() * box.dy() < min_area) {
			min_area = box.dx() * box.dy();
			min_theta = theta;
			min_box = box;
		}
	}

	rotate(roads, min_theta);
	return min_box;
}

/**
 * Extract the specified type of road segments.
 * Note that this function does not change neither the vertex desc nor the edge desc.
 */
void GraphUtil::extractRoads(RoadGraph& roads, int roadType) {
	if (roadType == 0) return;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if (!isRoadTypeMatched(roads.graph[*ei]->type, roadType)) {
			roads.graph[*ei]->valid = false;
		}
	}

	removeIsolatedVertices(roads);

	roads.setModified();
}

/**
 * Extract roads that reside in the specified area.
 * If "strict" is true, only the edges that are completely within the area will be extracted.
 * Note that this function does not change neither the vertex desc nor the edge desc.
 */
void GraphUtil::extractRoads(RoadGraph& roads, Polygon2D& area, bool strict, int roadType) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (isRoadTypeMatched(roads.graph[*ei]->type, roadType)) {
			if (strict) {
				// if either vertice is out of the range, invalidate this edge.
				if (!area.contains(roads.graph[src]->pt) || !area.contains(roads.graph[tgt]->pt)) {
					roads.graph[*ei]->valid = false;
				}
			} else {
				// if both the vertices is out of the range, invalidate this edge.
				if (!area.contains(roads.graph[src]->pt) && !area.contains(roads.graph[tgt]->pt)) {
					roads.graph[*ei]->valid = false;
				}
			}
		} else {
			roads.graph[*ei]->valid = false;
		}
	}

	removeIsolatedVertices(roads);

	roads.setModified();
}

/**
 * Extract roads that reside in the specified area.
 * If a edge is across the border of the area, add a vertex on the border and split the edge at the vertex.
 */
void GraphUtil::extractRoads2(RoadGraph& roads, const Polygon2D& area, int roadType) {
	QList<RoadEdgeDesc> edges;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (isRoadTypeMatched(roads.graph[*ei]->type, roadType)) {
			if (!area.contains(roads.graph[src]->pt) && !area.contains(roads.graph[tgt]->pt)) {
				roads.graph[*ei]->valid = false;
			} else if (!area.contains(roads.graph[src]->pt) || !area.contains(roads.graph[tgt]->pt)) {
				edges.push_back(*ei);
			}
		} else {
			roads.graph[*ei]->valid = false;
		}
	}

	for (int e_id = 0; e_id < edges.size(); e_id++) {
		RoadVertexDesc src = boost::source(edges[e_id], roads.graph);
		RoadVertexDesc tgt = boost::target(edges[e_id], roads.graph);

		// if either vertice is out of the range, add a vertex on the border
		std::vector<QVector2D> polyline = finerEdge(roads, edges[e_id]);
		QVector2D intPt;
		if (area.contains(polyline[0])) {
			for (int i = 1; i < polyline.size(); i++) {
				if (!area.contains(polyline[i])) {
					intPt = polyline[i];
					break;
				}
			}
		} else {
			for (int i = polyline.size() - 1; i >= 0; i--) {
				if (!area.contains(polyline[i])) {
					intPt = polyline[i];
					break;
				}
			}
		}

		RoadVertexDesc v = splitEdge(roads, edges[e_id], intPt);
		roads.graph[v]->onBoundary = true;
		if (area.contains(roads.graph[src]->pt)) {
			RoadEdgeDesc e = getEdge(roads, v, tgt);
			roads.graph[e]->valid = false;
		} else {
			RoadEdgeDesc e = getEdge(roads, v, src);
			roads.graph[e]->valid = false;
		}
	}

	removeIsolatedVertices(roads);

	roads.setModified();
}

/**
 * Subtract an area from the road graph.
 * If "strict" is true, only the edges that are completely within the area will be subtracted.
 * Note that this function does not change neighter the vertex desc nor the edge desc.
 */
void GraphUtil::subtractRoads(RoadGraph& roads, Polygon2D& area, bool strict) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (strict) {
			// if both the vertices is within the range, invalidate this edge.
			if (area.contains(roads.graph[src]->pt) && area.contains(roads.graph[tgt]->pt)) {
				roads.graph[*ei]->valid = false;
			}
		} else {
			// if either vertice is within the range, invalidate this edge.
			if (area.contains(roads.graph[src]->pt) || area.contains(roads.graph[tgt]->pt)) {
				roads.graph[*ei]->valid = false;
			}
		}
	}

	removeIsolatedVertices(roads);

	roads.setModified();
}

/**
 * Subtract an area from the road graph.
 */
void GraphUtil::subtractRoads2(RoadGraph& roads, Polygon2D& area) {
	QList<RoadEdgeDesc> edges;

	// list up all the edges that are across the border of the area
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		
		if (area.contains(roads.graph[src]->pt) && area.contains(roads.graph[tgt]->pt)) {
			roads.graph[*ei]->valid = false;
		} else if (area.contains(roads.graph[src]->pt) || area.contains(roads.graph[tgt]->pt)) {
			edges.push_back(*ei);
		}
	}

	for (int e_id = 0; e_id < edges.size(); e_id++) {
		RoadVertexDesc src = boost::source(edges[e_id], roads.graph);
		RoadVertexDesc tgt = boost::target(edges[e_id], roads.graph);

		// if either vertice is out of the range, add a vertex on the border
		std::vector<QVector2D> polyline = finerEdge(roads, edges[e_id], 3.0f);
		QVector2D intPt;
		if (area.contains(polyline[0])) {
			for (int i = 1; i < polyline.size(); i++) {
				if (!area.contains(polyline[i])) {
					intPt = polyline[i];
					break;
				}
			}
		} else {
			for (int i = polyline.size() - 1; i >= 0; i--) {
				if (!area.contains(polyline[i])) {
					intPt = polyline[i];
					break;
				}
			}
		}

		RoadVertexDesc v = splitEdge(roads, edges[e_id], intPt);
		if (area.contains(roads.graph[src]->pt)) {
			RoadEdgeDesc e = getEdge(roads, v, src);
			roads.graph[e]->valid = false;
		} else {
			RoadEdgeDesc e = getEdge(roads, v, tgt);
			roads.graph[e]->valid = false;
		}
	}

	removeIsolatedVertices(roads);
	reduce(roads);

	roads.setModified();
}

/**
 * 頂点を少しずらす。
 */
void GraphUtil::perturb(RoadGraph &roads, const Polygon2D &area, float factor) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// outing edgeの最短距離を求める
		float min_length = 1000.0f;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			float length = roads.graph[*ei]->getLength();
			if (length < min_length) {
				min_length = length;
			}
		}

		float dx = Util::genRand(-min_length * factor, min_length * factor);
		float dy = Util::genRand(-min_length * factor, min_length * factor);

		moveVertex(roads, *vi, roads.graph[*vi]->pt + QVector2D(dx, dy));
	}

	roads.setModified();
}

/**
 * Return the neighbors of the specified vertex.
 */
std::vector<RoadVertexDesc> GraphUtil::getNeighbors(RoadGraph& roads, RoadVertexDesc v, bool onlyValidVertex) {
	std::vector<RoadVertexDesc> neighbors;

	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
		if (onlyValidVertex && !roads.graph[*ei]->valid) continue;

		neighbors.push_back(boost::target(*ei, roads.graph));
	}

	return neighbors;
}

/**
 * v1とv2が隣接した頂点かどうかチェックする。
 */
bool GraphUtil::isNeighbor(RoadGraph& roads, RoadVertexDesc v1, RoadVertexDesc v2) {
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v1, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (boost::target(*ei, roads.graph) == v2) return true;
	}
	for (boost::tie(ei, eend) = boost::out_edges(v2, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (boost::target(*ei, roads.graph) == v1) return true;
	}

	return false;
}

/**
 * Check if desc2 is reachable from desc1.
 */
bool GraphUtil::isConnected(RoadGraph& roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge) {
	QList<RoadVertexDesc> seeds;
	QSet<RoadVertexDesc> visited;

	seeds.push_back(desc1);
	visited.insert(desc1);

	while (!seeds.empty()) {
		RoadVertexDesc v = seeds.front();
		seeds.pop_front();

		visited.insert(v);

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(v, roads.graph); ei != eend; ++ei) {
			if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

			RoadVertexDesc u = boost::target(*ei, roads.graph);
			if (onlyValidEdge && !roads.graph[u]->valid) continue;

			if (u == desc2) return true;

			if (!visited.contains(u)) seeds.push_back(u);			
		}
	}

	return false;
}

/**
 * Find the closest vertex from the specified point.
 */
/*RoadVertexDesc GraphUtil::findNearestVertex(RoadGraph* roads, const QVector2D &pt) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads->graph); vi != vend; ++vi) {
		if (!roads->graph[*vi]->valid) continue;

		float dist = (roads->graph[*vi]->getPt() - pt).length();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}*/

/**
 * Find the closest vertex from the specified point.
 * The vertex "ignore" is ignored.
 */
/*RoadVertexDesc GraphUtil::findNearestVertex(RoadGraph* roads, const QVector2D &pt, RoadVertexDesc ignore) {
	RoadVertexDesc nearest_desc;
	float min_dist = std::numeric_limits<float>::max();

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads->graph); vi != vend; ++vi) {
		if (*vi == ignore) continue;
		if (!roads->graph[*vi]->valid) continue;

		float dist = (roads->graph[*vi]->getPt() - pt).length();
		if (dist < min_dist) {
			nearest_desc = *vi;
			min_dist = dist;
		}
	}

	return nearest_desc;
}*/

/**
 * 指定したノードvと接続されたノードの中で、指定した座標に最も近いノードを返却する。
 */
RoadVertexDesc GraphUtil::findConnectedNearestNeighbor(RoadGraph* roads, const QVector2D &pt, RoadVertexDesc v) {
	QMap<RoadVertexDesc, bool> visited;
	std::list<RoadVertexDesc> seeds;
	seeds.push_back(v);

	float min_dist = std::numeric_limits<float>::max();
	RoadVertexDesc min_desc;

	while (!seeds.empty()) {
		RoadVertexDesc seed = seeds.front();
		seeds.pop_front();

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(seed, roads->graph); ei != eend; ++ei) {
			if (!roads->graph[*ei]->valid) continue;

			RoadVertexDesc v2 = boost::target(*ei, roads->graph);
			if (visited.contains(v2)) continue;

			// 指定したノードvは除く（除かない方が良いのか？検討中。。。。）
			//if (v2 == v) continue;

			visited[v2] = true;

			// 指定した座標との距離をチェック
			float dist = (roads->graph[v2]->getPt() - pt).lengthSquared();
			if (dist < min_dist) {
				min_dist = dist;
				min_desc = v2;
			}

			seeds.push_back(v2);
		}
	}

	return min_desc;
}

/**
 * Find the edge which is the closest to the specified point.
 * If the distance is within the threshold, return true. Otherwise, return false.
 */
bool GraphUtil::getEdge(RoadGraph &roads, const QVector2D &pt, float threshold, RoadEdgeDesc& e, bool onlyValidEdge) {
	float min_dist = std::numeric_limits<float>::max();
	RoadEdgeDesc min_e;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		RoadVertexPtr src = roads.graph[boost::source(*ei, roads.graph)];
		RoadVertexPtr tgt = roads.graph[boost::target(*ei, roads.graph)];

		if (onlyValidEdge && !src->valid) continue;
		if (onlyValidEdge && !tgt->valid) continue;

		QVector2D pt2;
		for (int i = 0; i < roads.graph[*ei]->polyline.size() - 1; i++) {
			float dist = Util::pointSegmentDistanceXY(roads.graph[*ei]->polyline[i], roads.graph[*ei]->polyline[i + 1], pt, pt2);
			if (dist < min_dist) {
				min_dist = dist;
				e = *ei;
			}
		}
	}

	if (min_dist < threshold) return true;
	else return false;
}

/**
 * 指定された点に近いエッジを探す。ただし、指定された頂点から出るエッジは検索対象外とする。
 */
bool GraphUtil::getEdge(RoadGraph& roads, const QVector2D &pt, RoadVertexDesc ignore, float threshold, RoadEdgeDesc& e, bool onlyValidEdge) {
	float min_dist = std::numeric_limits<float>::max();
	RoadEdgeDesc min_e;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (onlyValidEdge && !roads.graph[src]->valid) continue;
		if (onlyValidEdge && !roads.graph[tgt]->valid) continue;

		// 検索対象外のエッジなら、スキップ
		if (src == ignore || tgt == ignore) continue;

		QVector2D pt2;
		for (int i = 0; i < roads.graph[*ei]->polyline.size() - 1; i++) {
			float dist = Util::pointSegmentDistanceXY(roads.graph[*ei]->polyline[i], roads.graph[*ei]->polyline[i + 1], pt, pt2);
			if (dist < min_dist) {
				min_dist = dist;
				e = *ei;
			}
		}
	}

	if (min_dist < threshold) return true;
	else return false;
}

/**
 * Find the edge which is the closest to the specified vertex.
 * If the distance is within the threshold, return true. Otherwise, return false.
 */
bool GraphUtil::getEdge(RoadGraph* roads, RoadVertexDesc v, float threshold, RoadEdgeDesc& e, bool onlyValidEdge) {
	float min_dist = std::numeric_limits<float>::max();
	RoadEdgeDesc min_e;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads->graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads->graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads->graph);
		RoadVertexDesc tgt = boost::target(*ei, roads->graph);

		if (src == v || tgt == v) continue;

		if (onlyValidEdge && !roads->graph[src]->valid) continue;
		if (onlyValidEdge && !roads->graph[tgt]->valid) continue;

		QVector2D pt2;
		for (int i = 0; i < roads->graph[*ei]->polyline.size() - 1; i++) {
			float dist = Util::pointSegmentDistanceXY(roads->graph[*ei]->polyline[i], roads->graph[*ei]->polyline[i + 1], roads->graph[v]->pt, pt2);
			if (dist < min_dist) {
				min_dist = dist;
				e = *ei;
			}
		}
	}

	if (min_dist < threshold) return true;
	else return false;
}

/**
 * 指定された頂点に最も近いエッジを返却する。
 * ただし、指定された頂点に隣接するエッジは、対象外とする。
 */
RoadEdgeDesc GraphUtil::findNearestEdge(RoadGraph* roads, RoadVertexDesc v, float& dist, QVector2D &closestPt, bool onlyValidEdge) {
	dist = std::numeric_limits<float>::max();
	RoadEdgeDesc min_e;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads->graph); ei != eend; ++ei) {
		if (onlyValidEdge && !roads->graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads->graph);
		RoadVertexDesc tgt = boost::target(*ei, roads->graph);
		if (v == src || v == tgt) continue;

		if (onlyValidEdge && !roads->graph[src]->valid) continue;
		if (onlyValidEdge && !roads->graph[tgt]->valid) continue;

		QVector2D pt2;
		float d = Util::pointSegmentDistanceXY(roads->graph[src]->getPt(), roads->graph[tgt]->getPt(), roads->graph[v]->getPt(), pt2);
		if (d < dist) {
			dist = d;
			min_e = *ei;
			closestPt = pt2;
		}
	}

	return min_e;
}

/**
 * Clean the road graph by removing all the invalid vertices and edges.
 */
void GraphUtil::clean(RoadGraph& roads) {
	RoadGraph temp;
	GraphUtil::copyRoads(roads, temp);

	roads.clear();

	removeIsolatedVertices(temp);

	QMap<RoadVertexDesc, RoadVertexDesc> conv;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(temp.graph); vi != vend; ++vi) {
		if (!temp.graph[*vi]->valid) continue;

		// Add a vertex
		RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(*temp.graph[*vi]));
		RoadVertexDesc new_v_desc = boost::add_vertex(roads.graph);
		roads.graph[new_v_desc] = new_v;	

		conv[*vi] = new_v_desc;
	}

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(temp.graph); ei != eend; ++ei) {
		if (!temp.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, temp.graph);
		RoadVertexDesc tgt = boost::target(*ei, temp.graph);

		RoadVertexDesc new_src = conv[src];
		RoadVertexDesc new_tgt = conv[tgt];

		// Add an edge
		RoadEdgePtr new_e = RoadEdgePtr(new RoadEdge(*temp.graph[*ei]));
		std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(new_src, new_tgt, roads.graph);
		roads.graph[edge_pair.first] = new_e;
	}

	roads.setModified();
}

/**
 * Remove the vertices of degree of 2, and make it as a part of an edge.
 */
void GraphUtil::reduce(RoadGraph& roads) {
	bool actuallReduced = false;

	RoadVertexIter vi, vend;
	bool deleted = false;
	do {
		deleted = false;

		for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
			if (!roads.graph[*vi]->valid) continue;

			RoadVertexPtr v = roads.graph[*vi];

			if (getDegree(roads, *vi) == 2) {
				if (reduce(roads, *vi)) {
					deleted = true;
					actuallReduced = true;
					break;
				}
			}
		}
	} while (deleted);

	if (actuallReduced) {
		roads.setModified();
	}
}

/**
 * Remove the vertex of degree 2, and make it as a part of an edge.
 */
bool GraphUtil::reduce(RoadGraph& roads, RoadVertexDesc desc) {
	int count = 0;
	RoadVertexDesc vd[2];
	RoadEdgeDesc ed[2];
	RoadEdgePtr edges[2];

	RoadOutEdgeIter ei, ei_end;
	for (boost::tie(ei, ei_end) = out_edges(desc, roads.graph); ei != ei_end; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		vd[count] = boost::target(*ei, roads.graph);
		ed[count] = *ei;
		edges[count] = roads.graph[*ei];
		count++;
	}

	if (edges[0]->type != edges[1]->type) return false;
	//if (edges[0]->lanes != edges[1]->lanes) return false;

	// If the vertices form a triangle, don't remove it.
	//if (hasEdge(roads, vd[0], vd[1])) return false;

	RoadEdgePtr new_edge = RoadEdgePtr(new RoadEdge(edges[0]->type, edges[0]->lanes, edges[0]->oneWay));
	orderPolyLine(roads, ed[0], vd[0]);
	orderPolyLine(roads, ed[1], desc);
	
	for (int i = 0; i < edges[0]->polyline.size(); i++) {
		new_edge->addPoint(edges[0]->polyline[i]);
	}
	for (int i = 1; i < edges[1]->polyline.size(); i++) {
		new_edge->addPoint(edges[1]->polyline[i]);
	}
	std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(vd[0], vd[1], roads.graph);
	roads.graph[edge_pair.first] = new_edge;

	// invalidate the old edge
	roads.graph[ed[0]]->valid = false;
	roads.graph[ed[1]]->valid = false;

	// invalidate the vertex
	roads.graph[desc]->valid = false;

	return true;
}

/**
 * ノード間の距離が指定した距離よりも近い場合は、１つにしてしまう。
 * ノードとエッジ間の距離が、閾値よりも小さい場合も、エッジ上にノードを移してしまう。
 */
void GraphUtil::simplify(RoadGraph& roads, float dist_threshold) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		while (true) {
			//RoadVertexDesc v2 = findNearestVertex(roads, roads->graph[*vi]->getPt(), *vi);
			RoadVertexDesc v2;
			if (!getVertex(roads, roads.graph[*vi]->pt, dist_threshold, *vi, v2)) break;
			//if ((roads->graph[v2]->getPt() - roads->graph[*vi]->getPt()).length() > dist_threshold) break;

			// define the new position
			QVector2D pt;
			int degree1 = getDegree(roads, *vi);
			int degree2 = getDegree(roads, v2);
			if (degree1 > 2 && degree2 <= 2) {
				pt = roads.graph[*vi]->pt;
			} else if (degree1 <= 2 && degree2 > 2) {
				pt = roads.graph[v2]->pt;
			} else {
				pt = (roads.graph[*vi]->pt + roads.graph[v2]->pt) / 2.0f;
			}

			moveVertex(roads, *vi, pt);
			snapVertex(roads, v2, *vi);
		}

		// find the closest vertex
		QVector2D closestPt;
		float dist;
		RoadEdgeDesc e = GraphUtil::findNearestEdge(&roads, *vi, dist, closestPt);
		if (dist < dist_threshold) {
			// move the vertex to the closest point on the edge
			GraphUtil::moveVertex(roads, *vi, closestPt);

			// retrieve src and tgt of the edge
			RoadVertexDesc src = boost::source(e, roads.graph);
			RoadVertexDesc tgt = boost::target(e, roads.graph);

			// invalidate the edge
			roads.graph[e]->valid = false;

			// update the edge
			if (!GraphUtil::hasEdge(roads, src, *vi)) {
				addEdge(roads, src, *vi, roads.graph[e]->type, roads.graph[e]->lanes, roads.graph[e]->oneWay);
			}
			if (!GraphUtil::hasEdge(roads, tgt, *vi)) {
				addEdge(roads, tgt, *vi, roads.graph[e]->type, roads.graph[e]->lanes, roads.graph[e]->oneWay);
			}
		}
	}

	roads.setModified();
}

/**
 * ノード間の距離が指定した距離よりも近い場合は、１つにしてしまう。
 * ノードとエッジ間の距離が、閾値よりも小さい場合も、エッジ上にノードを移してしまう。
 */
void GraphUtil::simplify2(RoadGraph& roads, float dist_threshold) {
	float threshold2 = dist_threshold * dist_threshold;

	RoadGraph temp;
	copyRoads(roads, temp);

	roads.clear();

	// 全ての頂点同士で、近いものをグループ化する
	int group_id = 0;
	std::vector<QVector2D> group_centers;
	std::vector<int> group_nums;
	QHash<RoadVertexDesc, int> groups;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(temp.graph); vi != vend; ++vi) {
		if (!temp.graph[*vi]->valid) continue;

		float min_dist = std::numeric_limits<float>::max();
		int min_group_id = -1;
		for (int i = 0; i < group_centers.size(); i++) {
			float dist = (group_centers[i] - temp.graph[*vi]->pt).lengthSquared();
			if (dist < min_dist) {
				min_dist = dist;
				min_group_id = i;
			}
		}

		if (min_group_id >= 0 && min_dist < threshold2) {
			group_centers[min_group_id] = group_centers[min_group_id] * group_nums[min_group_id] + temp.graph[*vi]->pt;
			group_nums[min_group_id]++;
			group_centers[min_group_id] /= group_nums[min_group_id];
		} else {
			min_group_id = group_centers.size();
			group_centers.push_back(temp.graph[*vi]->pt);
			group_nums.push_back(1);
		}
		
		groups[*vi] = min_group_id;
	}

	// エッジを登録する
	QHash<int, RoadVertexDesc> conv;	// group center ⇒ 実際の頂点desc
	RoadGraph temp2;
	for (int i = 0; i < group_centers.size(); i++) {
		RoadVertexDesc v = boost::add_vertex(temp2.graph);
		temp2.graph[v] = RoadVertexPtr(new RoadVertex(group_centers[i]));
		conv[i] = v;
	}

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(temp.graph); ei != eend; ++ei) {
		if (!temp.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, temp.graph);
		RoadVertexDesc tgt = boost::target(*ei, temp.graph);

		RoadVertexDesc new_src = conv[groups[src]];
		RoadVertexDesc new_tgt = conv[groups[tgt]];

		// エッジが点に縮退する場合は、スキップ
		if (new_src == new_tgt) continue;

		// 既にエッジがあれば、スキップ
		if (hasEdge(temp2, new_src, new_tgt)) continue;

		// エッジを追加
		RoadEdgeDesc e = addEdge(temp2, new_src, new_tgt, temp.graph[*ei]);
		moveEdge(temp2, e, temp2.graph[new_src]->pt, temp2.graph[new_tgt]->pt);
	}

	copyRoads(temp2, roads);

	roads.setModified();
}

/**
 * エッジのポリゴンが3つ以上で構成されている場合、中間点を全てノードとして登録する。
 */
void GraphUtil::normalize(RoadGraph& roads) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		if (roads.graph[*ei]->polyline.size() <= 2) continue;

		// invalidate the edge
		roads.graph[*ei]->valid = false;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		RoadVertexDesc prev_desc;
		RoadVertexDesc last_desc;
		if ((roads.graph[src]->getPt() - roads.graph[*ei]->polyline[0]).lengthSquared() < (roads.graph[tgt]->getPt() - roads.graph[*ei]->polyline[0]).lengthSquared()) {
			prev_desc = src;
			last_desc = tgt;
		} else {
			prev_desc = tgt;
			last_desc = src;
		}

		for (int i = 1; i < roads.graph[*ei]->polyline.size() - 1; i++) {
			// add all the points along the poly line as vertices
			RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(roads.graph[*ei]->polyline[i]));
			RoadVertexDesc new_v_desc = boost::add_vertex(roads.graph);
			roads.graph[new_v_desc] = new_v;

			// Add an edge
			addEdge(roads, prev_desc, new_v_desc, roads.graph[*ei]->type, roads.graph[*ei]->lanes, roads.graph[*ei]->oneWay);

			prev_desc = new_v_desc;
		}

		// Add the last edge
		addEdge(roads, prev_desc, last_desc, roads.graph[*ei]->type, roads.graph[*ei]->lanes, roads.graph[*ei]->oneWay);
	}
}

/**
 * エッジを指定されたstep_sizeで分割し、それぞれの点を全て頂点として登録する。
 */
void GraphUtil::normalize(RoadGraph& roads, float step_size) {
	RoadGraph temp;
	copyRoads(roads, temp);

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		roads.graph[*ei]->valid = false;
	}
	
	for (boost::tie(ei, eend) = boost::edges(temp.graph); ei != eend; ++ei) {
		if (!temp.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, temp.graph);
		RoadVertexDesc tgt = boost::target(*ei, temp.graph);

		std::vector<QVector2D> line = finerEdge(temp, *ei, step_size);

		RoadVertexDesc prev_desc;
		RoadVertexDesc last_desc;
		if ((temp.graph[src]->pt - line[0]).lengthSquared() < (temp.graph[tgt]->pt - line[0]).lengthSquared()) {
			prev_desc = src;
			last_desc = tgt;
		} else {
			prev_desc = tgt;
			last_desc = src;
		}

		for (int i = 1; i < line.size() - 1; i++) {
			// add all the points along the poly line as vertices
			RoadVertexDesc new_v_desc = boost::add_vertex(roads.graph);
			roads.graph[new_v_desc] = RoadVertexPtr(new RoadVertex(line[i]));

			// Add an edge
			addEdge(roads, prev_desc, new_v_desc, temp.graph[*ei]->type, temp.graph[*ei]->lanes, temp.graph[*ei]->oneWay);

			prev_desc = new_v_desc;
		}

		// Add the last edge
		addEdge(roads, prev_desc, last_desc, temp.graph[*ei]->type, temp.graph[*ei]->lanes, temp.graph[*ei]->oneWay);
	}
}

/**
 * start頂点と接続されているノードのみ有効とし、それ以外のノード、およびエッジを、全て無効にする。
 * 本実装では、事前の有効・無効フラグを考慮していない。要検討。。。
 */
void GraphUtil::singlify(RoadGraph& roads) {
	int max_size = 0;
	RoadVertexDesc start;

	// 最も大きいかたまり（接続されている）の道路網を探し出す
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		int size = getNumConnectedVertices(roads, *vi);
		if (size > max_size) {
			max_size = size;
			start = *vi;
		}
	}

	RoadGraph new_roads;

	std::list<RoadVertexDesc> queue;
	queue.push_back(start);

	QMap<RoadVertexDesc, RoadVertexDesc> conv;

	// Add the starting vertex
	RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(roads.graph[start]->getPt()));
	RoadVertexDesc new_v_desc = boost::add_vertex(new_roads.graph);
	new_roads.graph[new_v_desc] = new_v;

	conv[start] = new_v_desc;

	std::list<RoadVertexDesc> queue2;
	queue2.push_back(new_v_desc);

	while (!queue.empty()) {
		RoadVertexDesc v_desc = queue.front();
		queue.pop_front();

		RoadVertexDesc new_v_desc = queue2.front();
		queue2.pop_front();


		RoadOutEdgeIter oei, oeend;
		for (boost::tie(oei, oeend) = boost::out_edges(v_desc, roads.graph); oei != oeend; ++oei) {
			if (!roads.graph[*oei]->valid) continue;

			RoadVertexDesc u_desc = boost::target(*oei, roads.graph);
			if (!roads.graph[u_desc]->valid) continue;

			RoadVertexDesc new_u_desc;

			if (conv.contains(u_desc)) {
				new_u_desc = conv[u_desc];
			} else {
				// Add a vertex
				RoadVertexPtr new_u = RoadVertexPtr(new RoadVertex(roads.graph[u_desc]->getPt()));
				new_u_desc = boost::add_vertex(new_roads.graph);
				new_roads.graph[new_u_desc] = new_u;
			}

			// Add an edge
			if (!hasEdge(new_roads, new_v_desc, new_u_desc)) {
				addEdge(new_roads, new_v_desc, new_u_desc, roads.graph[*oei]);
			}

			if (!conv.contains(u_desc)) {
				conv[u_desc] = new_u_desc;
				queue.push_back(u_desc);
				queue2.push_back(new_u_desc);
			}
		}
	}

	// copy new_roads to roads
	copyRoads(new_roads, roads);
}

/**
 * Convert the road graph to a planar graph.
 */
void GraphUtil::planarify(RoadGraph& roads) {
	clock_t start, end;
	start = clock();

	bool split = true;

	while (split) {
		split = planarifyOne(roads);
	}

	end = clock();
	std::cout << "planarify: " << (double)(end-start)/CLOCKS_PER_SEC << std::endl;
}

/**
 * Convert one intersected road segments to a planar one by adding the intersection, and return true.
 * If the road segments do not intersect, return false.
 */
bool GraphUtil::planarifyOne(RoadGraph& roads) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		RoadEdgePtr e = roads.graph[*ei];
		if (!e->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		RoadEdgeIter ei2, eend2;
		for (boost::tie(ei2, eend2) = boost::edges(roads.graph); ei2 != eend2; ++ei2) {
			RoadEdgePtr e2 = roads.graph[*ei2];
			if (!e2->valid) continue;

			RoadVertexDesc src2 = boost::source(*ei2, roads.graph);
			RoadVertexDesc tgt2 = boost::target(*ei2, roads.graph);

			//if ((src == src2 && tgt == tgt2) || (src == tgt2 && tgt == src2)) continue;
			if (src == src2 || src == tgt2 || tgt == src2 || tgt == tgt2) continue;

			for (int i = 0; i < e->polyline.size() - 1; i++) {
				for (int j = 0; j < e2->polyline.size() - 1; j++) {
					float tab, tcd;
					QVector2D intPt;
					if (Util::segmentSegmentIntersectXY(e->polyline[i], e->polyline[i+1], e2->polyline[j], e2->polyline[j+1], &tab, &tcd, true, intPt)) {
						// エッジの端、ぎりぎりで、交差する場合は、交差させない
						if ((roads.graph[src]->pt - intPt).length() < 10 || (roads.graph[tgt]->pt - intPt).length() < 10 || (roads.graph[src2]->pt - intPt).length() < 10 || (roads.graph[tgt2]->pt - intPt).length() < 10) continue;

						// 交点をノードとして登録
						RoadVertexDesc new_v_desc = splitEdge(roads, *ei, intPt);
						RoadVertexDesc new_v_desc2 = splitEdge(roads, *ei2, intPt);



						// 交点をノードとして登録
						//RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(intPt));
						//RoadVertexDesc new_v_desc = boost::add_vertex(roads.graph);
						//roads.graph[new_v_desc] = new_v;

						// もともとのエッジを無効にする
						roads.graph[*ei]->valid = false;
						roads.graph[*ei2]->valid = false;

						// スナップする
						snapVertex(roads, new_v_desc2, new_v_desc);

						// 新たなエッジを追加する
						//addEdge(roads, src, new_v_desc, roads.graph[*ei]->type, roads.graph[*ei]->lanes, roads.graph[*ei]->oneWay);
						//addEdge(roads, new_v_desc, tgt, roads.graph[*ei]->type, roads.graph[*ei]->lanes, roads.graph[*ei]->oneWay);

						//addEdge(roads, src2, new_v_desc, roads.graph[*ei2]->type, roads.graph[*ei2]->lanes, roads.graph[*ei2]->oneWay);
						//addEdge(roads, new_v_desc, tgt2, roads.graph[*ei2]->type, roads.graph[*ei2]->lanes, roads.graph[*ei2]->oneWay);

						return true;
					}
				}
			}
		}
	}

	return false;
}

/**
 * 道路網をスケルトン化する。
 * 具体的には、オリジナル道路網で、degreeが1の頂点と、その隣接エッジを無効にする。
 * 注意：頂点を削除した結果、新たにdegreeが1となる頂点は、その対象ではない。
 */
void GraphUtil::skeltonize(RoadGraph* roads) {
	QList<RoadVertexDesc> list;

	// 削除対象となる頂点リストを取得
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads->graph); vi != vend; ++vi) {
		if (!roads->graph[*vi]->valid) continue;

		if (getDegree(*roads, *vi) == 1) {
			list.push_back(*vi);
		}
	}

	for (int i = 0; i < list.size(); i++) {
		// 隣接エッジを無効にする
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(list[i], roads->graph); ei != eend; ++ei) {
			if (!roads->graph[*ei]->valid) continue;

			roads->graph[*ei]->valid = false;
		}

		// 頂点を無効にする
		roads->graph[list[i]]->valid = false;
	}
}

/**
 * Rotate the road graph by theta [rad].
 */
void GraphUtil::rotate(RoadGraph& roads, float theta, const QVector2D& rotationCenter) {
	// Rotate vertices
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		QVector2D pos = roads.graph[*vi]->pt;

		roads.graph[*vi]->pt.setX(cosf(theta) * (pos.x() - rotationCenter.x()) - sinf(theta) * (pos.y() - rotationCenter.y()) + rotationCenter.x());
		roads.graph[*vi]->pt.setY(sinf(theta) * (pos.x() - rotationCenter.x()) + cosf(theta) * (pos.y() - rotationCenter.y()) + rotationCenter.y());
	}

	// Rotate edges
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		for (int i = 0; i < roads.graph[*ei]->polyline.size(); i++) {
			QVector2D pos = roads.graph[*ei]->polyline[i];
			roads.graph[*ei]->polyline[i].setX(cosf(theta) * (pos.x() - rotationCenter.x()) - sinf(theta) * (pos.y() - rotationCenter.y()) + rotationCenter.x());
			roads.graph[*ei]->polyline[i].setY(sinf(theta) * (pos.x() - rotationCenter.x()) + cosf(theta) * (pos.y() - rotationCenter.y()) + rotationCenter.y());
		}
	}

	roads.setModified();
}

/**
 * Translate the road graph.
 */
void GraphUtil::translate(RoadGraph& roads, const QVector2D& offset) {
	// Translate vertices
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		roads.graph[*vi]->pt += offset;
	}

	// Translate edges
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		for (int i = 0; i < roads.graph[*ei]->polyline.size(); i++) {
			roads.graph[*ei]->polyline[i] += offset;
		}
	}

	roads.setModified();
}

/**
 * Scale the road graph
 */
void GraphUtil::scale(RoadGraph& roads, const BBox& bbox1, const BBox& bbox2) {
	float scaleX = bbox2.dx() / bbox1.dx();
	float scaleY = bbox2.dy() / bbox1.dy();

	// Translate vertices
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		QVector2D vec = roads.graph[*vi]->pt - bbox1.minPt;
		float x = vec.x() * scaleX + bbox2.minPt.x();
		float y = vec.y() * scaleY + bbox2.minPt.y();
		roads.graph[*vi]->pt.setX(x);
		roads.graph[*vi]->pt.setY(y);
	}

	// Translate edges
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		for (int i = 0; i < roads.graph[*ei]->polyline.size(); i++) {
			QVector2D vec = roads.graph[*ei]->polyline[i] - bbox1.minPt;
			float x = vec.x() * scaleX + bbox2.minPt.x();
			float y = vec.y() * scaleY + bbox2.minPt.y();

			roads.graph[*ei]->polyline[i].setX(x);
			roads.graph[*ei]->polyline[i].setY(y);
		}
	}

	roads.setModified();
}

/**
 * snap the dead-end edges to the near vertices.
 * First, for vertices of degree more than 1, find the closest vertex.
 * If no such vertex exists, for vertices of degree 1, find the cloest vertex.
 */
void GraphUtil::snapDeadendEdges(RoadGraph& roads, float threshold) {
	float min_angle_threshold = 0.34f;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// only for the vertices of degree more than 1
		if (GraphUtil::getDegree(roads, *vi) != 1) continue;

		// retrieve the tgt vertex
		RoadVertexDesc tgt;
		RoadEdgeDesc e_desc;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			tgt = boost::target(*ei, roads.graph);
			e_desc = *ei;
			break;
		}

		// find the closest vertex
		RoadVertexDesc nearest_desc;
		float min_dist = std::numeric_limits<float>::max();

		RoadVertexIter vi2, vend2;
		for (boost::tie(vi2, vend2) = boost::vertices(roads.graph); vi2 != vend2; ++vi2) {
			if (!roads.graph[*vi2]->valid) continue;
			if (*vi2 == *vi) continue;
			if (*vi2 == tgt) continue;
			if (GraphUtil::getDegree(roads, *vi2) == 1) continue;

			float dist = (roads.graph[*vi2]->pt - roads.graph[*vi]->pt).length();

			// 近接頂点が、*viよりもtgtの方に近い場合は、当該近接頂点は対象からはずす
			float dist2 = (roads.graph[*vi2]->pt - roads.graph[tgt]->pt).length();
			if (dist > dist2) continue;

			if (dist < min_dist) {
				nearest_desc = *vi2;
				min_dist = dist;
			}

			// *vi2から出るエッジとのなす角度の最小値が小さすぎる場合は、対象からはずす
			float min_angle = std::numeric_limits<float>::max();
			RoadOutEdgeIter ei, eend;
			for (boost::tie(ei, eend) = boost::out_edges(*vi2, roads.graph); ei != eend; ++ei) {
				if (!roads.graph[*ei]->valid) continue;

				RoadVertexDesc tgt2 = boost::target(*ei, roads.graph);
				float angle = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[tgt]->pt, roads.graph[*vi2]->pt - roads.graph[tgt2]->pt);
				if (angle < min_angle) {
					min_angle = angle;
				}
			}
			if (min_angle < min_angle_threshold) continue;
		}

		// If no such vertex exists, find the closest vertex of degree 1.
		if (min_dist > threshold) {
			for (boost::tie(vi2, vend2) = boost::vertices(roads.graph); vi2 != vend2; ++vi2) {
				if (!roads.graph[*vi2]->valid) continue;
				if (*vi2 == *vi) continue;
				if (*vi2 == tgt) continue;
				if (GraphUtil::getDegree(roads, *vi2) != 1) continue;

				// Find the edge of the vertex
				RoadEdgeDesc e_desc2;
				RoadOutEdgeIter ei, eend;
				for (boost::tie(ei, eend) = boost::out_edges(*vi2, roads.graph); ei != eend; ++ei) {
					if (!roads.graph[*ei]->valid) continue;

					e_desc2 = *ei;
					break;
				}

				// If th edge is too short, skip it.
				//if (roads->graph[e_desc2]->getLength() < deadend_removal_threshold) continue;

				float dist = (roads.graph[*vi2]->pt - roads.graph[*vi]->pt).length();

				// 近接頂点が、*viよりもtgtの方に近い場合は、当該近接頂点は対象からはずす
				float dist2 = (roads.graph[*vi2]->pt - roads.graph[tgt]->pt).length();
				if (dist > dist2) continue;

				if (dist < min_dist) {
					nearest_desc = *vi2;
					min_dist = dist;
				}

				// *vi2から出るエッジとのなす角度の最小値が小さすぎる場合は、対象からはずす
				float min_angle = std::numeric_limits<float>::max();
				for (boost::tie(ei, eend) = boost::out_edges(*vi2, roads.graph); ei != eend; ++ei) {
					if (!roads.graph[*ei]->valid) continue;

					RoadVertexDesc tgt2 = boost::target(*ei, roads.graph);
					float angle = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[tgt]->pt, roads.graph[*vi2]->pt - roads.graph[tgt2]->pt);
					if (angle < min_angle) {
						min_angle = angle;
					}
				}
				if (min_angle < min_angle_threshold) continue;
			}
		}

		// 当該頂点と近接頂点との距離が、snap_deadend_threshold未満か？
		if (min_dist <= threshold) {
			// 一旦、古いエッジを、近接頂点にスナップするよう移動する
			GraphUtil::moveEdge(roads, e_desc, roads.graph[nearest_desc]->pt, roads.graph[tgt]->pt);

			if (GraphUtil::hasEdge(roads, nearest_desc, tgt, false)) {
				// もともとエッジがあるが無効となっている場合、それを有効にし、エッジのポリラインを更新する
				RoadEdgeDesc new_e_desc = GraphUtil::getEdge(roads, nearest_desc, tgt, false);
				roads.graph[new_e_desc]->valid = true;
				roads.graph[new_e_desc]->polyline = roads.graph[e_desc]->polyline;
			} else {
				// 該当頂点間にエッジがない場合は、新しいエッジを追加する
				GraphUtil::addEdge(roads, nearest_desc, tgt, roads.graph[e_desc]);
			}

			// 古いエッジを無効にする
			roads.graph[e_desc]->valid = false;

			// 当該頂点を無効にする
			roads.graph[*vi]->valid = false;
		}
	}
}

/**
 * 指定されたdegreeの頂点について、近くに頂点がある場合は、Snapさせる。
 * ただし、Snap対象となるエッジとのなす角度がmin_angle_threshold以下の場合は、対象外。
 */
void GraphUtil::snapDeadendEdges2(RoadGraph& roads, int degree, float threshold) {
	float angle_threshold = 0.34f;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// 指定されたdegree以外の頂点は、対象外
		if (GraphUtil::getDegree(roads, *vi) != degree) continue;

		// 当該頂点と接続されている唯一の頂点を取得
		RoadVertexDesc tgt;
		RoadEdgeDesc e_desc;
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			if (!roads.graph[*ei]->valid) continue;

			tgt = boost::target(*ei, roads.graph);
			e_desc = *ei;
			break;
		}

		// 近接頂点を探す
		RoadVertexDesc nearest_desc;
		float min_dist = std::numeric_limits<float>::max();

		RoadVertexIter vi2, vend2;
		for (boost::tie(vi2, vend2) = boost::vertices(roads.graph); vi2 != vend2; ++vi2) {
			if (!roads.graph[*vi2]->valid) continue;
			if (*vi2 == *vi) continue;
			if (*vi2 == tgt) continue;

			float dist = (roads.graph[*vi2]->pt - roads.graph[*vi]->pt).length();

			// 近接頂点が、*viよりもtgtの方に近い場合は、当該近接頂点は対象からはずす
			//float dist2 = (roads->graph[*vi2]->pt - roads->graph[tgt]->pt).length();
			//if (dist > dist2) continue;

			if (dist < min_dist) {
				nearest_desc = *vi2;
				min_dist = dist;
			}
		}
		
		// 近接頂点が、*viよりもtgtの方に近い場合は、スナップしない
		if ((roads.graph[nearest_desc]->pt - roads.graph[tgt]->pt).length() < (roads.graph[*vi]->pt - roads.graph[tgt]->pt).length()) continue;

		// スナップによるエッジの角度変化が大きすぎる場合は、対象からはずす
		float diff_angle = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[tgt]->pt, roads.graph[nearest_desc]->pt - roads.graph[tgt]->pt);
		if (diff_angle > angle_threshold) continue;

		// tgtとスナップ先との間に既にエッジがある場合は、スナップしない
		if (hasEdge(roads, tgt, nearest_desc)) continue;

		// 当該頂点と近接頂点との距離が、threshold以下なら、スナップする
		if (min_dist <= threshold) {
			snapVertex(roads, *vi, nearest_desc);
		}
	}
}

/**
 * Remove too short dead-end edges unless it has a pair.
 */
void GraphUtil::removeShortDeadend(RoadGraph& roads, float threshold) {
	bool actuallyDeleted = false;

	bool deleted = true;
	while (deleted) {
		deleted = false;

		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
			if (!roads.graph[*vi]->valid) continue;

			if (getDegree(roads, *vi) > 1) continue;

			RoadOutEdgeIter ei, eend;
			for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
				if (!roads.graph[*ei]->valid) continue;

				// If the edge has a pair, don't remove it.
				if (roads.graph[*ei]->properties["fullyPaired"] == true) continue;

				RoadVertexDesc tgt = boost::target(*ei, roads.graph);

				// invalidate the too short edge, and invalidate the dead-end vertex.
				if (roads.graph[*ei]->getLength() < threshold) {
					roads.graph[*vi]->valid = false;
					roads.graph[*ei]->valid = false;
					deleted = true;
					actuallyDeleted = true;
				}
			}
		}
	}

	if (actuallyDeleted) roads.setModified();
}

/**
 * 指定した道路エッジのタイプが、指定されたタイプに含まれるかチェックする。
 * 例えば、タイプとして3を指定した場合、Local streetsとAvenuesが含まれる。
 * 　　　　タイプとして0を指定した場合、全てのタイプのエッジが含まれることを意味する。
 */
bool GraphUtil::isRoadTypeMatched(int type, int ref_type) {
	if (ref_type == 0) return true;

	//if (((int)powf(2, (type - 1)) & ref_type)) return true;
	if (type & ref_type) return true;
	else return false;
}

/**
 * 道路網をcv::Mat行列に置き換える
 */
void GraphUtil::convertToMat(RoadGraph& roads, cv::Mat_<uchar>& mat, const cv::Size& size, int width, bool flip) {
	mat = cv::Mat_<uchar>(size, 0);

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		drawRoadSegmentOnMat(roads, *ei, mat, width);
	}

	// 上下を反転
	if (flip) cv::flip(mat, mat, 0);
}

/**
 * 道路のエッジを、cv::Mat行列上に描画する
 * brightnessは、0から255で指定。（デフォルト値は255）
 */
void GraphUtil::drawRoadSegmentOnMat(RoadGraph& roads, RoadEdgeDesc e, cv::Mat& mat, int width, int brightness) {
	for (int i = 0; i < roads.graph[e]->polyline.size() - 1; i++) {
		QVector2D p0 = roads.graph[e]->polyline[i];
		QVector2D p1 = roads.graph[e]->polyline[i + 1];
		cv::line(mat, cv::Point(p0.x(), p0.y()), cv::Point(p1.x(), p1.y()), cv::Scalar(brightness), width, CV_AA);
	}
}
