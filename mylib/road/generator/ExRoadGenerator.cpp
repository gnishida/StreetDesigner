#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include "../../common/global.h"
#include "../../common/Util.h"
#include "../../common/TopNSearch.h"
#include "../../common/ConvexHull.h"
#include "../GraphUtil.h"
#include "ExRoadGenerator.h"
#include "RoadGeneratorHelper.h"

void ExRoadGenerator::generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf) {
	srand(12345);

	// 境界上に、Avenueを生成
	if (G::getBool("addAvenuesOnBoundary")) {
		generateRoadsOnBoundary(roads, area, RoadEdge::TYPE_AVENUE, 1);
	}

	std::list<RoadVertexDesc> seeds;
	std::list<RoadVertexDesc> additionalSeeds;

	// Avenueのシードを生成
	generateAvenueSeeds(roads, area, kf, seeds);

	// Avenueを生成
	{
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			std::cout << "attemptExpansion (avenue): " << i << " (Seed: " << desc << ")" << std::endl;
			attemptExpansion(roads, area, desc, RoadEdge::TYPE_AVENUE, kf, seeds, additionalSeeds);
		}
		for (; !additionalSeeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = additionalSeeds.front();
			additionalSeeds.pop_front();

			std::cout << "attemptExpansion (avenue): " << i << " (Additional Seed: " << desc << ")" << std::endl;
			attemptExpansion2(roads, area, desc, RoadEdge::TYPE_AVENUE, kf, additionalSeeds);
		}
	}

	seeds.clear();
	additionalSeeds.clear();

	GraphUtil::removeSelfIntersectingRoads(roads);

	// Local streetを生成
	if (G::getBool("generateLocalStreets")) {
		generateStreetSeeds2(roads, area, kf, seeds);
		
		int i;
		for (i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			std::cout << "attemptExpansion (street): " << i << std::endl;
			attemptExpansion(roads, area, desc, RoadEdge::TYPE_STREET, kf, seeds, additionalSeeds);
		}
		for (; !additionalSeeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = additionalSeeds.front();
			additionalSeeds.pop_front();

			std::cout << "attemptExpansion (street): " << i << " (Additional Seed: " << desc << ")" << std::endl;
			attemptExpansion2(roads, area, desc, RoadEdge::TYPE_STREET, kf, additionalSeeds);
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
 * Areaの境界上に、道路を生成する。
 */
void ExRoadGenerator::generateRoadsOnBoundary(RoadGraph &roads, const Polygon2D &area, int roadType, int lanes) {
	RoadVertexDesc prevDesc;

	for (int i = 0; i < area.size(); ++i) {
		RoadVertexDesc desc;
		if (!GraphUtil::getVertex(roads, area[i], 0.1f, desc)) {
			RoadVertexPtr v = RoadVertexPtr(new RoadVertex(area[i]));
			desc = GraphUtil::addVertex(roads, v);
		}

		if (i > 0) {
			GraphUtil::addEdge(roads, prevDesc, desc, roadType, lanes);
		}

		prevDesc = desc;
	}
}

/**
 * シード頂点を生成する。
 * 密度に応じて、エリア内にランダムにシードを生成する。
 */
void ExRoadGenerator::generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds) {
	seeds.clear();
	
	float numExpectedVertices = f.density(RoadEdge::TYPE_AVENUE) * area.area();
	int numSeeds = numExpectedVertices / 30 + 1;
	float threshold = area.area() / numExpectedVertices;
	
	if (G::getBool("multiSeeds")) {
		// シードの数を決定
		BBox bboxTarget = area.envelope();
		QVector2D centerTarget = bboxTarget.midPt();

		BBox bboxExample = f.area().envelope();

		int dx = bboxTarget.dx() / 2 - bboxExample.dx() / 2;
		if (dx > 0) {
			dx = (int)(dx / bboxExample.dx()) * 2 + 3;
		} else {
			dx = 1;
		}

		int dy = bboxTarget.dy() / 2 - bboxExample.dy() / 2;
		if (dy > 0) {
			dy = (int)(dy / bboxExample.dy()) * 2 + 3;
		} else {
			dy = 1;
		}

		if (dx % 2 == 0 || dy % 2 == 0) {
			std::cerr << "ERROR: BUG in the number of seeds." << std::endl;
		}

		for (int i = 0; i < dy; ++i) {
			float y = bboxTarget.midPt().y() + bboxExample.dy() * (i - (dy - 1) / 2);
			for (int j = 0; j < dx; ++j) {
				float x = bboxTarget.midPt().x() + bboxExample.dx() * (j - (dx - 1) / 2);
				std::cout << "Put a seed at (" << x << ", " << y << ")" << std::endl;
				if (!addAvenueSeed(roads, area, f, QVector2D(x, y), seeds)) {
					// エリア外の場合は、位置を少しずらして再度トライ
					/*if (x > centerTarget.x()) {
						if (y > centerTarget.y()) {
							addAvenueSeed(roads, area, f, QVector2D(x - bboxExample.dx() * 0.5f, y - bboxExample.dy() * 0.5f), seeds);
						} else {
							addAvenueSeed(roads, area, f, QVector2D(x - bboxExample.dx() * 0.5f, y + bboxExample.dy() * 0.5f), seeds);
						}
					} else {
						if (y > centerTarget.y()) {
							addAvenueSeed(roads, area, f, QVector2D(x + bboxExample.dx() * 0.5f, y - bboxExample.dy() * 0.5f), seeds);
						} else {
							addAvenueSeed(roads, area, f, QVector2D(x + bboxExample.dx() * 0.5f, y + bboxExample.dy() * 0.5f), seeds);
						}
					}*/
				}
			}
		}
	} else {
		addAvenueSeed(roads, area, f, area.envelope().midPt(), seeds);
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
bool ExRoadGenerator::addAvenueSeed(RoadGraph &roads, const Polygon2D &area, const KDEFeature &f, QVector2D &pt, std::list<RoadVertexDesc>& seeds) {
	if (!area.contains(pt)) return false;

	BBox bboxTarget = area.envelope();

	BBox bbox;
	QVector2D offset = RoadGeneratorHelper::modulo(area, f.area(), pt, bbox);

	// Avenueカーネルの中で、offsetの位置に最も近いものを探す
	int min_index = RoadGeneratorHelper::getClosestItem(f, RoadEdge::TYPE_AVENUE, offset);

	// カーネルの座標を、もとの座標に戻す
	QVector2D seed_pt = pt + f.items(RoadEdge::TYPE_AVENUE)[min_index].pt - offset;
	//if (!area.contains(seed_pt)) return false;

	// 頂点を追加し、シードとする
	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(seed_pt));
	RoadVertexDesc desc = GraphUtil::addVertex(roads, v);
	seeds.push_back(desc);

	return true;
}

/**
 * Local street用のシードを生成する。
 * Avenueが既に生成済みであることを前提とする。streetSeedフラグがtrueのAvenue頂点をシードとする。
 */
void ExRoadGenerator::generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds) {
	std::vector<RoadVertexDesc> tempSeeds;

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (roads.graph[*vi]->kernel.streetSeed) {
			tempSeeds.push_back(*vi);
		} else if (GraphUtil::getDegree(roads, *vi) == 2) {
			tempSeeds.push_back(*vi);
		}
	}

	// 各シードに、カーネルを割り当てる
	for (int i = 0; i < tempSeeds.size(); ++i) {
		if (getItem(roads, area, f, RoadEdge::TYPE_STREET, tempSeeds[i], roads.graph[tempSeeds[i]]->kernel)) {
			// カーネル割り当てに成功したら、シードに追加する
			seeds.push_back(tempSeeds[i]);
		}
	}
}

