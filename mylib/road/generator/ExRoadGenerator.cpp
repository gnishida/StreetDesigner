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
	int i;
	for (i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
		RoadVertexDesc desc = seeds.front();
		seeds.pop_front();

		std::cout << "attemptExpansion (avenue): " << i << " (Seed: " << desc << ")" << std::endl;
		attemptExpansion(roads, area, desc, RoadEdge::TYPE_AVENUE, kf, seeds, additionalSeeds);
	}

	//GraphUtil::removeSelfIntersectingRoads(roads);
	
	if (G::getBool("multiSeeds")) {
		if (G::getBool("connectAvenues")) {
			for (; !additionalSeeds.empty() && i < G::getInt("numIterations"); ++i) {
				RoadVertexDesc desc = additionalSeeds.front();
				additionalSeeds.pop_front();

				std::cout << "attemptExpansion (avenue): " << i << " (Additional Seed: " << desc << ")" << std::endl;

				if (GraphUtil::getDegree(roads, desc) > 1) continue;
				connectAvenues(roads, area, kf, desc, 1000.0f);
			}
		}

		// 指定されたエリアでCropping
		if (G::getBool("cropping")) {
			GraphUtil::extractRoads2(roads, area);
		}
	}

	// clean up
	//GraphUtil::clean(roads);
	//GraphUtil::reduce(roads);
	//GraphUtil::clean(roads);

	// Local streetを生成
	if (G::getBool("generateLocalStreets")) {
		generateStreetSeeds(roads, area, kf, seeds);

		for (int i = 0; !seeds.empty() && i < G::getInt("numIterations"); ++i) {
			RoadVertexDesc desc = seeds.front();
			seeds.pop_front();

			std::cout << "attemptExpansion (street): " << i << std::endl;
			//if (i == 3) break;
			attemptExpansion(roads, area, desc, RoadEdge::TYPE_STREET, kf, seeds, additionalSeeds);
		}
	}

	if (G::getBool("multiSeeds")) {
		// 指定されたエリアでCropping
		if (G::getBool("cropping")) {
			GraphUtil::extractRoads2(roads, area);
		}
	}

	// isolated edgeを削除
	//GraphUtil::removeIsolatedEdges(roads);
	//GraphUtil::clean(roads);
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

	//QVector2D center = area.centroid();
	
	float numExpectedVertices = f.density(RoadEdge::TYPE_AVENUE) * area.area();
	int numSeeds = numExpectedVertices / 30 + 1;
	float threshold = area.area() / numExpectedVertices;
	
	if (G::getBool("multiSeeds")) {
		// シードの数を決定
		BBox bboxTarget = area.envelope();
		std::cout << "Target BBox: (" << bboxTarget.minPt.x() << "," << bboxTarget.minPt.y() << ") - (" << bboxTarget.maxPt.x() << "," << bboxTarget.maxPt.y() << ")" << std::endl;

		BBox bboxExample = f.area().envelope();
		std::cout << "Example BBox: (" << bboxExample.minPt.x() << "," << bboxExample.minPt.y() << ") - (" << bboxExample.maxPt.x() << "," << bboxExample.maxPt.y() << ")" << std::endl;

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
				addAvenueSeed(roads, area, f, QVector2D(x, y), seeds);
			}
		}
	} else {
		addAvenueSeed(roads, area, f, area.envelope().midPt(), seeds);
	}

	// onBoundary頂点について、このエリア内であれば、シードとして使用する
	/*
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (roads.graph[*vi]->onBoundary && area.contains(roads.graph[*vi]->pt)) {
			RoadVertexPtr new_v = RoadVertexPtr(new RoadVertex(roads.graph[*vi]->pt));
			RoadVertexDesc new_v_desc = GraphUtil::addVertex(roads, new_v);

			seeds.push_back(new_v_desc);
		}
	}
	*/
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
bool ExRoadGenerator::addAvenueSeed(RoadGraph &roads, const Polygon2D &area, const KDEFeature &f, const QVector2D &pt, std::list<RoadVertexDesc>& seeds) {
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
	v->kernel = f.items(RoadEdge::TYPE_AVENUE)[min_index];
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

/**
 * このシードを使って、道路生成を再度トライしたい場合は、falseを返却する。
 */
void ExRoadGenerator::attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds) {
	// カーネル割り当てに失敗したら、道路生成をキャンセルする
	if (!getItem(roads, area, f, roadType, srcDesc, roads.graph[srcDesc]->kernel)) {
		return;
	}
	
	std::vector<bool> isRedundant;
	for (int i = 0; i < roads.graph[srcDesc]->kernel.edges.size(); ++i) {
		if (RoadGeneratorHelper::isRedundantEdge(roads, srcDesc, roads.graph[srcDesc]->kernel.edges[i].edge)) {
			isRedundant.push_back(true);
		} else {
			isRedundant.push_back(false);
		}
	}

	float snapFactor = 0.7f;
	float angleTolerance = 1.2566f;
	if (roads.graph[srcDesc]->kernel.confident) {
		snapFactor = 0.1f;
		angleTolerance = 0.1f;
	}

	for (int i = 0; i < roads.graph[srcDesc]->kernel.edges.size(); ++i) {
		if (!isRedundant[i]) {
			if (!G::getBool("multiSeeds")) {
				growRoadSegment(roads, area, srcDesc, roadType, f, roads.graph[srcDesc]->kernel.edges[i], roads.graph[srcDesc]->kernel.confident, 0.7f, 1.2566f, seeds, additionalSeeds);
			} else {
				growRoadSegment2(roads, area, srcDesc, roadType, f, roads.graph[srcDesc]->kernel.edges[i], seeds, additionalSeeds);
			}
		}
	}
}

