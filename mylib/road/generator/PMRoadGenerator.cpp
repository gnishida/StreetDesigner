#include "../../common/Util.h"
#include "../GraphUtil.h"
#include "PMRoadGenerator.h"

void PMRoadGenerator::generateRoadNetwork(RoadGraph &roads, const Polygon2D &area) {
	roads.clear();

	float snapToClosestVertexFactor = 0.7f;
	float roadAngleTolerance = 1.2566f;

	// generate arterials
	int count = 0;
	std::vector<RoadVertexDesc> seeds;
	generateInitialArterialSeeds(roads, area, seeds);

	while (!seeds.empty() && count < 100000) {
		std::cout << count << std::endl;

		RoadVertexDesc id = seeds.front();
		seeds.erase(seeds.begin());

		if (!area.contains(roads.graph[id]->pt)) continue;

		for (int i = 0; i < roads.graph[id]->angles.size(); ++i) {
			float direction = roads.graph[id]->angles[i];
			RoadEdgePtr edge = attemptExpansion(roads, id, direction, RoadEdge::TYPE_AVENUE);

			if (edge->polyline.size() <= 3 || isRedundantEdge(roads, id, edge->polyline, roadAngleTolerance)) {
				continue;
			}

			RoadVertexDesc id2;
			if (GraphUtil::getVertex(roads, edge->polyline.last(), edge->polyline.length() * snapToClosestVertexFactor, id, id2)) {
				GraphUtil::movePolyline(roads, edge->polyline, roads.graph[id]->pt, roads.graph[id2]->pt);
				std::reverse(edge->polyline.begin(), edge->polyline.end());
				if (isRedundantEdge(roads, id2, edge->polyline, roadAngleTolerance)) continue;
			} else {
				RoadVertexPtr v2 = RoadVertexPtr(new RoadVertex(edge->polyline.last()));
				id2 = boost::add_vertex(roads.graph);
				roads.graph[id2] = v2;
				initDirection(roads, id2, direction);
				
				seeds.push_back(id2);
			}

			/*
			if (roads.findNearestVertex(edge->getPolyLine()[num - 1], id, id2) < edge->getLength() * snapToClosestVertexFactor) {
				edge->snapTo(roads.graph[id2]->getPt(), urbanGeometry);
				if (roads.isRedundant(id2, edge, roadAngleTolerance)) {
					delete edge;
					continue;
				}
			} else {
				MyRoadGraphVertex* vertex2 = new MyRoadGraphVertex(edge->getPolyLine()[num - 1]);
				vertex2->initDirection(direction, (direction > M_PI) ? direction - M_PI : direction + M_PI);
				id2 = roadGraph->addVertex(vertex2);
				seeds.push_back(id2);
				roadGraph->getSeeds().push_back(id2);
			}*/

			GraphUtil::addEdge(roads, id, id2, edge);
		}

		count++;
	}

	//GraphUtil::removeDeadEnd(roads);
	removeSelfIntersectingRoads(roads);

	// generate streets
	generateInitialStreetSeeds(roads, seeds);

	count = 0;

	while (!seeds.empty()) {
		std::cout << count << std::endl;

		int id = seeds.front();
		seeds.erase(seeds.begin());

		if (!area.contains(roads.graph[id]->pt)) continue;
		
		for (int i = 0; i < roads.graph[id]->angles.size(); ++i) {
			float direction = roads.graph[id]->angles[i];
			RoadEdgePtr edge;
			if ((edge = attemptExpansion(roads, id, direction, RoadEdge::TYPE_STREET)) == NULL) {
				continue;
			}

			if (edge->polyline.size() <= 3 || isRedundantEdge(roads, id, edge->polyline, roadAngleTolerance)) {
				continue;
			}

			RoadVertexDesc id2;
			if (GraphUtil::getVertex(roads, edge->polyline.last(), edge->polyline.length() * snapToClosestVertexFactor, id, id2)) {
				GraphUtil::movePolyline(roads, edge->polyline, roads.graph[id]->pt, roads.graph[id2]->pt);
				std::reverse(edge->polyline.begin(), edge->polyline.end());
				if (isRedundantEdge(roads, id2, edge->polyline, roadAngleTolerance)) continue;
			} else {
				RoadVertexPtr v2 = RoadVertexPtr(new RoadVertex(edge->polyline.last()));
				id2 = boost::add_vertex(roads.graph);
				roads.graph[id2] = v2;
				initDirection(roads, id2, direction);
				
				seeds.push_back(id2);
			}

			GraphUtil::addEdge(roads, id, id2, edge);
		}

		count++;
	}

	//GraphUtil::removeDeadEnd(roads);
	removeSelfIntersectingRoads(roads);
}

