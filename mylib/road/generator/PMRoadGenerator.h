#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../feature/KDEFeature.h"
#include "../feature/KDEFeatureItem.h"

class PMRoadGenerator {
public:
	PMRoadGenerator() {}
	~PMRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area);

private:
	static void generateInitialArterialSeeds(RoadGraph &roads, const Polygon2D &area, std::list<RoadVertexDesc>& seeds);
	static void generateInitialStreetSeeds(RoadGraph &roads, std::list<RoadVertexDesc>& seeds);
	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc id, int roadType, std::list<RoadVertexDesc> &seeds);
	static bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, float direction, float organicFactor, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);
	//static bool removeIntersectingEdges(RoadGraph &roads);
	//static bool isRedundantEdge(RoadGraph &roads, RoadVertexDesc desc, const Polyline2D &polyline, float threshold);
	static void initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction);
	static void initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction1, float direction2);
};