/**
 * 指定されたpolylineに従って、srcDesc頂点からエッジを伸ばす。
 * エッジの端点が、srcDescとは違うセルに入る場合は、falseを返却する。
 */
bool ExRoadGenerator::growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, const KDEFeatureItemEdge &ex_edge, bool confident, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds) {
	// srcDescを含む、Example領域のBBoxに相当するBBoxを取得
	// (BBoxは、ターゲット領域の中心を原点とする座標系となっている）
	BBox currentBBox;
	RoadGeneratorHelper::modulo(area, f.area(), roads.graph[srcDesc]->pt, currentBBox);

	// 新しいエッジを生成
	RoadEdgePtr new_edge = RoadEdgePtr(new RoadEdge(roadType, ex_edge.lanes));
	new_edge->polyline.push_back(roads.graph[srcDesc]->pt);
	for (int i = 0; i < ex_edge.edge.size(); ++i) {
		QVector2D pt = roads.graph[srcDesc]->pt + ex_edge.edge[i];
		new_edge->polyline.push_back(pt);
	}

	RoadVertexDesc tgtDesc;

	// 交差していないかチェック
	bool intersected = false;
	QVector2D intPoint;
	RoadEdgeDesc closestEdge;
	if (RoadGeneratorHelper::intersects(roads, srcDesc, new_edge->polyline, closestEdge, intPoint)) {
		intersected = true;
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, intPoint);

		confident = false;
	}

	if (confident) {
		snapFactor = 0.1f;
		angleTolerance = 0.1f;
	}

	// スナップできるか？
	//if (GraphUtil::getVertex(roads, new_edge->polyline.last(), new_edge->polyline.length() * 0.7f, srcDesc, tgtDesc)) {
	if (RoadGeneratorHelper::canSnapToVertex(roads, new_edge->polyline.last(), new_edge->polyline, new_edge->polyline.length() * snapFactor, srcDesc, tgtDesc)) {
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		std::reverse(new_edge->polyline.begin(), new_edge->polyline.end());
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else if (RoadGeneratorHelper::canSnapToEdge(roads, new_edge->polyline.last(), new_edge->polyline.length() * snapFactor, srcDesc, closestEdge, intPoint)) {
		tgtDesc = GraphUtil::splitEdge(roads, closestEdge, intPoint);
		GraphUtil::movePolyline(roads, new_edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, new_edge->polyline, angleTolerance)) return false;
	} else {
		if (intersected) return false;

		// 頂点を追加
		RoadVertexPtr v = RoadVertexPtr(new RoadVertex(new_edge->polyline.last()));
		tgtDesc = GraphUtil::addVertex(roads, v);
		roads.graph[tgtDesc]->properties["parent"] = srcDesc;

		if (area.contains(new_edge->polyline.last())) {
			// シードに追加する
			seeds.push_back(tgtDesc);
			//seeds.push_front(tgtDesc);
		}
	}

	GraphUtil::addEdge(roads, srcDesc, tgtDesc, new_edge);

	return true;
}