void ExRoadGenerator::generateStreetSeeds2(RoadGraph &roads, const Polygon2D &area, const KDEFeature &kf, std::list<RoadVertexDesc> &seeds) {
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
			while (edge->polyline.size() > 2) {


				// このエッジ上の各点について、ほぼ同じ位置のStreetカーネルが存在するか探す
				int index = -1;
				for (int p_id = 1; p_id < edge->polyline.size() - 1; ++p_id) {
					// この点の、Example座標空間での位置を計算する
					BBox bbox;
					QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), edge->polyline[p_id], bbox);

					// この点と、ほぼ同じ位置のStreetカーネルが存在するなら、シードとして使用する
					for (int j = 0; j < kf.items(RoadEdge::TYPE_STREET).size(); ++j) {
						if ((pt - kf.items(RoadEdge::TYPE_STREET)[j].pt).lengthSquared() < 1.0f) {
							index = p_id;
							break;
						}
					}

					if (index >= 0) break;
				}

				// このエッジ上に、ほぼ同じ位置のStreetカーネルが存在しない場合は、終了
				if (index == -1) break;

				RoadVertexDesc v_desc = GraphUtil::splitEdge(roads, e, edge->polyline[index], e1, e2);

				seeds.push_back(v_desc);

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
 */
void ExRoadGenerator::attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds) {
	// カーネル割り当てに失敗したら、キャンセルして、additionalSeedsに登録する
	KDEFeatureItem item;
	if (!getItem(roads, area, f, roadType, srcDesc, item)) {
		additionalSeeds.push_back(srcDesc);
		return;
	}

	roads.graph[srcDesc]->kernel = item;
	
	float roadSnapFactor = G::getFloat("roadSnapFactor");
	float roadAngleTolerance = G::getFloat("roadAngleTolerance");

	for (int i = 0; i < item.edges.size(); ++i) {
		growRoadSegment(roads, area, srcDesc, roadType, f, item.edges[i], item.byExample, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * このシードを使って、道路生成する。
 * 道路生成をする前に、既に近くに道路があるかチェックし、ある場合は、スナップさせる。
 * これにより、短い道路セグメントができたり、むやみに交差してしまうことを防ぐ。
 */
void ExRoadGenerator::attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds) {
	float snapThreshold = roadType == RoadEdge::TYPE_AVENUE ? 100.0f : 20.0f;

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

	// 道路生成用のカーネルを合成する
	KDEFeatureItem item;
	synthesizeItem(roads, srcDesc, roadType, G::getFloat("roadOrganicFactor"), item);

	roads.graph[srcDesc]->kernel = item;
	
	float roadSnapFactor = G::getFloat("roadSnapFactor");
	float roadAngleTolerance = G::getFloat("roadAngleTolerance");

	for (int i = 0; i < item.edges.size(); ++i) {
		growRoadSegment(roads, area, srcDesc, roadType, f, item.edges[i], item.byExample, roadSnapFactor, roadAngleTolerance, seeds);
	}
}

/**
 * 指定されたpolylineに従って、srcDesc頂点からエッジを伸ばす。
 * エッジの端点が、srcDescとは違うセルに入る場合は、falseを返却する。
 */
bool ExRoadGenerator::growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, const KDEFeatureItemEdge &ex_edge, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds) {
	// 新しいエッジを生成
	RoadEdgePtr new_edge = RoadEdgePtr(new RoadEdge(roadType, ex_edge.lanes));
	new_edge->polyline.push_back(roads.graph[srcDesc]->pt);
	for (int i = 0; i < ex_edge.edge.size(); ++i) {
		QVector2D pt = roads.graph[srcDesc]->pt + ex_edge.edge[i];
		new_edge->polyline.push_back(pt);
	}

	if (GraphUtil::hasRedundantEdge(roads, srcDesc, new_edge->polyline, angleTolerance)) {
		return false;
	}

	// 他のエッジとの交差をチェック
	if (!byExample) {
		//if (GraphUtil::isIntersect(roads, new_edge->polyline)) return false;
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
			seeds.push_back(tgtDesc);
		}
	}

	RoadEdgeDesc e_desc = GraphUtil::addEdge(roads, srcDesc, tgtDesc, new_edge);
	roads.graph[e_desc]->properties["byExample"] = byExample;

	return true;
}

