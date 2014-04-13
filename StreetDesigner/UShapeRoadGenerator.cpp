#include <QTest>
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <common/global.h>
#include <common/Util.h>
#include <common/ConvexHull.h>
#include <road/GraphUtil.h>
#include "UShapeRoadGenerator.h"
#include "RoadGeneratorHelper.h"

void UShapeRoadGenerator::generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, mylib::Terrain* terrain, ExFeature& feature) {
	srand(12345);

	std::list<RoadVertexDesc> seeds;
	std::list<RoadVertexDesc> additionalSeeds;

	// Avenueのシードを生成
	generateAvenueSeeds(roads, area, hintLine, feature, seeds);

	// Avenueを生成
	{
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numAvenueIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			float z = terrain->getValue(roads.graph[desc]->pt.x(), roads.graph[desc]->pt.y());
			if (z < 0.0f || z > 100.0f) {
				std::cout << "attemptExpansion (avenue): " << i << " (skipped because it is under the sea or on the mountains)" << std::endl;
				continue;
			}

			std::cout << "attemptExpansion (avenue): " << i << " (Seed: " << desc << ")" << std::endl;
			if (roads.graph[desc]->properties.contains("example_desc")) {
				attemptExpansion(roads, area, desc, RoadEdge::TYPE_AVENUE, terrain, feature, seeds);
			} else {
				attemptExpansion2(roads, area, desc, RoadEdge::TYPE_AVENUE, terrain, feature, seeds);
			}

			if (animation) {
				mainWin->glWidget->updateGL();
				QTest::qWait(10);
			}
		}
	}

	seeds.clear();
	additionalSeeds.clear();

	if (G::getBool("cleanAvenues")) {
		GraphUtil::removeSelfIntersectingRoads(roads);
		RoadGeneratorHelper::extendDanglingEdges(roads);
		RoadGeneratorHelper::removeDeadend(roads);
		//GraphUtil::reduce(roads);
		//GraphUtil::removeLoop(roads);

		if (animation) {
			mainWin->glWidget->updateGL();
			QTest::qWait(10);
		}
	}

	// Local streetを生成
	if (G::getBool("generateLocalStreets")) {
		generateStreetSeeds(roads, area, feature, seeds);
		
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numStreetIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			float z = terrain->getValue(roads.graph[desc]->pt.x(), roads.graph[desc]->pt.y());
			if (z < 0.0f || z > 100.0f) {
				std::cout << "attemptExpansion (street): " << i << " (skipped because it is under the sea or on the mountains)" << std::endl;
				continue;
			}

			std::cout << "attemptExpansion (street): " << i << " (Seed: " << desc << ")" << std::endl;
			if (roads.graph[desc]->properties.contains("example_desc")) {
				attemptExpansion(roads, area, desc, RoadEdge::TYPE_STREET, terrain, feature, seeds);
			} else {
				attemptExpansion2(roads, area, desc, RoadEdge::TYPE_STREET, terrain, feature, seeds);
			}

			if (animation) {
				mainWin->glWidget->updateGL();
				QTest::qWait(10);
			}
		}
	}

	// 指定されたエリアでCropping
	if (G::getBool("cropping")) {
		GraphUtil::extractRoads2(roads, area);
	}

	if (G::getBool("cleanStreets")) {
		GraphUtil::removeSelfIntersectingRoads(roads);
		RoadGeneratorHelper::removeDeadend(roads);
	}

	GraphUtil::clean(roads);
	GraphUtil::normalizeLoop(roads);
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
	roads.graph[desc]->properties["group_id"] = group_id;
	roads.graph[desc]->properties["used"] = true;
	roads.graph[desc]->properties["generation_type"] = "example";
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
		
		int group_id;
		if (roads.graph[src]->properties.contains("group_id")) {
			group_id = roads.graph[src]->properties["group_id"].toInt();
		} else if (roads.graph[tgt]->properties.contains("group_id")) {
			group_id = roads.graph[tgt]->properties["group_id"].toInt();
		} else {
			group_id = -1;
		}

		if (roads.graph[e]->properties["byExample"] == true) {
			// ターゲットエリア座標空間から、Example座標空間へのオフセットを計算
			QVector2D offset;
			if (roads.graph[src]->properties.contains("example_desc")) {
				RoadVertexDesc ex_v_desc = roads.graph[src]->properties["example_desc"].toUInt();
				offset = f.roads(RoadEdge::TYPE_AVENUE).graph[ex_v_desc]->pt - roads.graph[src]->pt;
			} else {
				RoadVertexDesc ex_v_desc = roads.graph[tgt]->properties["example_desc"].toUInt();
				offset = f.roads(RoadEdge::TYPE_AVENUE).graph[ex_v_desc]->pt - roads.graph[tgt]->pt;
			}
			
			while (edge->polyline.size() > 2) {
				// このエッジ上の各点について、ほぼ同じ位置のStreetカーネルが存在するか探す
				int index = -1;
				bool found = false;
				RoadVertexDesc seedDesc;
				for (int p_id = 1; p_id < edge->polyline.size() - 1; ++p_id) {
					found = false;

					// この点が、エリア外なら、スキップ
					if (!area.contains(edge->polyline[p_id])) continue;

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

				// シードとして追加
				seeds.push_back(v_desc);
				roads.graph[v_desc]->properties["group_id"] = group_id;
				roads.graph[v_desc]->properties["example_desc"] = seedDesc;
				roads.graph[v_desc]->properties["generation_type"] = "example";

				// エッジを更新
				edge = roads.graph[e2];
				e = e2;
			}
		} else {
			int step;
			if (roads.graph[e]->polyline.length() > 100.0f) {
				step = roads.graph[e]->polyline.size() / 5;
			} else {
				step = roads.graph[e]->polyline.size() / 2;
			}
			if (step <= 1) continue;
		
			while (step < edge->polyline.size() - step) {
				RoadVertexDesc desc = GraphUtil::splitEdge(roads, e, edge->polyline[step], e1, e2);
				roads.graph[desc]->properties["generation_type"] = "pm";

				// この点が、エリア内なら、シードとして追加
				if (area.contains(edge->polyline[step])) {
					seeds.push_back(desc);
				}

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
void UShapeRoadGenerator::attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds) {
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

		growRoadSegment(roads, area, srcDesc, roadType, terrain, f, polyline, f.roads(roadType).graph[*ei]->lanes, tgt, true, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * このシードを使って、PM方式で道路生成する。
 */
void UShapeRoadGenerator::attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds) {
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
		growRoadSegment(roads, area, srcDesc, roadType, terrain, f, edges[i]->polyline, 1, 0, false, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * 指定されたpolylineに従って、srcDesc頂点からエッジを伸ばす。
 * エッジの端点が、srcDescとは違うセルに入る場合は、falseを返却する。
 */
bool UShapeRoadGenerator::growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds) {
	bool intercepted = false;

	// 新しいエッジを生成
	RoadEdgePtr new_edge = RoadEdgePtr(new RoadEdge(roadType, lanes));
	for (int i = 0; i < polyline.size(); ++i) {
		QVector2D pt = roads.graph[srcDesc]->pt + polyline[i];

		new_edge->polyline.push_back(pt);

		// 水没、または、山の上なら、道路生成をストップ
		float z = terrain->getValue(pt.x(), pt.y());
		if (z < 0.0f || z > 100.0f) {
			// 最初っから水没している場合は、そもそもエッジ生成をキャンセル
			if (new_edge->polyline.size() <= 1) return false;

			RoadGeneratorHelper::cutEdgeBySteepElevationChange(new_edge->polyline, terrain);

			intercepted = true;
			break;
		}

		// 他のエッジと交差したら、道路生成をストップ
		QVector2D intPoint;
		//if (roadType == RoadEdge::TYPE_STREET && GraphUtil::isIntersect(roads, new_edge->polyline, intPoint)) {
		if (GraphUtil::isIntersect(roads, new_edge->polyline, intPoint)) {
			new_edge->polyline.erase(new_edge->polyline.begin() + new_edge->polyline.size() - 1);
			new_edge->polyline.push_back(intPoint);

			intercepted = true;

			// エッジ長が最短thresholdより短い場合は、キャンセル
			if (new_edge->polyline.length() < 30.0f) return false;

			break;
		}
	}

	if (new_edge->polyline.size() == 1) return false;

	if (GraphUtil::hasRedundantEdge(roads, srcDesc, new_edge->polyline, angleTolerance)) {
		return false;
	}

	RoadVertexDesc tgtDesc;

	if (byExample) {
		snapFactor = 0.01f;
	}

	// スナップできるか？
	RoadEdgeDesc closestEdge;
	QVector2D intPoint;
	if ((new_edge->polyline.last() - roads.graph[srcDesc]->pt).lengthSquared() < 0.1f) {
		// ループエッジ
		tgtDesc = srcDesc;
	} else if (GraphUtil::getVertex(roads, new_edge->polyline.last(), new_edge->polyline.length() * snapFactor, srcDesc, tgtDesc)) {
		if (byExample && roads.graph[tgtDesc]->properties["generation_type"] == "example") {
			angleTolerance = 0.01f;
		}

		// 他の頂点にスナップ
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		std::reverse(new_edge->polyline.begin(), new_edge->polyline.end());
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else if (GraphUtil::getEdge(roads, new_edge->polyline.last(), new_edge->polyline.length() * snapFactor, srcDesc, closestEdge, intPoint)) {
		if (byExample && roads.graph[closestEdge]->properties["generation_type"] == "example") {
			angleTolerance = 0.01f;
		}

		// 他のエッジにスナップ
		tgtDesc = GraphUtil::splitEdge(roads, closestEdge, intPoint);

		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else {
		// 頂点を追加
		RoadVertexPtr v = RoadVertexPtr(new RoadVertex(new_edge->polyline.last()));
		tgtDesc = GraphUtil::addVertex(roads, v);
		roads.graph[tgtDesc]->properties["parent"] = srcDesc;

		// 新しい頂点にgroup_idを引き継ぐ
		roads.graph[tgtDesc]->properties["group_id"] = roads.graph[srcDesc]->properties["group_id"];

		// 新しい頂点のgeneration_typeを設定
		roads.graph[tgtDesc]->properties["generation_type"] = byExample ? "example" : "pm";

		if (area.contains(new_edge->polyline.last())) {
			roads.graph[tgtDesc]->properties["generation_type"] = "pm";

			// シードに追加する
			if (byExample && !intercepted) {	// Exampleベースで、且つ、途中で生成ストップしていない場合（つまり、Exampleそのまま）
				if (roadType == RoadEdge::TYPE_AVENUE || GraphUtil::getDegree(f.roads(roadType), next_ex_v_desc) > 1) {
					seeds.push_back(tgtDesc);
				}

				// 対応するExampleが存在する場合は、それを設定する
				if (!f.roads(roadType).graph[next_ex_v_desc]->properties.contains("used") && GraphUtil::getDegree(f.roads(roadType), next_ex_v_desc) > 1) {
					f.roads(roadType).graph[next_ex_v_desc]->properties["used"] = true;
					roads.graph[tgtDesc]->properties["generation_type"] = "example";
					roads.graph[tgtDesc]->properties["example_desc"] = next_ex_v_desc;
				}
			} else {
				seeds.push_back(tgtDesc);
			}
		} else {
			// ターゲットエリアの外に出たら
			if (byExample) {
				roads.graph[tgtDesc]->properties["generation_type"] = "example";
				roads.graph[tgtDesc]->properties["example_desc"] = next_ex_v_desc;
			} else {
				roads.graph[tgtDesc]->properties["generation_type"] = "pm";
			}

			roads.graph[tgtDesc]->onBoundary = true;
		}

		// Example道路のDeadendは、Deadendとして正式に登録する
		if (byExample && GraphUtil::getDegree(f.roads(roadType), next_ex_v_desc) == 1) {
			roads.graph[tgtDesc]->properties["deadend"] = true;
		}
	}

	RoadEdgeDesc e_desc = GraphUtil::addEdge(roads, srcDesc, tgtDesc, new_edge);
	roads.graph[e_desc]->properties["byExample"] = byExample;

	// 新しいエッジにgroup_idを引き継ぐ
	roads.graph[e_desc]->properties["group_id"] = roads.graph[srcDesc]->properties["group_id"];

	// 新しいエッジのgeneration_typeを設定
	roads.graph[e_desc]->properties["generation_type"] = byExample ? "example" : "pm";

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

	// 直近の頂点で、example_descを持つ頂点を探し、そのexample空間座標からの相対座標を使って、example空間座標を計算する
	RoadVertexDesc nearest_v_desc = RoadGeneratorHelper::getClosestVertexByExample(roads, v_desc);
	RoadVertexDesc ex_desc = roads.graph[nearest_v_desc]->properties["example_desc"].toUInt();
	QVector2D pt = f.roads(roadType).graph[ex_desc]->pt + roads.graph[v_desc]->pt - roads.graph[nearest_v_desc]->pt;

	RoadGeneratorHelper::createFourEdges(f, roadType, pt, 1, direction, 10.0f, edges);

	roads.graph[v_desc]->properties["generation_type"] = "pm";
}
