#include "../../common/global.h"
#include "../../common/Util.h"
#include "../GraphUtil.h"
#include "PMRoadGenerator.h"
#include "RoadGeneratorHelper.h"

void PMRoadGenerator::generateRoadNetwork(RoadGraph &roads, const Polygon2D &area) {
	srand(12345);

	roads.clear();

	float snapToClosestVertexFactor = G::getFloat("roadSnapFactor");
	float roadAngleTolerance = G::getFloat("roadAngleTolerance");

	// generate arterials
	int count = 0;
	std::list<RoadVertexDesc> seeds;
	generateInitialArterialSeeds(roads, area, seeds);

	int numIterations = G::getInt("numIterations");

	while (!seeds.empty()) {// && count < numIterations) {
		std::cout << count << std::endl;

		RoadVertexDesc id = seeds.front();
		seeds.pop_front();

		if (!area.contains(roads.graph[id]->pt)) continue;

		attemptExpansion(roads, area, id, RoadEdge::TYPE_AVENUE, seeds);

		count++;
	}

	//GraphUtil::removeSelfIntersectingRoads(roads);

	return;

	// generate streets
	generateInitialStreetSeeds(roads, seeds);

	count = 0;

	while (!seeds.empty() && count < numIterations) {
		std::cout << count << std::endl;

		int id = seeds.front();
		seeds.erase(seeds.begin());

		if (!area.contains(roads.graph[id]->pt)) continue;

		attemptExpansion(roads, area, id, RoadEdge::TYPE_STREET, seeds);		

		count++;
	}

	//GraphUtil::removeSelfIntersectingRoads(roads);

	//GraphUtil::removeDeadEnd(roads);
	GraphUtil::clean(roads);
}

void PMRoadGenerator::generateInitialArterialSeeds(RoadGraph &roads, const Polygon2D &area, std::list<RoadVertexDesc>& seeds) {
	seeds.clear();

	QVector2D center = area.centroid();

	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(center));
	RoadVertexDesc desc = GraphUtil::addVertex(roads, v);

	initDirection(roads, desc, 0.0f);

	seeds.push_back(desc);
}

void PMRoadGenerator::generateInitialStreetSeeds(RoadGraph &roads, std::list<RoadVertexDesc>& seeds) {
	seeds.clear();

	int i = 0;
	int num = GraphUtil::getNumEdges(roads);
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = edges(roads.graph); ei != eend && i < num; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		i++;

		int step = roads.graph[*ei]->polyline.size() / 5;
		if (step <= 1) continue;

		RoadEdgePtr edge = roads.graph[*ei];
		RoadEdgeDesc e = *ei;
		
		while (step < edge->polyline.size() - step) {
			RoadEdgeDesc e1, e2;
			RoadVertexDesc desc = GraphUtil::splitEdge(roads, e, edge->polyline[step], e1, e2);
			
			QVector2D vec = edge->polyline[step] - edge->polyline[step - 1];
			float direction = atan2f(vec.y(), vec.x());
			initDirection(roads, desc, direction + M_PI * 0.5f, direction - M_PI * 0.5f);

			seeds.push_back(desc);

			roads.graph[*ei]->valid = false;

			edge = roads.graph[e2];
			e = e2;
		}
	}
}

/**
 * Expand a road to the specified direction.
 * If the slope is too steep or the road goes out of the domain, this method does not create
 * a road and returns false. The created road will be added to the road graph.
 * If the raod expansion is failed, NULL will be returned.
 * 
 * @param roadGraph		the road graph
 * @param id			the vertex descriptor from which a new road will be generated
 * @param direction		the direction to which a new road will be generated
 * @param roadType		the road type: ROAD_TYPE_ARTERIAL, ROAD_TYPE_STREET
 * @return edge			the generated edge as a result
 */
void PMRoadGenerator::attemptExpansion(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc id, int roadType, std::list<RoadVertexDesc> &seeds) {
	float organicFactor = G::getFloat("roadOrganicFactor");
	float angleTolerance = G::getFloat("roadAngleTolerance");
	float snapFactor = G::getFloat("roadSnapFactor");

	for (int i = 0; i < roads.graph[id]->angles.size(); ++i) {
		growRoadSegment(roads, area, id, roadType, roads.graph[id]->angles[i], organicFactor, snapFactor, angleTolerance, seeds);
	}
}

