#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../RoadAreaSet.h"
#include "../feature/KDEFeature.h"

class ExRoadGenerator {
public:
	ExRoadGenerator() {}
	~ExRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf);

private:
	static void generateRoadsOnBoundary(RoadGraph &roads, const Polygon2D &area, int roadType, int lanes);
	static void generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds);
	static bool addAvenueSeed(RoadGraph &roads, const const Polygon2D &area, const KDEFeature &f, QVector2D &pt, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds2(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds);
	static void attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds);
	static bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, const KDEFeatureItemEdge &ex_edge, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	static bool getItem(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, int roadType, RoadVertexDesc v_desc, KDEFeatureItem &item);
	static void synthesizeItem(RoadGraph &roads, const KDEFeature &f, RoadVertexDesc v_desc, int roadType, float organicFactor, KDEFeatureItem &item);

public:
	static void connectRoads(RoadGraph &roads, float dist_threshold, float angle_threshold);
	static bool growRoadOneStep(RoadGraph& roads, RoadVertexDesc srcDesc, const QVector2D& step);
	static void connectRoads2(RoadAreaSet &areas, float dist_threshold, float angle_threshold);
	static void connectRoads2(RoadAreaSet &areas, int area_id, RoadVertexDesc v_desc, float dist_threshold, float angle_threshold);
};
