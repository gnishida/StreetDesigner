﻿/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <limits>
#include <QtTest/qtest.h>
#include <common/Util.h>
#include <common/Polygon3D.h>
#include <pm/Block.h>
#include <road/RoadGraph.h>
#include "BlockGenerator.h"
#include "MainWindow.h"
#include "UrbanGeometry.h"

RoadGraph* roadGraphPtr;
std::vector<pm::Block*>* blocksPtr;
Polygon3D blockContourTmp;
Polygon3D blockContourPoints;
std::vector<Polyline3D> blockContourLines;
std::vector<float> blockContourWidths;

//Vertex visitor
struct vertex_output_visitor : public boost::planar_face_traversal_visitor {//output_visitor {
	void begin_face() {
		blockContourTmp.clear();
		blockContourWidths.clear();

		blockContourPoints.clear();
		blockContourLines.clear();
	}

	void end_face() {
		if (blockContourPoints.size() > 2) {
			blockContourTmp.clear();
			
			for (int i = 0; i < blockContourPoints.size(); ++i) {
				blockContourTmp.push_back(blockContourPoints[i]);

				if ((blockContourLines[i][0] - blockContourPoints[i]).lengthSquared() < (blockContourLines[i].last() - blockContourPoints[i]).lengthSquared()) {
					for (int j = 1; j < blockContourLines[i].size() - 1; ++j) {
						blockContourTmp.push_back(blockContourLines[i][j]);
					}
				} else {
					for (int j = blockContourLines[i].size() - 2; j > 0; --j) {
						blockContourTmp.push_back(blockContourLines[i][j]);
					}
				}
			}

			pm::Block* newBlock = new pm::Block();
			newBlock->setContour(blockContourTmp);
			newBlock->setRoadWidths(blockContourWidths);
			
			blocksPtr->push_back(newBlock);
		}
	}

	template <typename Vertex> 
	void next_vertex(Vertex v) {
		blockContourPoints.push_back(roadGraphPtr->graph[v]->pt);
	}

	template <typename Edge> 
	void next_edge(Edge e) {
		blockContourLines.push_back(roadGraphPtr->graph[e]->polyline3D);

		for (int i = 0; i < roadGraphPtr->graph[e]->polyline3D.size() - 1; ++i) {
			blockContourWidths.push_back(0.5f * roadGraphPtr->graph[e]->getWidth());
		}
	}
};

BlockGenerator::BlockGenerator(MainWindow* mainWin) {
	this->mainWin = mainWin;
}

BlockGenerator::~BlockGenerator() {
}