/**
 * "multiSeeds"用の道路生成関数。
 * 指定されたpolylineに従って、srcDesc頂点からエッジを伸ばす。
 * エッジの端点が、srcDescとは違うセルに入る場合は、falseを返却する。
 */
bool ExRoadGenerator::growRoadSegment2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, const KDEFeatureItemEdge &edge, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds) {
	RoadVertexDesc tgtDesc;
	RoadVertexDesc snapDesc;

	// srcDescを含む、Example領域のBBoxに相当するBBoxを取得
	// (BBoxは、ターゲット領域の中心を原点とする座標系となっている）
	BBox currentBBox;
	QVector2D moduleSrcPt = RoadGeneratorHelper::modulo(area, f.area(), roads.graph[srcDesc]->pt, currentBBox);

	bool snapped = false;
	bool intersected = false;
	bool outside = false;
	QVector2D outsidePt;

	bool toBeSeed = true;
	bool toBeAdditionalSeed = false;

	Polyline2D polyline;
	polyline.push_back(roads.graph[srcDesc]->pt);

	QVector2D pt;
	QVector2D prevPt = roads.graph[srcDesc]->pt;
	for (int j = 0; j < edge.edge.size(); ++j) {
		pt = roads.graph[srcDesc]->pt + edge.edge[j];

		// INTERSECTS -- If edge intersects other edge
		QVector2D intPoint;
		RoadEdgeDesc closestEdge;
		intersected = RoadGeneratorHelper::intersects(roads, prevPt, pt, closestEdge, intPoint);
		prevPt = pt;
		if (intersected) {
			RoadVertexDesc src = boost::source(closestEdge, roads.graph);
			RoadVertexDesc tgt = boost::target(closestEdge, roads.graph);

			// 自分のエッジに交差した場合は、このエッジのgrowをキャンセル
			if (src == srcDesc || tgt == srcDesc) return true;

			pt = intPoint;
			toBeSeed = false;
		}

		polyline.push_back(pt);

		// Densityをチェック
		if (roadType == RoadEdge::TYPE_STREET) {
			float density = GraphUtil::getNumVertices(roads, pt, 400);
			//if (density >= (f.density(roadType) + f.density(RoadEdge::TYPE_AVENUE)) * 400.0f * 400.0f * M_PI * 3) return false;
		} else {
			//float density = GraphUtil::getNumVertices(roads, pt, 400);
			//if (density >= f.density(roadType) * 400.0f * 400.0f * M_PI) return false;
		}

		float threshold;
		if (roadType == RoadEdge::TYPE_STREET) {
			//threshold = std::max(0.25f * (float)edge[j].length(), 10.0f);
			threshold = std::min(0.25f * (float)edge.edge[j].length(), 10.0f);
		} else {
			if (intersected || j == edge.edge.size() - 1) {
				threshold = std::min(0.5f * (float)edge.edge[j].length(), 40.0f);
			} else {
				threshold = std::min(0.25f * (float)edge.edge[j].length(), 20.0f);
			}
		}

		// 近くに頂点があるか？
		RoadVertexDesc desc;
		RoadEdgeDesc e_desc;
		QVector2D closestPt;		
		if (RoadGeneratorHelper::canSnapToVertex(roads, pt, polyline, threshold, srcDesc, desc)) {
			snapDesc = desc;
			snapped = true;
			intersected = false;
			toBeSeed = false;

			QString str;
			str.setNum(srcDesc);
			if (roads.graph[snapDesc]->properties.contains("uncles")) {
				roads.graph[snapDesc]->properties["uncles"] = roads.graph[snapDesc]->properties["uncles"].toString() + "," + str;
			} else {
				roads.graph[snapDesc]->properties["uncles"] = str;
			}
		} else if (RoadGeneratorHelper::canSnapToEdge(roads, pt, threshold * 0.2f, srcDesc, e_desc, closestPt)) {
			// 実験。既存のエッジを分割させないよう、キャンセルさせてみる
			if (roadType == RoadEdge::TYPE_AVENUE && roads.graph[e_desc]->type == RoadEdge::TYPE_AVENUE) {
				//return false;
			} 
			
			toBeSeed = false;

			snapDesc = GraphUtil::splitEdge(roads, e_desc, pt);
			roads.graph[snapDesc]->properties["parent"] = srcDesc;
			snapped = true;
			intersected = false;
		} else {
			BBox nextBBox;
			QVector2D moduloPt = RoadGeneratorHelper::modulo(area, f.area(), pt, nextBBox);

			if (!outside && (!currentBBox.contains(pt) || !f.area().contains(moduloPt))) {
				outside = true;
				toBeAdditionalSeed = true;

				if (currentBBox.intersects(roads.graph[srcDesc]->pt, pt, outsidePt)) {
					pt = outsidePt;
					polyline[polyline.size() - 1] = outsidePt;
				}
			}
		}

		if (intersected) {
			// 交差相手のエッジを分割
			tgtDesc = GraphUtil::splitEdge(roads, closestEdge, pt);
		}

		if (snapped || intersected || outside) break;
	}

	if (!intersected) {
		if (G::getBool("invadingCheck")) {
			// 他の頂点のテリトリーに侵入したら、頂点生成、エッジ生成を却下する
			if (RoadGeneratorHelper::invadingTerritory(roads, pt, srcDesc, roads.graph[srcDesc]->pt + edge.edge[edge.edge.size() - 1])) return true;
		}

		if (snapped) {
			Polyline2D old_polyline = polyline;

			GraphUtil::movePolyline(roads, polyline, roads.graph[srcDesc]->pt, roads.graph[snapDesc]->pt);
			tgtDesc = snapDesc;

			// デバッグ用に、スナップ画像を保存
			if (G::getBool("saveSnappingImages")) {
				RoadGeneratorHelper::saveSnappingImage(roads, area, srcDesc, old_polyline, polyline, snapDesc, "snap");
			}
		} else {
			// 頂点を追加
			RoadVertexPtr v = RoadVertexPtr(new RoadVertex(pt));
			tgtDesc = GraphUtil::addVertex(roads, v);
			roads.graph[tgtDesc]->properties["parent"] = srcDesc;
		}
			
		if (outside) {
			roads.graph[tgtDesc]->onBoundary = true;
		}
	}

	if (GraphUtil::hasSimilarEdge(roads, srcDesc, tgtDesc, polyline)) return false;

	// エッジを追加
	RoadEdgeDesc e = GraphUtil::addEdge(roads, srcDesc, tgtDesc, roadType, edge.lanes);
	roads.graph[e]->polyline = polyline;

	// シードに追加
	if (toBeSeed) {
		if (!toBeAdditionalSeed) {
			seeds.push_back(tgtDesc);

			// 追加した頂点に、カーネルを割り当てる
			roads.graph[tgtDesc]->kernel = getItem2(roads, area, f, roadType, tgtDesc);
		} else {
			additionalSeeds.push_back(tgtDesc);
		}
	}

	return true;
}

