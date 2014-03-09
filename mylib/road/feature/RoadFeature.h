#pragma once

#include <vector>
#include "AbstractFeature.h"
#include "GridFeature.h"
#include "RadialFeature.h"
#include "KDEFeature.h"
#include "GenericFeature.h"

class RoadFeature {
public:
	static QString version;

public:
	std::vector<AbstractFeaturePtr> features;

public:
	RoadFeature() {}
	~RoadFeature() {}

	void clear();
	void load(QString filename);
	void save(QString filename);

	void addFeature(AbstractFeaturePtr feature);

	void normalize();

	void rotate(float deg);
	void scale(const Polygon2D &area);
};

