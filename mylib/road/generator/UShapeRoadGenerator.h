#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../RoadAreaSet.h"
#include "../feature/ExFeature.h"

class UShapeRoadGenerator {
public:
	UShapeRoadGenerator() {}
	~UShapeRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& feature);

private:
	static void generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& f, std::list<RoadVertexDesc>& seeds);
	static bool addAvenueSeed(RoadGraph &roads, const const Polygon2D &area, ExFeature &f, const QVector2D &pt, const QVector2D &ex_pt, int group_id, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, ExFeature& kf, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	static bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, ExFeature& f, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	static void synthesizeItem(RoadGraph &roads, ExFeature &f, RoadVertexDesc v_desc, int roadType, std::vector<RoadEdgePtr> &edges);

public:
	static void connectRoads(RoadGraph &roads, float dist_threshold, float angle_threshold);
	static bool growRoadOneStep(RoadGraph& roads, RoadVertexDesc srcDesc, const QVector2D& step);
	static void connectRoads2(RoadAreaSet &areas, float dist_threshold, float angle_threshold);
	static void connectRoads2(RoadAreaSet &areas, int area_id, RoadVertexDesc v_desc, float dist_threshold, float angle_threshold);
};