/**
 * 与えられたエッジの方向、長さを含むデータを検索し、近いものを返却する。
 * 繰り返しを防ぐために、モジュラを実装。
 *
 * @param kf					特徴量
 * @param roadType				道路タイプ
 * @param offsetPosOfVertex		与えられた頂点の、このエリアの中心からのオフセット位置
 */
bool ExRoadGenerator::getItem(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, int roadType, RoadVertexDesc v_desc, KDEFeatureItem &item) {
	BBox currentBBox;
	RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, currentBBox);

	// 当該頂点から出るエッジをリストアップする
	QList<Polyline2D> polylines;
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

	/*
	// currentBBox内のカーネルをリストアップ
	QHash<int, bool> neighborKernels;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// 自分自身ならスキップ
		if (*vi == v_desc) continue;

		// 別のセルの頂点は、スキップする
		if (!currentBBox.contains(roads.graph[*vi]->pt)) continue;

		neighborKernels[roads.graph[*vi]->kernel.id] = true;
	}
	*/

	float weightEdge = G::getFloat("weightEdge");
	float weightLocation = G::getFloat("weightLocation");
	float weightRepetition = G::getFloat("weightRepetition");

	// 各カーネルについて、非類似度スコアを計算する
	float min_diff = std::numeric_limits<float>::max();
	int min_index = -1;
	bool noLocationError = false;
	for (int i = 0; i < kf.items(roadType).size(); ++i) {
		// 使用済みのカーネルは使わない
		//if (neighborKernels.contains(kf.items(roadType)[i].id)) continue;

		// エッジの非類似度を計算
		/*float edge_diff = 0.0f;
		for (int j = 0; j < polylines.size(); ++j) {
			edge_diff += kf.items(roadType)[i].getMinDistance(polylines[j]);
		}*/

		// 位置の非類似度を計算
		float location_diff = 0.0f;
		if (neighbors.size() == 0) { // 最初のシードには隣接ノードがないので
			BBox bbox;
			QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, bbox);
			location_diff += (pt - kf.items(roadType)[i].pt).length();
		} else {
			for (int j = 0; j < neighbors.size(); ++j) { // 隣接頂点のカーネルからの相対座標を使用
				BBox bbox;
				QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[neighbors[j]]->kernel.pt - polylines[j].last() + polylines[j][0] + area.envelope().midPt(), bbox);

				if (G::g["coordiniates"] == "cartesian") { // カーテシアン座標系の場合
					location_diff += (pt - kf.items(roadType)[i].pt).length();
				} else { // 極座標系の場合
					location_diff += Util::diffAngle(roads.graph[v_desc]->pt - area.envelope().midPt(), kf.items(roadType)[i].pt) * 100.0f;
				}
			}
		}

		// フィッティングスコアを計算
		//float diff = edge_diff * weightEdge + location_diff * weightLocation;// + repetition * weightRepetition;
		//float diff = edge_diff + location_diff;
		if (location_diff < 1.0f) {
			min_index = i;
			break;
		}
	}

	if (min_index >= 0) {
		item = kf.items(roadType)[min_index];
		item.confident = true;
	} else {
		// カーネル割当なしの場合は、パラメータを合成する
		float direction = 0.0f;

		if (polylines.size() > 0) {
			QVector2D vec = polylines[0].last() - polylines[0][0];
			direction = atan2f(vec.y(), vec.x());
		}

		std::vector<Polyline2D> new_polylines;
		RoadGeneratorHelper::createFourEdges(direction, 10.0f, 0.1, new_polylines);

		for (int i = 0; i < new_polylines.size(); ++i) {
			item.addEdge(new_polylines[i], 1, false, false);
		}
	}


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
 * 与えられたエッジの方向、長さを含むデータを検索し、近いものを返却する。
 * 繰り返しを防ぐために、モジュラを実装。
 *
 * @param kf					特徴量
 * @param roadType				道路タイプ
 * @param offsetPosOfVertex		与えられた頂点の、このエリアの中心からのオフセット位置
 */
