#pragma once

#include "../../common/Polygon2D.h"
#include "../RoadGraph.h"
#include "../RoadAreaSet.h"
#include "../feature/ExFeature.h"

class IntRoadGenerator {
private:
	MainWindow *mainWin;

public:
	IntRoadGenerator(MainWindow *mainWin) : mainWin(mainWin) {}
	~IntRoadGenerator() {}

	static void generateRoadNetwork(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, mylib::Terrain* terrain, ExFeature& feature);

private:
	static void generateAvenueSeeds(RoadGraph &roads, const Polygon2D &area, const Polyline2D &hintLine, ExFeature& f, std::list<RoadVertexDesc>& seeds);
	static bool addAvenueSeed(RoadGraph &roads, const const Polygon2D &area, ExFeature &f, const QVector2D &pt, const QVector2D &ex_pt, int group_id, std::list<RoadVertexDesc>& seeds);
	static void generateStreetSeeds(RoadGraph &roads, const Polygon2D &area, ExFeature& kf, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	static void attemptExpansion2(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, std::list<RoadVertexDesc> &seeds);
	static bool growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, mylib::Terrain* terrain, ExFeature& f, const Polyline2D &polyline, int lanes, RoadVertexDesc next_ex_v_desc, bool byExample, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds);

	static RoadVertexDesc getItemByLocation(RoadGraph &roads, const Polygon2D &area, ExFeature &f, int roadType, const QVector2D &pt);
	static RoadVertexDesc getItemByEdgeShape(RoadGraph &roads, ExFeature &f, int roadType, RoadVertexDesc tgtDesc, const Polyline2D &polyline);
	static void synthesizeItem(RoadGraph &roads, ExFeature &f, RoadVertexDesc v_desc, int roadType, std::vector<RoadEdgePtr> &edges);

public:
};

