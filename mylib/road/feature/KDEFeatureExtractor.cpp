#include <time.h>
#include "../../common/global.h"
#include "../../common/Util.h"
#include "../GraphUtil.h"
#include "KDEFeatureExtractor.h"

/**
 * KDEベースでの特徴量を抽出する。
 */
void KDEFeatureExtractor::extractFeature(RoadGraph& roads, Polygon2D& area, RoadFeature& roadFeature) {
	roadFeature.clear();

	KDEFeaturePtr kf = KDEFeaturePtr(new KDEFeature(0));

	QVector2D center = area.envelope().midPt();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Avenueのみを抽出する
	RoadGraph temp_roads;
	time_t start = clock();
	GraphUtil::copyRoads(roads, temp_roads);
	time_t end = clock();
	std::cout << "Elapsed time for copying the roads: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	if (G::getBool("exactCut")) {
		GraphUtil::extractRoads2(temp_roads, area);
	}
	start = clock();
	GraphUtil::extractRoads(temp_roads, RoadEdge::TYPE_AVENUE | RoadEdge::TYPE_BOULEVARD);
	end = clock();
	std::cout << "Elapsed time for extracting only the avenues: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	start = clock();
	GraphUtil::clean(temp_roads);
	end = clock();
	std::cout << "Elapsed time for cleaning the avenues: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	start = clock();
	GraphUtil::reduce(temp_roads);
	end = clock();
	std::cout << "Elapsed time for reducing the avenues: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	// roundaboutを削除する
	//GraphUtil::removeRoundabout(temp_roads);

	// linkを削除する
	start = clock();
	GraphUtil::removeLinkEdges(temp_roads);
	end = clock();
	std::cout << "Elapsed time for removing links: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	start = clock();
	GraphUtil::reduce(temp_roads);
	end = clock();
	std::cout << "Elapsed time for reducing links: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	GraphUtil::removeIsolatedVertices(temp_roads);
	start = clock();
	GraphUtil::clean(temp_roads);
	end = clock();
	std::cout << "Elapsed time for cleaning the avenues: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	// 特徴量を抽出
	int num_vertices = extractAvenueFeature(temp_roads, area, kf);
	if (G::getBool("perturbation")) {
		extractAvenueFeature(temp_roads, area, kf, true);
	}

	BBox bbox = area.envelope();
	std::cout << "Area: " << area.area() << std::endl;
	std::cout << "BBox: " << bbox.dx() << "," << bbox.dy() << std::endl;
	std::cout << "Num avenue vertices: " << num_vertices << std::endl;

	kf->setDensity(RoadEdge::TYPE_AVENUE, num_vertices / area.area());

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// streetのみを抽出する
	start = clock();
	GraphUtil::copyRoads(roads, temp_roads);
	end = clock();
	std::cout << "Elapsed time for copying the roads: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	if (G::getBool("exactCut")) {
		GraphUtil::extractRoads2(temp_roads, area);
	}
	start = clock();
	GraphUtil::extractRoads(temp_roads, RoadEdge::TYPE_STREET);
	end = clock();
	std::cout << "Elapsed time for extracting the streets: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	//GraphUtil::reduce(temp_roads);  <- わざとreduceしない

	// linkを削除する
	start = clock();
	GraphUtil::removeLinkEdges(temp_roads);
	end = clock();
	std::cout << "Elapsed time for removing links: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;
	GraphUtil::removeIsolatedVertices(temp_roads);
	start = clock();
	GraphUtil::clean(temp_roads);
	end = clock();
	std::cout << "Elapsed time for cleaning the streets: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	// 特徴量を抽出
	num_vertices = extractStreetFeature(temp_roads, area, kf);
	if (G::getBool("perturbation")) {
		extractStreetFeature(temp_roads, area, kf, true);
	}

	kf->setDensity(RoadEdge::TYPE_STREET, num_vertices / area.area());

	kf->setWeight(1.0f);
	kf->setCenter(center);

	// 領域を少しだけ大きくする。これにより、抽出された道路網の交差点が、確実に領域内と判定されるはず。
	for (int i = 0; i < area.size(); ++i) {
		area[i] = (area[i] - center) * 1.02f + center;
	}
	kf->setArea(area);

	roadFeature.addFeature(kf);
}

