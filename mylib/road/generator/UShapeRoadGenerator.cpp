#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "../../common/global.h"
#include "../../common/Util.h"
#include "../../common/TopNSearch.h"
#include "../../common/ConvexHull.h"
#include "../GraphUtil.h"
#include "UShapeRoadGenerator.h"
#include "RoadGeneratorHelper.h"

void UShapeRoadGenerator::generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& feature) {
	srand(12345);

	std::list<RoadVertexDesc> seeds;
	std::list<RoadVertexDesc> additionalSeeds;

	// Avenueのシードを生成
	generateAvenueSeeds(roads, area, hintLine, feature, seeds);

	// Avenueを生成
	{
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			std::cout << "attemptExpansion (avenue): " << i << " (Seed: " << desc << ")" << std::endl;
			if (roads.graph[desc]->properties.contains("example_desc")) {
				attemptExpansion(roads, area, desc, RoadEdge::TYPE_AVENUE, feature, seeds);
			} else {
				attemptExpansion2(roads, area, desc, RoadEdge::TYPE_AVENUE, feature, seeds);
			}
		}
	}

	seeds.clear();
	additionalSeeds.clear();

	GraphUtil::removeSelfIntersectingRoads(roads);
	RoadGeneratorHelper::removeDeadend(roads);

	// Local streetを生成
	if (G::getBool("generateLocalStreets")) {
		generateStreetSeeds(roads, area, feature, seeds);
		
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			std::cout << "attemptExpansion (street): " << i << std::endl;
			if (roads.graph[desc]->properties.contains("example_desc")) {
				attemptExpansion(roads, area, desc, RoadEdge::TYPE_STREET, feature, seeds);
			} else {
				attemptExpansion2(roads, area, desc, RoadEdge::TYPE_STREET, feature, seeds);
			}
		}
	}

	// 指定されたエリアでCropping
	if (G::getBool("cropping")) {
		GraphUtil::extractRoads2(roads, area);
	}

	GraphUtil::removeSelfIntersectingRoads(roads);GraphUtil::removeSelfIntersectingRoads(roads);
	GraphUtil::clean(roads);
}

/**
 * シード頂点を生成する。
 */
void UShapeRoadGenerator::generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& f, std::list<RoadVertexDesc>& seeds) {
	seeds.clear();

	for (int i = 0; i < hintLine.size(); ++i) {
		addAvenueSeed(roads, area, f, hintLine[i], f.hintLine[i], i, seeds);
	}
}

/**
 * Avenue用のシードを座標pt付近に追加する。
 * 座標pt付近の、該当するカーネルを捜し、そのカーネルを使ってシードを追加する。
 *
 * @param area			ターゲット領域
 * @param f				特徴量
 * @param pt			シード座標
 * @param seeds			追加されたシードは、seedsに追加される。
 */
bool UShapeRoadGenerator::addAvenueSeed(RoadGraph &roads, const Polygon2D &area, ExFeature &f, const QVector2D &pt, const QVector2D &ex_pt, int group_id, std::list<RoadVertexDesc>& seeds) {
	if (!area.contains(pt)) return false;

	// Avenueカーネルの中で、offsetの位置に最も近いものを探す
	RoadVertexDesc seedDesc = GraphUtil::getVertex(f.avenues, ex_pt);

	// 頂点を追加し、シードとする
	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(pt));
	RoadVertexDesc desc = GraphUtil::addVertex(roads, v);
	roads.graph[desc]->properties["example_desc"] = seedDesc;
	roads.graph[desc]->properties["initial_seed"] = true;
	roads.graph[desc]->properties["group_id"] = group_id;
	roads.graph[desc]->properties["used"] = true;
	seeds.push_back(desc);

	return true;
}

/**
 * Local Street用のシードを生成する。
 */
