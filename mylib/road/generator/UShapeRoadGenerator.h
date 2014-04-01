#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../feature/KDEFeature.h"
#include "../feature/KDEFeatureItem.h"

class UShapeRoadGenerator {
public:
	UShapeRoadGenerator() {}
	~UShapeRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf);

private:
	static void generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds);
	static bool addAvenueSeed(RoadGraph &roads, const const Polygon2D &area, const KDEFeature &f, QVector2D &pt, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, const KDEFeature& f, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds2(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds, std::list<RoadVertexDesc> &additionalSeeds);
	static void attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, std::list<RoadVertexDesc> &seeds);
	static bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, const KDEFeature& f, const KDEFeatureItemEdge &ex_edge, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	static bool getItem(RoadGraph &roads, const Polygon2D &area, const KDEFeature& kf, int roadType, RoadVertexDesc v_desc, KDEFeatureItem &item);
	static void synthesizeItem(RoadGraph &roads, const KDEFeature &f, RoadVertexDesc v_desc, int roadType, KDEFeatureItem &item);

};