KDEFeatureItem ExRoadGenerator::getItem2(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, int roadType, RoadVertexDesc v_desc) {
	BBox currentBBox;
	RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, currentBBox);

	// 当該頂点から出るエッジをリストアップする
	QList<Polyline2D> polylines;
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

	// 周辺の頂点のカーネルをリストアップし、最短距離を計算する
	QMap<int, float> neighborKernels;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// 自分自身ならスキップ
		if (*vi == v_desc) continue;

		// 別のセルの頂点は、スキップする
		if (!currentBBox.contains(roads.graph[*vi]->pt)) continue;

		float min_dist = std::numeric_limits<float>::max();
		if (neighborKernels.contains(roads.graph[*vi]->kernel.id)) {
			min_dist = neighborKernels[roads.graph[*vi]->kernel.id];
		}
		float dist = (roads.graph[*vi]->pt - roads.graph[v_desc]->pt).length();
		if (dist < min_dist) {
			neighborKernels[roads.graph[*vi]->kernel.id] = dist;
		}
	}

	float weightEdge = G::getFloat("weightEdge");
	float weightLocation = G::getFloat("weightLocation");
	float weightRepetition = G::getFloat("weightRepetition");

	// 各カーネルについて、非類似度スコアを計算する
	float min_diff = std::numeric_limits<float>::max();
	int min_index = -1;
	bool noLocationError = false;
	for (int i = 0; i < kf.items(roadType).size(); ++i) {
		// 繰り返し度を計算
		float repetition = 0.0f;
		if (neighborKernels.contains(kf.items(roadType)[i].id)) {
			repetition = 1.0f / neighborKernels[kf.items(roadType)[i].id];
		}

		// エッジの非類似度を計算
		float edge_diff = 0.0f;
		for (int j = 0; j < polylines.size(); ++j) {
			edge_diff += kf.items(roadType)[i].getMinDistance(polylines[j]);
		}

		// 位置の非類似度を計算
		float location_diff = 0.0f;
		if (neighbors.size() == 0) { // 最初のシードには隣接ノードがないので
			BBox bbox;
			QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[v_desc]->pt, bbox);
			location_diff += (pt - kf.items(roadType)[i].pt).length();
		} else {
			for (int j = 0; j < neighbors.size(); ++j) { // 隣接頂点のカーネルからの相対座標を使用
				BBox bbox;
				QVector2D pt = RoadGeneratorHelper::modulo(area, kf.area(), roads.graph[neighbors[j]]->kernel.pt - polylines[j].last() + polylines[j][0] + area.envelope().midPt(), bbox);

				if (G::g["coordiniates"] == "cartesian") { // カーテシアン座標系の場合
					location_diff += (pt - kf.items(roadType)[i].pt).length();
				} else { // 極座標系の場合
					location_diff += Util::diffAngle(roads.graph[v_desc]->pt - area.envelope().midPt(), kf.items(roadType)[i].pt) * 100.0f;
				}
			}
		}

		// フィッティングスコアを計算
		float diff = edge_diff * weightEdge + location_diff * weightLocation + repetition * weightRepetition;
		//float diff = edge_diff + location_diff;
		if (diff < min_diff) {
			min_diff = diff;
			min_index = i;
			noLocationError = (location_diff < 0.1f) ? true : false;
		}
	}

	KDEFeatureItem item = kf.items(roadType)[min_index];

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

	return item;
}

