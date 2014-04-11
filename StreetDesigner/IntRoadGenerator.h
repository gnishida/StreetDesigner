#pragma once

#include <common/Polygon2D.h>
#include <road/RoadGraph.h>
#include <road/RoadAreaSet.h>
#include <road/feature/ExFeature.h>
#include "MainWindow.h"

class IntRoadGenerator {
private:
	MainWindow *mainWin;
	bool animation;

public:
	IntRoadGenerator(MainWindow *mainWin, bool animation) : mainWin(mainWin), animation(animation) {}
	~IntRoadGenerator() {}

	void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, mylib::Terrain* terrain, ExFeature& feature);

private:
	void generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& f, std::list<RoadVertexDesc>& seeds);
	bool addAvenueSeed(RoadGraph &roads, const const Polygon2D &area, ExFeature &f, const QVector2D &pt, const QVector2D &ex_pt, int group_id, std::list<RoadVertexDesc>& seeds);
	void generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, ExFeature& kf, std::list<RoadVertexDesc> &seeds);
	void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	void attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	RoadVertexDesc getItemByLocation(RoadGraph &roads, const Polygon2D &area, ExFeature &f, int roadType, const QVector2D &pt);
	RoadVertexDesc getItemByEdgeShape(RoadGraph &roads, ExFeature &f, int roadType, RoadVertexDesc tgtDesc, const Polyline2D &polyline);
	void synthesizeItem(RoadGraph &roads, ExFeature &f, RoadVertexDesc v_desc, int roadType, std::vector<RoadEdgePtr> &edges);

public:
};