void UShapeRoadGenerator::generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, ExFeature &f, std::list<RoadVertexDesc> &seeds) {
	seeds.clear();

	int i = 0;
	int num = GraphUtil::getNumEdges(roads);
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = edges(roads.graph); ei != eend && i < num; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		i++;

		RoadEdgePtr edge = roads.graph[*ei];
		RoadEdgeDesc e = *ei;
		RoadEdgeDesc e1, e2;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (roads.graph[e]->properties["byExample"] == true) {
			// ターゲットエリア座標空間から、Example座標空間へのオフセットを計算
			RoadVertexDesc ex_v_desc = roads.graph[src]->properties["example_desc"].toUInt();
			QVector2D offset = f.roads(RoadEdge::TYPE_AVENUE).graph[ex_v_desc]->pt - roads.graph[src]->pt;
			
			while (edge->polyline.size() > 2) {


				// このエッジ上の各点について、ほぼ同じ位置のStreetカーネルが存在するか探す
				int index = -1;
				bool found = false;
				RoadVertexDesc seedDesc;
				for (int p_id = 1; p_id < edge->polyline.size() - 1; ++p_id) {
					found = false;

					// この点の、Example座標空間での位置を計算する
					BBox bbox;
					QVector2D pt = edge->polyline[p_id] + offset;

					
					if (GraphUtil::getVertex(f.roads(RoadEdge::TYPE_STREET), pt, 1.0f, seedDesc)) {
						found = true;
						index = p_id;
						break;
					}
				}

				// このエッジ上に、ほぼ同じ位置のStreetカーネルが存在しない場合は、終了
				if (!found) break;

				RoadVertexDesc v_desc = GraphUtil::splitEdge(roads, e, edge->polyline[index], e1, e2);

				seeds.push_back(v_desc);

				roads.graph[v_desc]->properties["example_desc"] = seedDesc;

				edge = roads.graph[e2];
				e = e2;
			}
		} else {
			int step = roads.graph[e]->polyline.size() / 5;
			if (step <= 1) continue;
		
			while (step < edge->polyline.size() - step) {
				RoadVertexDesc desc = GraphUtil::splitEdge(roads, e, edge->polyline[step], e1, e2);
			
				seeds.push_back(desc);

				edge = roads.graph[e2];
				e = e2;
			}
		}
	}
}

/**
 * このシードを使って、道路生成する。
 * Exampleベースで生成する。
 */