/**
 * 与えられた頂点の位置と、ほぼ同じ位置のカーネルを探して返却する。見つからない場合は、falseを返却する。
 *
 * @param kf					特徴量
 * @param roadType				道路タイプ
 * @param offsetPosOfVertex		与えられた頂点の、このエリアの中心からのオフセット位置
 */
bool ExRoadGenerator::getItem(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, int roadType, RoadVertexDesc v_desc, KDEFeatureItem &item) {
	// Exampleを使用する領域の外であれば、PM用のパラメータを合成して、使用する。
	if (!RoadGeneratorHelper::isWithinScaledArea(area, G::getFloat("roadExactSimilarityFactor"), roads.graph[v_desc]->pt)) {
		return false;
	}

	BBox currentBBox;
	RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, currentBBox);

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
	
	BBox bbox;
	QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, bbox);

	// 各カーネルについて、非類似度スコアを計算する
	float min_diff = std::numeric_limits<float>::max();
	int min_index = -1;
	bool noLocationError = false;
	for (int i = 0; i < kf.items(roadType).size(); ++i) {
		// 位置の非類似度を計算
		float location_diff = (pt - kf.items(roadType)[i].pt).lengthSquared();
		if (location_diff < 1.0f) {
			min_index = i;
			break;
		}
	}

	if (min_index == -1) return false;

	item = kf.items(roadType)[min_index];

	// Exampleに基づいたカーネルであるとマークする
	item.byExample = true;

	// 極座標系の場合は、ポリラインを変形しておく
	if (G::g["coordiniates"] == "polar") {
		QVector2D offset = roads.graph[v_desc]->pt - area.envelope().midPt();
		float scale = offset.length() / item.pt.length();

		for (int i = 0; i < item.edges.size(); ++i) {
			for (int j = 0; j < item.edges[i].edge.size(); ++j) {
				float r, theta;
				Util::cartesian2polar(item.edges[i].edge[j], r, theta);

				item.edges[i].edge[j].setX(r * scale * cosf(theta));
				item.edges[i].edge[j].setY(r * scale * sinf(theta));
			}
		}
	}

	return true;
}