bool PMRoadGenerator::growRoadSegment(RoadGraph &roads, const Polygon2D &area, RoadVertexDesc &srcDesc, int roadType, float direction, float organicFactor, float snapFactor, float angleTolerance, std::list<RoadVertexDesc> &seeds) {
	float step = 10.0f;

	float minIntersectionInterval;
	float meanRoadGrowingQuantum;
	if (roadType == RoadEdge::TYPE_STREET) {
		minIntersectionInterval = 60.0f;
		meanRoadGrowingQuantum = 40.0f;
	} else {
		minIntersectionInterval = 300.0f;
		meanRoadGrowingQuantum = 200.0f;
	}

	// create an edge
	RoadEdgePtr edge = RoadEdgePtr(new RoadEdge(roadType, 1));
	edge->polyline.push_back(roads.graph[srcDesc]->pt);

	QVector2D cur = roads.graph[srcDesc]->pt;
	for (int i = 0; i < 10000 && edge->polyline.length() < minIntersectionInterval; ++i) {
		// Advance the current point to the next position
		cur.setX(cur.x() + cos(direction) * step);
		cur.setY(cur.y() + sin(direction) * step);

		edge->polyline.push_back(cur);

		// Update the direction
		float deltaDir = 0.9 * deltaDir + 0.1 * Util::genRand(-1.0, 1.0);
		float newDir = direction + organicFactor * 8.33333f * deltaDir;
		direction = 0.9 * direction + 0.1 * newDir;
	}

	if (edge->polyline.size() <= 3 || GraphUtil::hasRedundantEdge(roads, srcDesc, edge->polyline, angleTolerance)) {
		return false;
	}

	RoadVertexDesc tgtDesc;
	if (GraphUtil::getVertex(roads, edge->polyline.last(), edge->polyline.length() * snapFactor, srcDesc, tgtDesc)) {
		GraphUtil::movePolyline(roads, edge->polyline, roads.graph[srcDesc]->pt, roads.graph[tgtDesc]->pt);
		std::reverse(edge->polyline.begin(), edge->polyline.end());
		if (GraphUtil::hasRedundantEdge(roads, tgtDesc, edge->polyline, angleTolerance)) return false;
	} else {
		RoadVertexPtr v2 = RoadVertexPtr(new RoadVertex(edge->polyline.last()));
		tgtDesc = boost::add_vertex(roads.graph);
		roads.graph[tgtDesc] = v2;
		initDirection(roads, tgtDesc, direction);
				
		seeds.push_back(tgtDesc);
	}

	GraphUtil::addEdge(roads, srcDesc, tgtDesc, edge);

	return true;
}

/*bool PMRoadGenerator::removeIntersectingEdges(RoadGraph &roads) {
	std::vector<RoadEdgeIter> edgesToRemove;

	QVector2D a0, a1, b0, b1;
	QVector2D intPt;
	RoadEdgeIter a_ei, a_ei_end;
	RoadEdgeIter b_ei, b_ei_end;
	float ta0a1, tb0b1;

	for (boost::tie(a_ei, a_ei_end) = boost::edges(roads.graph); a_ei != a_ei_end; ++a_ei) {
		if (!roads.graph[*a_ei]->valid) continue;

		a0 = roads.graph[boost::source(*a_ei, roads.graph)]->pt;
		a1 = roads.graph[boost::target(*a_ei, roads.graph)]->pt;

		for (b_ei = a_ei; b_ei != a_ei_end; ++b_ei) {
			if (!roads.graph[*b_ei]->valid) continue;
			if (b_ei == a_ei) continue;

			b0 = roads.graph[boost::source(*b_ei, roads.graph)]->pt;
			b1 = roads.graph[boost::target(*b_ei, roads.graph)]->pt;

			if (Util::segmentSegmentIntersectXY(a0, a1, b0, b1, &ta0a1, &tb0b1, true, intPt) ){
				bool addEd=true;
				for (int eN = 0;eN < edgesToRemove.size(); eN++) {
					if (edgesToRemove[eN] == b_ei) {
						addEd = false;
						break;
					}
				}
				if(addEd)
					roads.graph[*b_ei]->valid = false;
			}
		}		
	}
	
	if(edgesToRemove.size()>0){
		return true;
	} else {
		return false;
	}
}
*/

void PMRoadGenerator::initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction) {
	float deltaDir;
	float tmpDir;
	
	roads.graph[desc]->angles.clear();
	RoadGeneratorHelper::createFourDirection(direction, roads.graph[desc]->angles);
}

void PMRoadGenerator::initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction1, float direction2) {
	float deltaDir;
	float tmpDir;
	
	roads.graph[desc]->angles.clear();

	roads.graph[desc]->angles.push_back(direction1);
	roads.graph[desc]->angles.push_back(direction2);
}