void UShapeRoadGenerator::attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, std::list<RoadVertexDesc> &seeds) {
	RoadVertexDesc ex_v_desc = roads.graph[srcDesc]->properties["example_desc"].toUInt();
	RoadVertexPtr ex_vertex = f.roads(roadType).graph[ex_v_desc];

	float roadSnapFactor = G::getFloat("roadSnapFactor");
	float roadAngleTolerance = G::getFloat("roadAngleTolerance");

	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(ex_v_desc, f.roads(roadType).graph); ei != eend; ++ei) {
		RoadVertexDesc tgt = boost::target(*ei, f.roads(roadType).graph);

		Polyline2D polyline = GraphUtil::orderPolyLine(f.roads(roadType), *ei, ex_v_desc);

		QVector2D offset = polyline[0];
		polyline.translate(offset * -1.0f);

		growRoadSegment(roads, area, srcDesc, roadType, f, polyline, f.roads(roadType).graph[*ei]->lanes, tgt, true, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * このシードを使って、PM方式で道路生成する。
 */
void UShapeRoadGenerator::attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, std::list<RoadVertexDesc> &seeds) {
	float snapThreshold;

	if (roadType == RoadEdge::TYPE_AVENUE) {
		snapThreshold = f.avgAvenueLength * 0.2f;
	} else {
		snapThreshold = f.avgStreetLength * 0.2f;
	}

	// 当該シードに、roadTypeよりも上位レベルの道路セグメントが接続されているか、チェックする
	bool isConnectedByUpperLevelRoadSegment = false;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(srcDesc, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		if (roads.graph[*ei]->type > roadType) {
			isConnectedByUpperLevelRoadSegment = true;
			break;
		}
	}

	if (!isConnectedByUpperLevelRoadSegment) {
		// 当該頂点の近くに他の頂点があれば、スナップさせる
		RoadVertexDesc tgtDesc;
		//if (RoadGeneratorHelper::canSnapToVertex3(roads, srcDesc, snapThreshold, tgtDesc)) {
		if (GraphUtil::getVertex(roads, roads.graph[srcDesc]->pt, snapThreshold, srcDesc, tgtDesc)) {
			GraphUtil::snapVertex(roads, srcDesc, tgtDesc);
			return;
		}

		RoadEdgeDesc closeEdge;
		QVector2D closestPt;
		if (GraphUtil::getEdge(roads, srcDesc, snapThreshold, closeEdge, closestPt)) {
			tgtDesc = GraphUtil::splitEdge(roads, closeEdge, closestPt);
			GraphUtil::snapVertex(roads, srcDesc, tgtDesc);
			return;
		}
	}

	// 道路生成用のカーネルを合成する
	std::vector<RoadEdgePtr> edges;
	synthesizeItem(roads, f, srcDesc, roadType, edges);
	
	float roadSnapFactor = G::getFloat("roadSnapFactor");
	float roadAngleTolerance = G::getFloat("roadAngleTolerance");

	for (int i = 0; i < edges.size(); ++i) {
		growRoadSegment(roads, area, srcDesc, roadType, f, edges[i]->polyline, 1, 0, false, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * 指定されたpolylineに従って、srcDesc頂点からエッジを伸ばす。
 * エッジの端点が、srcDescとは違うセルに入る場合は、falseを返却する。
 */
bool UShapeRoadGenerator::growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds) {
	// 新しいエッジを生成
	RoadEdgePtr new_edge = RoadEdgePtr(new RoadEdge(roadType, lanes));
	for (int i = 0; i < polyline.size(); ++i) {
		QVector2D pt = roads.graph[srcDesc]->pt + polyline[i];
		new_edge->polyline.push_back(pt);
	}

	if (GraphUtil::hasRedundantEdge(roads, srcDesc, new_edge->polyline, angleTolerance)) {
		return false;
	}

	// 他のエッジとの交差をチェック
	if (!byExample) {
		if (GraphUtil::isIntersect(roads, new_edge->polyline)) {
			if (roadType == RoadEdge::TYPE_AVENUE) return false;

			// Local Streetsの場合は、エッジにスナップさせたい
			RoadEdgeDesc eiClosest;
			QVector2D closestIntPt;
			RoadGeneratorHelper::intersects(roads, srcDesc, new_edge->polyline, eiClosest, closestIntPt);
			GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, closestIntPt);
		}
	}

	RoadVertexDesc tgtDesc;

	if (byExample) {
		snapFactor = 0.1f;
		angleTolerance = 0.1f;
	}

	// スナップできるか？
	RoadEdgeDesc closestEdge;
	QVector2D intPoint;
	if (GraphUtil::getVertex(roads, new_edge->polyline.last(), new_edge->polyline.length() * snapFactor, srcDesc, tgtDesc)) {
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		std::reverse(new_edge->polyline.begin(), new_edge->polyline.end());
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else if (RoadGeneratorHelper::canSnapToEdge(roads, new_edge->polyline.last(), new_edge->polyline.length() * snapFactor, srcDesc, closestEdge, intPoint)) {
		tgtDesc = GraphUtil::splitEdge(roads, closestEdge, intPoint);
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else {
		// 頂点を追加
		RoadVertexPtr v = RoadVertexPtr(new RoadVertex(new_edge->polyline.last()));
		tgtDesc = GraphUtil::addVertex(roads, v);
		roads.graph[tgtDesc]->properties["parent"] = srcDesc;


		if (area.contains(new_edge->polyline.last())) {
			// シードに追加する
			if (byExample) {
				if (roadType == RoadEdge::TYPE_AVENUE || GraphUtil::getDegree(f.roads(roadType), next_ex_v_desc) > 1) {
					seeds.push_back(tgtDesc);
				}

				// 対応するExampleが存在する場合は、それを設定する
				if (!f.roads(roadType).graph[next_ex_v_desc]->properties.contains("used") && GraphUtil::getDegree(f.roads(roadType), next_ex_v_desc) > 1) {
					roads.graph[tgtDesc]->properties["example_desc"] = next_ex_v_desc;
					f.roads(roadType).graph[next_ex_v_desc]->properties["used"] = true;
				}
			} else {
				seeds.push_back(tgtDesc);
			}
		} else {
			roads.graph[tgtDesc]->onBoundary = true;
		}


	}

	RoadEdgeDesc e_desc = GraphUtil::addEdge(roads, srcDesc, tgtDesc, new_edge);
	roads.graph[e_desc]->properties["byExample"] = byExample;

	return true;
}

/**
 * PMに従って、カーネルを合成する
 */
void UShapeRoadGenerator::synthesizeItem(RoadGraph &roads, ExFeature &f, RoadVertexDesc v_desc, int roadType, std::vector<RoadEdgePtr> &edges) {
	// 当該頂点から出るエッジをリストアップする
	std::vector<Polyline2D> polylines;
	QList<RoadVertexDesc> neighbors;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(v_desc, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		neighbors.push_back(tgt);

		if ((roads.graph[v_desc]->pt - roads.graph[*ei]->polyline[0]).lengthSquared() > (roads.graph[tgt]->pt - roads.graph[*ei]->polyline[0]).lengthSquared()) {
			std::reverse(roads.graph[*ei]->polyline.begin(), roads.graph[*ei]->polyline.end());
		}
		polylines.push_back(roads.graph[*ei]->polyline);
	}

	float direction = 0.0f;

	if (polylines.size() > 0) {
		QVector2D vec = polylines[0][1] - polylines[0][0];
		direction = atan2f(vec.y(), vec.x());
	}

	// エッジ長を決定する
	float length = f.length(roadType);
	float curvature = f.curvature(roadType);

	RoadGeneratorHelper::createFourEdges(roadType, 1, direction, 10.0f, length, G::getFloat("roadOrganicFactor"), edges);
}

/**
 * 境界上の頂点を延長し、近くのエッジにぶつける
 * ただし、現在の頂点のConvexHullの外に出たエッジは、そこで延長をストップする。
 */
void UShapeRoadGenerator::connectRoads(RoadGraph &roads, float dist_threshold, float angle_threshold) {
	// ConvexHullを計算する
	ConvexHull convexHull;
	Polygon2D hull;

	// 境界上の頂点、エッジの組をリストアップする
	QList<RoadVertexDesc> boundaryNodes;
	QMap<RoadVertexDesc, RoadEdgeDesc> boundaryEdges;
	QMap<RoadVertexDesc, RoadVertexDesc> boundaryNodesPair;
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);
		
		convexHull.addPoint(roads.graph[src]->pt);
		convexHull.addPoint(roads.graph[tgt]->pt);

		if (roads.graph[src]->onBoundary && GraphUtil::getDegree(roads, src) == 1) {
			if (!boundaryNodes.contains(src)) boundaryNodes.push_back(src);
			if (!boundaryEdges.contains(src)) boundaryEdges[src] = *ei;
			if (!boundaryNodesPair.contains(src)) boundaryNodesPair[src] = tgt;
		} else if (roads.graph[tgt]->onBoundary && GraphUtil::getDegree(roads, tgt) == 1) {
			if (!boundaryNodes.contains(tgt)) boundaryNodes.push_back(tgt);
			if (!boundaryEdges.contains(tgt)) boundaryEdges[tgt] = *ei;
			if (!boundaryNodesPair.contains(tgt)) boundaryNodesPair[tgt] = src;
		}
	}

	convexHull.convexHull(hull);

	// リストアップしたエッジを、それぞれ少しずつ伸ばしていき、他のエッジにぶつかったらストップする
	int numIterations = 50000;
	while (!boundaryNodes.empty() && numIterations >= 0) {
		RoadVertexDesc v_desc = boundaryNodes.front();
		boundaryNodes.pop_front();

		if (!roads.graph[v_desc]->valid) continue;

		RoadVertexDesc v2_desc = boundaryNodesPair[v_desc];
		RoadEdgeDesc e_desc = boundaryEdges[v_desc];

		QVector2D step;
		if ((roads.graph[v_desc]->pt - roads.graph[e_desc]->polyline[0]).lengthSquared() <= (roads.graph[v2_desc]->pt - roads.graph[e_desc]->polyline[0]).lengthSquared()) {
			step = roads.graph[e_desc]->polyline[0] - roads.graph[e_desc]->polyline[1];
		} else {
			step = roads.graph[e_desc]->polyline.last() - roads.graph[e_desc]->polyline[roads.graph[e_desc]->polyline.size() - 2];
		}
		step = step.normalized() * 20.0f;

		if (growRoadOneStep(roads, v_desc, step)) {
			if (hull.contains(roads.graph[v_desc]->pt)) {
				boundaryNodes.push_back(v_desc);
			}
		}

		numIterations--;
	}

	//GraphUtil::clean(roads);
}

bool UShapeRoadGenerator::growRoadOneStep(RoadGraph& roads, RoadVertexDesc srcDesc, const QVector2D& step) {
	bool snapped = false;
	bool intersected = false;

	QVector2D pt = roads.graph[srcDesc]->pt + step;
	RoadEdgeDesc closestEdge;

	// INTERSECTS -- If edge intersects other edge
	QVector2D intPoint;
	intersected = RoadGeneratorHelper::intersects(roads, roads.graph[srcDesc]->pt, pt, closestEdge, intPoint);
	if (intersected) {
		pt = intPoint;
	}

	if (intersected) {
		RoadVertexDesc splitVertex = GraphUtil::splitEdge(roads, closestEdge, pt);
		GraphUtil::snapVertex(roads, srcDesc, splitVertex);

		// 交差相手のエッジが、成長中のエッジなら、その成長をストップする
		RoadVertexDesc src = boost::source(closestEdge, roads.graph);
		RoadVertexDesc tgt = boost::target(closestEdge, roads.graph);
		if (roads.graph[src]->onBoundary) {
			RoadEdgeDesc e = GraphUtil::getEdge(roads, src, splitVertex);
			roads.graph[e]->valid = false;
			roads.graph[src]->valid = false;
		} else if (roads.graph[tgt]->onBoundary) {
			RoadEdgeDesc e = GraphUtil::getEdge(roads, tgt, splitVertex);
			roads.graph[e]->valid = false;
			roads.graph[tgt]->valid = false;
		}

		return false;
	} else {
		GraphUtil::moveVertex(roads, srcDesc, pt);
		return true;
	}	
}

/**
 * 境界上の頂点を、できるだけ近くの他の頂点とつなぐ
 */
void UShapeRoadGenerator::connectRoads2(RoadAreaSet &areas, float dist_threshold, float angle_threshold) {
	for (int i = 0; i < areas.size(); ++i) {
		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = vertices(areas.areas[i]->roads.graph); vi != vend; ++vi) {
			if (!areas.areas[i]->roads.graph[*vi]->valid) continue;

			if (!areas.areas[i]->roads.graph[*vi]->onBoundary) continue;

			connectRoads2(areas, i, *vi, dist_threshold, angle_threshold);
		}
	}

	// 繋がらなかった頂点については、その頂点と、そこから出るエッジを無効にする
	/*
	for (boost::tie(vi, vend) = vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (!roads.graph[*vi]->onBoundary) continue;

		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = out_edges(*vi, roads.graph); ei != eend; ++ei) {
			roads.graph[*ei]->valid = false;
		}

		roads.graph[*vi]->valid = false;
	}
	*/

	//GraphUtil::clean(roads);
}

void UShapeRoadGenerator::connectRoads2(RoadAreaSet &areas, int area_id, RoadVertexDesc v_desc, float dist_threshold, float angle_threshold) {
	float dist_threshold2 = dist_threshold * dist_threshold;

	float min_dist = std::numeric_limits<float>::max();
	int min_area_id;
	RoadVertexDesc min_desc;

	// v_descの、もう一端の頂点を取得
	RoadVertexDesc v2_desc;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = out_edges(v_desc, areas.areas[area_id]->roads.graph); ei != eend; ++ei) {
		if (!areas.areas[area_id]->roads.graph[*ei]->valid) continue;

		v2_desc = boost::target(*ei, areas.areas[area_id]->roads.graph);
	}

	for (int i = 0; i < areas.size(); ++i) {
		if (i == area_id) continue;

		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = vertices(areas.areas[i]->roads.graph); vi != vend; ++vi) {
			if (!areas.areas[i]->roads.graph[*vi]->valid) continue;

			//if (!roads.graph[*vi]->onBoundary) continue;

			/*
			for (boost::tie(ei, eend) = out_edges(*vi, roads.graph); ei != eend; ++ei) {
				if (!roads.graph[*ei]->valid) continue;

				RoadVertexDesc tgt = boost::target(*ei, roads.graph);
				float a = Util::diffAngle(roads.graph[*vi]->pt - roads.graph[tgt]->pt, roads.graph
			}
			*/

			float dist = (areas.areas[i]->roads.graph[*vi]->pt - areas.areas[area_id]->roads.graph[v_desc]->pt).lengthSquared();
			if (dist < min_dist) {
				min_dist = dist;
				min_area_id = i;
				min_desc = *vi;
			}
		}
	}

	//if (min_dist > dist_threshold2) return;

	// スナップにより、角度が大幅に変わる場合は、スナップさせない
	float angle = Util::diffAngle(areas.areas[area_id]->roads.graph[v_desc]->pt - areas.areas[area_id]->roads.graph[v2_desc]->pt, areas.areas[min_area_id]->roads.graph[min_desc]->pt - areas.areas[area_id]->roads.graph[v2_desc]->pt);
	if (angle > angle_threshold) return;

	GraphUtil::moveVertex(areas.areas[area_id]->roads, v_desc, areas.areas[min_area_id]->roads.graph[min_desc]->pt);
	areas.areas[area_id]->roads.graph[v_desc]->onBoundary = false;
	areas.areas[min_area_id]->roads.graph[min_desc]->onBoundary = false;
}