/**
 * PMに従って、カーネルを合成する
 */
void ExRoadGenerator::synthesizeItem(RoadGraph &roads, RoadVertexDesc v_desc, int roadType, float organicFactor, KDEFeatureItem &item) {
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
	float length = roadType == RoadEdge::TYPE_AVENUE ? 300.0f : 60.0f;

	std::vector<Polyline2D> new_polylines;
	RoadGeneratorHelper::createFourEdges(direction, 10.0f, length, organicFactor, new_polylines);

	for (int i = 0; i < new_polylines.size(); ++i) {
		item.addEdge(new_polylines[i], 1, false, false);
	}
}

/**
 * 境界上の頂点を延長し、近くのエッジにぶつける
 * ただし、現在の頂点のConvexHullの外に出たエッジは、そこで延長をストップする。
 */
void ExRoadGenerator::connectRoads(RoadGraph &roads, float dist_threshold, float angle_threshold) {
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

bool ExRoadGenerator::growRoadOneStep(RoadGraph& roads, RoadVertexDesc srcDesc, const QVector2D& step) {
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
void ExRoadGenerator::connectRoads2(RoadAreaSet &areas, float dist_threshold, float angle_threshold) {
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

void ExRoadGenerator::connectRoads2(RoadAreaSet &areas, int area_id, RoadVertexDesc v_desc, float dist_threshold, float angle_threshold) {
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
