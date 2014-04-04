#pragma once

#include <stdio.h>
#include <QVector2D>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/shared_ptr.hpp>
#include "RoadVertex.h"
#include "RoadEdge.h"
#include "../render/Renderable.h"
#include "../render/GeometryObject.h"
#include "../render/RenderableQuadList.h"
#include "../render/Terrain.h"

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, RoadVertexPtr, RoadEdgePtr> BGLGraph;
typedef graph_traits<BGLGraph>::vertex_descriptor RoadVertexDesc;
typedef graph_traits<BGLGraph>::edge_descriptor RoadEdgeDesc;
typedef graph_traits<BGLGraph>::vertex_iterator RoadVertexIter;
typedef graph_traits<BGLGraph>::edge_iterator RoadEdgeIter;
typedef graph_traits<BGLGraph>::out_edge_iterator RoadOutEdgeIter;
typedef graph_traits<BGLGraph>::in_edge_iterator RoadInEdgeIter;

class Terrain;

class RoadGraph : public mylib::GeometryObject {
public:
	BGLGraph graph;

	// for rendering (These variables should be updated via setZ() function only!!
	float highwayHeight;
	float avenueHeight;
	float widthBase;
	float curbRatio;

	QColor colorHighway;
	QColor colorBoulevard;
	QColor colorAvenue;
	QColor colorStreet;
	bool showHighways;
	bool showBoulevards;
	bool showAvenues;
	bool showLocalStreets;

public:
	RoadGraph();
	~RoadGraph();

	//void generateMesh();
	//void addMeshFromEdge(RenderablePtr renderable, RoadEdgePtr edge, float widthBase, QColor color, float height);
	//void addMeshFromVertex(RenderablePtr renderable, RoadVertexPtr vertex, QColor color, float height);

	void clear();
	void setZ(float z);
	void adaptToTerrain(mylib::Terrain* terrain);
	//void add3DMeshOfEdge(mylib::RenderableQuadList* renderable, RoadEdgePtr edge, float width, QColor color, float heightOffset = 0.0f);

	void generate2DMesh();
	void add2DMeshOfEdge(mylib::RenderablePtr renderable, RoadEdgePtr edge, float widthBase, QColor color, float height);
	void add2DMeshOfVertex(mylib::RenderablePtr renderable, RoadVertexPtr vertex, QColor color, float height);

private:
	void _generateMeshVertices(mylib::TextureManager* textureManager);
};

typedef boost::shared_ptr<RoadGraph> RoadGraphPtr;
