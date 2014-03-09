/*********************************************************************
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

#include "MyBlockGenerator.h"
#include "../Core/Block.h"
#include <road/RoadGraph.h>
#include "MyUrbanGeometry.h"
#include "../Core/Util.h"
#include "MainWindow.h"
#include "MyUrbanGeometry.h"
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <limits>
#include <QtTest/qtest.h>


RoadGraph* roadGraphPtr;
std::vector<ucore::Block*>* blocksPtr;
ucore::Loop3D blockContourTmp;
std::vector<float> blockContourWidths;

//Vertex visitor
struct vertex_output_visitor : public boost::planar_face_traversal_visitor {//output_visitor {
	void begin_face() {
		blockContourTmp.clear();
		blockContourWidths.clear();
	}

	void end_face() {
		if(blockContourTmp.size() > 2){
			ucore::Block* newBlock = new ucore::Block();
			newBlock->setContour(blockContourTmp);		
			newBlock->setRoadWidths(blockContourWidths);
			
			blocksPtr->push_back(newBlock);
		}
	}

	template <typename Vertex> 
	void next_vertex(Vertex v) {
		blockContourTmp.push_back(roadGraphPtr->graph[v]->pt);
	}

	template <typename Edge> 
	void next_edge(Edge e) {
		blockContourWidths.push_back(0.5f * roadGraphPtr->graph[e]->getWidth());
	}
};

MyBlockGenerator::MyBlockGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry) {
	this->mainWin = mainWin;
	this->urbanGeometry = urbanGeometry;
}

MyBlockGenerator::~MyBlockGenerator() {
}

/**
* Given a road network, this function extracts the blocks
**/
void MyBlockGenerator::run() {
	RoadGraph* roads = &urbanGeometry->roads;
	std::vector<ucore::Block*>* blocks = &urbanGeometry->getBlocks();

	// clean up memory allocated for blocks
	for (int i = 0; i < blocks->size(); ++i) {
		delete blocks->at(i);
	}
	blocks->clear();

	roadGraphPtr = roads;
	blocksPtr = blocks;
	blocksPtr->clear();

	bool isPlanar = false;
	bool converges = true;

	//Make sure graph is planar
	typedef std::vector<RoadEdgeDesc > tEdgeDescriptorVector;
	std::vector<tEdgeDescriptorVector> embedding(boost::num_vertices(roads->graph));

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
			/*if (!removeIntersectingEdges(roads)) {
				converges = false;
			}*/
		}
		cont++;
	}

	if (!isPlanar) {
		std::cout << "ERROR: Graph could not be planarized (generateBlocks)\n";
	}

	mainWin->glWidget->updateGL();
	QTest::qWait(1);

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

	mainWin->glWidget->updateGL();
	QTest::qWait(1);

	std::vector<float> blockAreas;

	ucore::Loop3D blockContourInset;
	for (int i = 0; i < blocks->size(); ++i) {
		//Reorient faces
		blocks->at(i)->getContour().reorientFace();

		if(blocks->at(i)->getContour().getContour().size() < 3){
			blockAreas.push_back(0.0f);
			continue;
		}

		float insetArea = blocks->at(i)->getContour().computeInset(blocks->at(i)->getRoadWidths(), blockContourInset);		
		blocks->at(i)->setContour(blockContourInset);

		blockAreas.push_back(insetArea);
	}

	//Remove the largest block
	float maxArea = (std::numeric_limits<float>::min)();
	int maxAreaIdx = -1;
	for (int i = 0; i < blocks->size(); ++i) {
		if (blocks->at(i)->getContour().getContour().size() < 3) {
			continue;
		}
		if (blockAreas[i] > maxArea) {
			maxArea = blockAreas[i];
			maxAreaIdx = i;
		}
	}

	if (maxAreaIdx != -1) {
		blocks->erase(blocks->begin() + maxAreaIdx);
		blockAreas.erase(blockAreas.begin() + maxAreaIdx);
	}

	mainWin->glWidget->updateGL();
	QTest::qWait(1);
}

/**
* Remove intersecting edges of a graph
**/
bool MyBlockGenerator::removeIntersectingEdges(RoadGraph* roadGraph) {
	std::vector<RoadEdgeIter> edgesToRemove;

	QVector2D a0, a1, b0, b1;
	QVector2D intPt;
	RoadEdgeIter a_ei, a_ei_end;
	RoadEdgeIter b_ei, b_ei_end;
	float ta0a1, tb0b1;

	for (boost::tie(a_ei, a_ei_end) = boost::edges(roadGraph->graph); a_ei != a_ei_end; ++a_ei) {
		a0 = QVector2D(roadGraph->graph[boost::source(*a_ei, roadGraph->graph)]->pt);
		a1 = QVector2D(roadGraph->graph[boost::target(*a_ei, roadGraph->graph)]->pt);

		for (b_ei = a_ei; b_ei != a_ei_end; ++b_ei) {
			if (b_ei == a_ei) continue;

			b0 = QVector2D(roadGraph->graph[boost::source(*b_ei, roadGraph->graph)]->pt);
			b1 = QVector2D(roadGraph->graph[boost::target(*b_ei, roadGraph->graph)]->pt);

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
		boost::remove_edge(*(edgesToRemove[i]), roadGraph->graph);
	}

	if(edgesToRemove.size()>0){
		return true;
	} else {
		return false;
	}
}