void PMRoadGenerator::generateInitialArterialSeeds(RoadGraph &roads, const Polygon2D &area, std::vector<RoadVertexDesc>& seeds) {
	seeds.clear();

	QVector2D center = area.centroid();

	RoadVertexPtr v = RoadVertexPtr(new RoadVertex(center));
	RoadVertexDesc desc = GraphUtil::addVertex(roads, v);

	initDirection(roads, desc, 0.0f);

	seeds.push_back(desc);
}

void PMRoadGenerator::generateInitialStreetSeeds(RoadGraph &roads, std::vector<RoadVertexDesc>& seeds) {
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
RoadEdgePtr PMRoadGenerator::attemptExpansion(RoadGraph &roads, RoadVertexDesc id, float& direction, int roadType) {
	// get some user defined parameters
	int maxPopulationPerCell = 600;
	int maxEmploymentPerCell = 600;
	float step = 10.0f;
	float organicFactor = 0.1f;
	float roadSlopeThreshold = 100.0f;
	float snapToClosestVertexFactor = 0.7f;
	float roadWidth;
	float minIntersectionInterval;
	float meanRoadGrowingQuantum;
	if (roadType == RoadEdge::TYPE_STREET) {
		roadWidth = 5.0f;
		minIntersectionInterval = 60.0f;
		meanRoadGrowingQuantum = 40.0f;
	} else {
		roadWidth = 10.0f;
		minIntersectionInterval = 300.0f;
		meanRoadGrowingQuantum = 200.0f;
	}

	// create an edge
	RoadEdgePtr edge = RoadEdgePtr(new RoadEdge(roadType, 1));
	edge->polyline.push_back(roads.graph[id]->pt);

	// initialize some variables
	QVector2D cur = roads.graph[id]->pt;
	float deltaDir = 0.0f;
	float accumlatedPopulation = 0.0f;

	float relPopDensity;
	float localOrganicFactor;

	for (int i = 0; i < 10000 && (accumlatedPopulation < meanRoadGrowingQuantum || edge->polyline.length() < minIntersectionInterval); ++i) {
		// Advance the current point to the next position
		cur.setX(cur.x() + cos(direction) * step);
		cur.setY(cur.y() + sin(direction) * step);

		edge->polyline.push_back(cur);

		// Accumulate the population along the road.
		float population = 100.0f;
		float jobs = 100.0f;
		accumlatedPopulation += population / step;

		// Update the local organic factor based on the population density.
		if (population + jobs < 0.5 * (maxPopulationPerCell + maxEmploymentPerCell)) {
			relPopDensity = 1.0 - (population + jobs) / (maxPopulationPerCell + maxEmploymentPerCell);
			localOrganicFactor = 10.0 * organicFactor * relPopDensity;
		} else {
			localOrganicFactor = 2.0 * organicFactor;
		}

		// Update the direction
		deltaDir = 0.9 * deltaDir + 0.1 * Util::genRand(-1.0, 1.0);
		float newDir = direction + localOrganicFactor * deltaDir;
		direction = 0.9 * direction + 0.1 * newDir;
	}

	return edge;
}

void PMRoadGenerator::removeSelfIntersectingRoads(RoadGraph &roads) {
	QVector2D a1, a2, b1, b2;
	float ta, tb;
	QVector2D intPt;
	RoadEdgeIter ei, eend;
	RoadEdgeIter ei2, eend2;

	std::vector<RoadEdgeIter> toBeRemoved;
	
	for (boost::tie(ei, eend) = boost::edges(roads.graph); ei != eend; ++ei) {
		if (!roads.graph[*ei]->valid) continue;

		a1 = roads.graph[boost::source(*ei, roads.graph)]->pt;
		a2 = roads.graph[boost::target(*ei, roads.graph)]->pt;
		
		for (boost::tie(ei2, eend2) = boost::edges(roads.graph); ei2 != ei; ++ei2);
		
		for (++ei2; ei2 != eend2; ++ei2) {
			if (!roads.graph[*ei2]->valid) continue;

			b1 = roads.graph[boost::source(*ei2, roads.graph)]->pt;
			b2 = roads.graph[boost::target(*ei2, roads.graph)]->pt;

			if (Util::segmentSegmentIntersectXY(a1, a2, b1, b2, &ta, &tb, true, intPt)) {
				RoadEdgeIter r;
				if (roads.graph[*ei]->getWidth() > roads.graph[*ei2]->getWidth()) {
					r = ei2;
				} else if(roads.graph[*ei]->getWidth() < roads.graph[*ei2]->getWidth()) {
					r = ei;
					break;
				} else {
					if (roads.graph[*ei]->getLength() < roads.graph[*ei2]->getLength()) {
						r = ei2;
					} else {
						r = ei;
					}
				}

				bool duplicated = false;
				for (int i = 0; i < toBeRemoved.size(); ++i) {
					if (toBeRemoved[i] == r) {
						duplicated = true;
						break;
					}
				}
				if (!duplicated) toBeRemoved.push_back(r);
			}
		}
	}

	for (int i = 0; i < toBeRemoved.size(); ++i) {
		boost::remove_edge(*toBeRemoved[i], roads.graph);
	}
}

bool PMRoadGenerator::removeIntersectingEdges(RoadGraph &roads) {
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

/**
 * 指定された頂点に登録済みのエッジに対して、指定されたポリラインとのなす角度がthreshold未満なら、trueを返却する。
 * 指定されたポリラインは、当該頂点の座標から順番に並ぶオーダとなっていること。
 */
bool PMRoadGenerator::isRedundantEdge(RoadGraph &roads, RoadVertexDesc desc, const Polyline2D &polyline, float threshold) {
	RoadOutEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::out_edges(desc, roads.graph); ei != eend; ++ei) {
		RoadVertexDesc tgt = boost::target(*ei, roads.graph);

		if (Util::angleThreePoints(roads.graph[tgt]->pt, roads.graph[desc]->pt, polyline.last()) < threshold) return true;
	}

	return false;
}

void PMRoadGenerator::initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction) {
	float deltaDir;
	float tmpDir;
	
	roads.graph[desc]->angles.clear();

	deltaDir = 0.5f * M_PI;
	for (int i = 0; i < 4; ++i) {
		if (i==0) {
			tmpDir = direction;
		} else {
			tmpDir = tmpDir + deltaDir;// + ucore::Util::genRand(-departingAnglesNoise, departingAnglesNoise);
		}

		if (tmpDir > 2.0 * M_PI) {
			tmpDir = tmpDir - 2.0 * M_PI; 
		}

		roads.graph[desc]->angles.push_back(tmpDir);
	}
}

void PMRoadGenerator::initDirection(RoadGraph &roads, RoadVertexDesc desc, float direction1, float direction2) {
	float deltaDir;
	float tmpDir;
	
	roads.graph[desc]->angles.clear();

	roads.graph[desc]->angles.push_back(direction1);
	roads.graph[desc]->angles.push_back(direction2);
}

