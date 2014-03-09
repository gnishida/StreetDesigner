#pragma once

#include "../RoadGraph.h"
#include "RoadFeature.h"
#include "KDEFeature.h"

class KDEFeatureExtractor {
public:
	KDEFeatureExtractor() {}
	~KDEFeatureExtractor() {}

	static void extractFeature(RoadGraph& roads, Polygon2D& area, RoadFeature& roadFeature);

	static int extractAvenueFeature(RoadGraph &roads, const Polygon2D &area, KDEFeaturePtr kf, bool perturbation = false);
	static int extractStreetFeature(RoadGraph &orig_roads, const Polygon2D &area, KDEFeaturePtr kf, bool perturbation = false);
};

