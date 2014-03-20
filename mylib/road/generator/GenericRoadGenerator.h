#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../feature/GenericFeature.h"

class GenericRoadGenerator {
public:
	GenericRoadGenerator() {}
	~GenericRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const GenericFeature& gf);

private:
	static void generateInitialSeeds(RoadGraph &roads, const Polygon2D &area, const GenericFeature& gf, std::list<RoadVertexDesc>& seeds);
	static bool generateInitialStreetSeeds(RoadGraph &roads, const GenericFeature& gf, std::list<RoadVertexDesc>& seeds);

	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const GenericFeature& gf, std::list<RoadVertexDesc> &newSeeds);
	static bool intersects(RoadGraph &roads, const QVector2D& p0, const QVector2D& p1, RoadEdgeDesc &eiClosest, QVector2D &closestIntPt);

	static void removeDeadEnds(RoadGraph &roads);
};

