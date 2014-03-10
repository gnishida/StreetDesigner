#pragma once

#include <boost/shared_ptr.hpp>
#include "../common/Polygon2D.h"
#include "../common/Polyline3D.h"
#include "RoadGraph.h"
#include "../../UrbanSimulator/MyTerrain.h"

class RoadArea {
public:
	Polygon2D area;
	Polyline3D area3D;
	RoadGraph roads;

public:
	RoadArea();
	RoadArea(const Polygon2D &area);
	~RoadArea();

	void load(QDomNode& node);
	void save(QDomDocument& doc, QDomNode& node);

	void adaptToTerrain(MyTerrain* terrain);
};

typedef boost::shared_ptr<RoadArea> RoadAreaPtr;