/**
 * セル境界上の頂点を、近くの頂点にスナップさせる。
 * 近くに頂点がなければ、近くのエッジにスナップさせる。
 * 近くに、頂点もエッジもなければ、この頂点と、そこから出るエッジを、全て削除する。
 *
 * @param thresholdRatio			この頂点から出る１本のエッジの長さ x threshold を、しきい値とする
 */
void ExRoadGenerator::connectAvenues(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, RoadVertexDesc v_desc, float thresholdRatio) {
	RoadVertexDesc srcDesc;
	RoadEdgeDesc srcEdge;

	// この頂点の親の頂点を取得
	RoadOutEdgeIter oei, oeend;
	for (boost::tie(oei, oeend) = out_edges(v_desc, roads.graph); oei != oeend; ++oei) {
		if (!roads.graph[*oei]->valid) continue;

		srcDesc = boost::target(*oei, roads.graph);
		srcEdge = *oei;
		break;
	}

	float threshold = roads.graph[srcEdge]->getLength() * thresholdRatio;

	BBox currentBBox;
	RoadGeneratorHelper::modulo(area, f.area(), roads.graph[srcDesc]->pt, currentBBox);

	// 近くに頂点があれば、スナップする
	RoadVertexDesc snapDesc;
	if (RoadGeneratorHelper::canSnapToVertex2(roads, roads.graph[v_desc]->pt, threshold, srcDesc, srcEdge, snapDesc)) {
		GraphUtil::snapVertex(roads, v_desc, snapDesc);

		if (G::getBool("saveConnectingImages")) {
			Polyline2D old_polyline = roads.graph[srcEdge]->polyline;
			RoadEdgeDesc new_edge = GraphUtil::getEdge(roads, srcDesc, snapDesc);

			RoadGeneratorHelper::saveSnappingImage(roads, area, srcDesc, old_polyline, roads.graph[new_edge]->polyline, snapDesc, "connect"); 
		}

		return;
	}

	// 近くにエッジがあれば、スナップする
	RoadEdgeDesc snapEdge;
	QVector2D closestPt;
	if (RoadGeneratorHelper::canSnapToEdge(roads, roads.graph[v_desc]->pt, threshold, srcDesc, snapEdge, closestPt)) {
		snapDesc = GraphUtil::splitEdge(roads, snapEdge, closestPt);
		GraphUtil::snapVertex(roads, v_desc, snapDesc);
		return;
	}

	// 近くに、頂点もエッジもない場合は、この頂点と、エッジを全て削除する。
	roads.graph[srcEdge]->valid = false;
	roads.graph[v_desc]->valid = false;
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