int KDEFeatureExtractor::extractAvenueFeature(RoadGraph &orig_roads, const Polygon2D &area, KDEFeaturePtr kf, bool perturbation) {
	RoadGraph roads;
	GraphUtil::copyRoads(orig_roads, roads);

	QVector2D center = area.centroid();

	if (perturbation) {
		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
			if (!roads.graph[*vi]->valid) continue;

			RoadVertexDesc closestVertexDesc = GraphUtil::getVertex(roads, roads.graph[*vi]->pt, *vi);
			float min_dist = (roads.graph[*vi]->pt - roads.graph[closestVertexDesc]->pt).length() * 0.5f;

			// 交差点を少し動かす
			GraphUtil::moveVertex(roads, *vi, roads.graph[*vi]->pt + QVector2D(Util::uniform_rand(-min_dist, min_dist), Util::uniform_rand(-min_dist, min_dist)));
		}

		GraphUtil::saveRoads(roads, "perturbed_avenues.gsm");
	}

	time_t start = clock();
	int num_vertices = 0;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// エリア外の頂点はスキップ
		if (!area.contains(roads.graph[*vi]->pt)) continue;

		num_vertices++;

		// エッジの数が2以下なら、スキップ
		//if (GraphUtil::getNumEdges(temp_roads, *vi) <= 2) continue;

		// 頂点の座標の、エリア中心からのオフセットを登録
		KDEFeatureItem item(kf->numItems(RoadEdge::TYPE_AVENUE));
		item.pt = roads.graph[*vi]->pt - center;

		// 近接頂点までの距離を登録
		RoadVertexDesc nearestVertexDesc = GraphUtil::getVertex(roads, roads.graph[*vi]->pt, *vi);
		item.territory = (roads.graph[nearestVertexDesc]->pt - roads.graph[*vi]->pt).length();

		// 各outing edgeを登録
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			RoadVertexDesc tgt = boost::target(*ei, roads.graph);
			int degree = GraphUtil::getNumEdges(roads, tgt);

			Polyline2D polyline = GraphUtil::finerEdge(roads, *ei, 20.0f);
			if ((polyline[0] - roads.graph[*vi]->pt).lengthSquared() > (polyline[0] - roads.graph[tgt]->pt).lengthSquared()) {
				std::reverse(polyline.begin(), polyline.end());
			}
			for (int i = 1; i < polyline.size(); ++i) {
				polyline[i] -= polyline[0];
			}
			polyline.erase(polyline.begin());
			item.addEdge(polyline, degree == 1, roads.graph[tgt]->onBoundary);
		}

		kf->addItem(RoadEdge::TYPE_AVENUE, item);
	}
	time_t end = clock();

	std::cout << "Elapsed time for extracting features from the avenues: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	return num_vertices;
}


int KDEFeatureExtractor::extractStreetFeature(RoadGraph &orig_roads, const Polygon2D &area, KDEFeaturePtr kf, bool perturbation) {
	RoadGraph roads;
	GraphUtil::copyRoads(orig_roads, roads);

	QVector2D center = area.centroid();

	if (perturbation) {
		RoadVertexIter vi, vend;
		for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
			if (!roads.graph[*vi]->valid) continue;

			RoadVertexDesc closestVertexDesc = GraphUtil::getVertex(roads, roads.graph[*vi]->pt, *vi);
			float min_dist = (roads.graph[*vi]->pt - roads.graph[closestVertexDesc]->pt).length() * 0.5f;

			// 交差点を少し動かす
			GraphUtil::moveVertex(roads, *vi, roads.graph[*vi]->pt + QVector2D(Util::uniform_rand(-min_dist, min_dist), Util::uniform_rand(-min_dist, min_dist)));
		}

		GraphUtil::saveRoads(roads, "perturbed_streets.gsm");
	}

	time_t start = clock();
	int num_vertices = 0;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		// エリア外の頂点はスキップ
		if (!area.contains(roads.graph[*vi]->pt)) continue;

		num_vertices++;

		// エッジの数が2以下なら、スキップ
		//if (GraphUtil::getNumEdges(temp_roads, *vi) <= 2) continue;

		// 頂点の座標の、エリア中心からのオフセットを登録
		KDEFeatureItem item(kf->numItems(RoadEdge::TYPE_STREET));
		item.pt = roads.graph[*vi]->pt - center;

		// 近接頂点までの距離を登録
		RoadVertexDesc nearestVertexDesc = GraphUtil::getVertex(roads, roads.graph[*vi]->pt, *vi);
		item.territory = (roads.graph[nearestVertexDesc]->pt - roads.graph[*vi]->pt).length();

		// 各outing edgeを登録
		RoadOutEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::out_edges(*vi, roads.graph); ei != eend; ++ei) {
			RoadVertexDesc tgt = boost::target(*ei, roads.graph);
			int degree = GraphUtil::getNumEdges(roads, tgt);

			Polyline2D polyline;
			if ((roads.graph[*ei]->polyLine[0] - roads.graph[*vi]->pt).lengthSquared() > (roads.graph[*ei]->polyLine[0] - roads.graph[tgt]->pt).lengthSquared()) {
				std::reverse(roads.graph[*ei]->polyLine.begin(), roads.graph[*ei]->polyLine.end());
			}
			for (int i = 1; i < roads.graph[*ei]->polyLine.size(); ++i) {
				polyline.push_back(roads.graph[*ei]->polyLine[i] - roads.graph[*vi]->pt);
			}
			item.addEdge(polyline, degree == 1, false);
		}

		kf->addItem(RoadEdge::TYPE_STREET, item);
	}
	time_t end = clock();
	std::cout << "Elapsed time for extracting features from the streets: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	return num_vertices;
}
