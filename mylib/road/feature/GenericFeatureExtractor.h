#pragma once

#include "../RoadGraph.h"
#include "RoadFeature.h"
#include "KDEFeature.h"

class GenericFeatureExtractor {
public:
	GenericFeatureExtractor() {}
	~GenericFeatureExtractor() {}

	static void extractFeature(RoadGraph& roads, Polygon2D& area, RoadFeature& roadFeature);
	static int getNumEdges(RoadGraph &roads, RoadVertexDesc v, int roadType, int shapeType);
};

