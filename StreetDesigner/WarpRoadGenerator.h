#pragma once

#include <common/Polygon2D.h>
#include "RoadGraph.h"
#include "RoadAreaSet.h"
#include "ExFeature.h"
#include "MainWindow.h"

class WarpRoadGenerator {
private:
	MainWindow *mainWin;
	RoadGraph &roads;
	Polygon2D targetArea;
	Polyline2D hintLine;
	mylib::Terrain *terrain;
	ExFeature &feature;
	bool animation;

public:
	WarpRoadGenerator(MainWindow *mainWin, RoadGraph &roads, const Polygon2D &targetArea, const Polyline2D &hintLine, mylib::Terrain* terrain, ExFeature &feature) : mainWin(mainWin), roads(roads), targetArea(targetArea), hintLine(hintLine), terrain(terrain), feature(feature) {}
	~WarpRoadGenerator() {}

	void generateRoadNetwork(bool animation = false);

private:
	void generateAvenueSeeds(std::list<RoadVertexDesc>& seeds);
	bool addAvenueSeed(const QVector2D &pt, const QVector2D &ex_pt, int group_id, float angle, std::list<RoadVertexDesc>& seeds);
	void generateStreetSeeds(std::list<RoadVertexDesc> &seeds);
	void attemptExpansion(int roadType, RoadVertexDesc srcDesc, std::list<RoadVertexDesc> &seeds);
	void attemptExpansion2(int roadType, RoadVertexDesc srcDesc, std::list<RoadVertexDesc> &seeds);
	bool growRoadSegment(int roadType, RoadVertexDesc srcDesc, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	void synthesizeItem(int roadType, RoadVertexDesc v_desc, std::vector<RoadEdgePtr> &edges);

public:
};

