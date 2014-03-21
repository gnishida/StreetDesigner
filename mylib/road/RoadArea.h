#pragma once

#include <boost/shared_ptr.hpp>
#include "../common/Polygon2D.h"
#include "../common/Polyline3D.h"
#include "../render/Terrain.h"
#include "RoadGraph.h"

class RoadArea {
public:
	Polygon2D area;
	Polyline3D area3D;
	RoadGraph roads;

public:
	RoadArea();
	RoadArea(const Polygon2D &area);
	~RoadArea() {}

	void clear();

	void load(QDomNode& node);
	void save(QDomDocument& doc, QDomNode& node);

	void adaptToTerrain(mylib::Terrain* terrain);
};

typedef boost::shared_ptr<RoadArea> RoadAreaPtr;