/**
* Given a road network, this function extracts the blocks
**/
void BlockGenerator::run() {
	RoadGraph* roads = &mainWin->urbanGeometry->roads;
	std::vector<pm::Block*>* blocks = &mainWin->urbanGeometry->blocks;

	// clean up memory allocated for blocks
	for (int i = 0; i < blocks->size(); ++i) {
		delete blocks->at(i);
	}
	blocks->clear();

	roadGraphPtr = roads;
	blocksPtr = blocks;

	bool isPlanar = false;
	bool converges = true;

	//Make sure graph is planar
	typedef std::vector<RoadEdgeDesc > tEdgeDescriptorVector;
	std::vector<tEdgeDescriptorVector> embedding(boost::num_vertices(roads->graph));

	// Planarグラフにする
	int cont=0;
	while (!isPlanar && converges) {	
		if (cont>2) {
			return;
		}
		// Test for planarity		
		if (boost::boyer_myrvold_planarity_test(boost::boyer_myrvold_params::graph = roads->graph, boost::boyer_myrvold_params::embedding = &embedding[0])) {
			isPlanar = true;
		} else {			
			//Remove intersecting edges
			if (!removeIntersectingEdges(roads)) {
				converges = false;
			}
		}
		cont++;
	}

	if (!isPlanar) {
		std::cout << "ERROR: Graph could not be planarized (generateBlocks)\n";
	}

	//mainWin->glWidget->updateGL();
	//QTest::qWait(1);

	//Create edge index property map?	
	typedef std::map<RoadEdgeDesc, size_t> EdgeIndexMap;
	EdgeIndexMap mapEdgeIdx;
	boost::associative_property_map<EdgeIndexMap> pmEdgeIndex(mapEdgeIdx);		
	RoadEdgeIter ei, ei_end;	
	int edge_count = 0;
	for (boost::tie(ei, ei_end) = boost::edges(roads->graph); ei != ei_end; ++ei) {
		mapEdgeIdx.insert(std::make_pair(*ei, edge_count++));	
	}

	//Extract blocks from road graph using boost graph planar_face_traversal
	vertex_output_visitor v_vis;
	boost::planar_face_traversal(roads->graph, &embedding[0], v_vis, pmEdgeIndex);

	//mainWin->glWidget->updateGL();
	//QTest::qWait(1);

	std::vector<float> blockAreas;

	Polygon3D blockContourInset;
	for (int i = 0; i < blocks->size(); ++i) {
		//Reorient faces
		//blocks->at(i)->getContour().reorientFace();
		//blocks->at(i)->getContour().correct();
		//std::reverse(blocks->at(i)->getContour().begin(), blocks->at(i)->getContour().end());

		if(blocks->at(i)->getContour().size() < 3){
			blockAreas.push_back(0.0f);
			continue;
		}

		//float insetArea = blocks->at(i)->getContour().computeInset(blocks->at(i)->getRoadWidths(), blockContourInset);		
		//blocks->at(i)->setContour(blockContourInset);

		//blockAreas.push_back(insetArea);
	}

	//Remove the largest block
	/*
	float maxArea = 0.0f;
	int maxAreaIdx = -1;
	for (int i = 0; i < blocks->size(); ++i) {
		if (blocks->at(i)->getContour().size() < 3) {
			continue;
		}
		float area = blocks->at(i)->getContour().area();
		if (area > maxArea) {
			maxArea = area;
			maxAreaIdx = i;
		}
	}

	if (maxAreaIdx != -1) {
		blocks->erase(blocks->begin() + maxAreaIdx);
		//blockAreas.erase(blockAreas.begin() + maxAreaIdx);
	}*/
	blocks->erase(blocks->begin());

	//mainWin->glWidget->updateGL();
	//QTest::qWait(1);

	printf("generation done.\n");
}

/**
* Remove intersecting edges of a graph
**/
bool BlockGenerator::removeIntersectingEdges(RoadGraph* roadGraph) {
	/*
	std::vector<RoadGraphEdgeIter> edgesToRemove;

	QVector2D a0, a1, b0, b1;
	QVector2D intPt;
	RoadGraphEdgeIter a_ei, a_ei_end;
	RoadGraphEdgeIter b_ei, b_ei_end;
	float ta0a1, tb0b1;

	for (boost::tie(a_ei, a_ei_end) = boost::edges(roadGraph->getRoadGraph()); a_ei != a_ei_end; ++a_ei) {
		a0 = QVector2D(roadGraph->getSourceVertex(*a_ei)->getPt());
		a1 = QVector2D(roadGraph->getTargetVertex(*a_ei)->getPt());

		for (b_ei = a_ei; b_ei != a_ei_end; ++b_ei) {
			if (b_ei == a_ei) continue;

			b0 = QVector2D(roadGraph->getSourceVertex(*b_ei)->getPt());
			b1 = QVector2D(roadGraph->getTargetVertex(*b_ei)->getPt());

			if (ucore::Util::segmentSegmentIntersectXY(a0, a1, b0, b1, &ta0a1, &tb0b1, true, intPt) ){
				bool addEd=true;
				for (int eN = 0;eN < edgesToRemove.size(); eN++) {
					if (edgesToRemove[eN] == b_ei) {
						addEd = false;
						break;
					}
				}
				if(addEd)
					edgesToRemove.push_back(b_ei);
			}
		}		
	}
	
	for(int i=0; i<edgesToRemove.size(); ++i) {	
		boost::remove_edge(*(edgesToRemove[i]), roadGraph->getRoadGraph());
	}

	if(edgesToRemove.size()>0){
		return true;
	} else {
		return false;
	}
	*/

	return true;
}

