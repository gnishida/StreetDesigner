#pragma once

#include "../../common/Polygon2D.h"
#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"

class PMRoadGenerator {
public:
	PMRoadGenerator() {}
	~PMRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area);

private:
	static void generateInitialArterialSeeds(RoadGraph &roads, const Polygon2D &area, std::vector<RoadVertexDesc>& seeds);
	static void generateInitialStreetSeeds(RoadGraph &roads, std::vector<RoadVertexDesc>& seeds);
	static RoadEdgePtr attemptExpansion(RoadGraph &roads, RoadVertexDesc id, float& direction, int roadType);
	//static bool removeIntersectingEdges(RoadGraph &roads);
	//static bool isRedundantEdge(RoadGraph &roads, RoadVertexDesc desc, const Polyline2D &polyline, float threshold);
	static void initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction);
	static void initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction1, float direction2);
};

