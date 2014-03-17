#include "../GraphUtil.h"
#include "GenericFeatureExtractor.h"

/**
 * GridでもRadialでもないエッジについて、一般的な特徴量を抽出する。
 */
void GenericFeatureExtractor::extractFeature(RoadGraph& roads, Polygon2D& area, RoadFeature& roadFeature) {
	GenericFeaturePtr gf = GenericFeaturePtr(new GenericFeature(0));

	BBox bbox;

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		// GridまたはRadialのエッジは、スキップ
		if (roads.graph[*ei]->properties["shapeType"].toInt() > 0) continue;

		RoadVertexDesc src = boost::source(*ei, roads.graph);
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		// エリア外ならスキップ
		if (!area.contains(roads.graph[src]->pt) && !area.contains(roads.graph[tgt]->pt)) continue;

		// Bounding Boxを更新
		if (area.contains(roads.graph[src]->pt)) {
			bbox.addPoint(roads.graph[src]->pt);
		}
		if (area.contains(roads.graph[tgt]->pt)) {
			bbox.addPoint(roads.graph[tgt]->pt);
		}

		int roadType = roads.graph[*ei]->type;
		float length = roads.graph[*ei]->getLength();

		gf->addEdge(length, roadType);
	}

	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(roads.graph); vi != vend; ++vi) {
		if (!roads.graph[*vi]->valid) continue;

		if (getNumEdges(roads, *vi, 2, 0) > 0) {
			gf->addNumDiretions(GraphUtil::getNumEdges(roads, *vi, 2), 2);
		}

		if (getNumEdges(roads, *vi, 1, 0) > 0) {
			gf->addNumDiretions(GraphUtil::getNumEdges(roads, *vi, 3), 1);
		}
	}

	// 領域の中心を設定
	gf->setCenter(bbox.midPt());

	gf->computeFeature();

	roadFeature.addFeature(gf);
}

int GenericFeatureExtractor::getNumEdges(RoadGraph &roads, RoadVertexDesc v, int roadType, int shapeType) {
	int count = 0;
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = out_edges(v, roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;
		
		if (!GraphUtil::isRoadTypeMatched(roads.graph[*ei]->type, roadType)) continue;

		if (roads.graph[*ei]->properties["shapeType"] != shapeType) continue;

		count++;
	}

	return count;
}
