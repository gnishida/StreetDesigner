#include <QGLWidget>
#include "GraphUtil.h"
#include "RoadGraph.h"
#include "../common/Util.h"
#include "../render/MyTerrain.h"
#include "../render/RenderableQuadList.h"
#include "../render/TextureManager.h"

RoadGraph::RoadGraph() {
	showHighways = true;
	showBoulevard = true;
	showAvenues = true;
	showLocalStreets = true;
}

RoadGraph::~RoadGraph() {
}

void RoadGraph::_generateMeshVertices(ucore::TextureManager* textureManager) {
	ucore::RenderableQuadList* renderable1 = new ucore::RenderableQuadList();
	ucore::RenderableQuadList* renderable2 = new ucore::RenderableQuadList(textureManager->get("data/textures/street.jpg"));

	// generate vertices for seeds
	/*
	for (int i = 0; i < seeds.size(); ++i) {
		RoadVertexPtr v = graph[seeds[i]];
		QVector3D p0 = v->getPt() + QVector3D(-5, -5, 0);
		QVector3D p1 = v->getPt() + QVector3D(5, -5, 0);
		QVector3D p2 = v->getPt() + QVector3D(5, 5, 0);
		QVector3D p3 = v->getPt() + QVector3D(-5, 5, 0);
		QVector3D normal = ucore::Util::calculateNormal(p0, p1, p2);

		renderable1->addQuad(p0, p1, p2, p3, normal, QColor(255, 255, 255));
	}
	*/

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		int num = graph[*ei]->polyline3D.size();
		if (num <= 1) continue;

		for (int i = 0; i < num - 1; ++i) {
			QVector3D pt1 = graph[*ei]->polyline3D[i];
			QVector3D pt2 = graph[*ei]->polyline3D[i + 1];
			QVector3D vec = pt2 - pt1;
			vec = QVector3D(-vec.y(), vec.x(), 0.0f);
			vec.normalize();

			QVector3D p0 = pt1 + vec * graph[*ei]->getWidth() / 2.0f;
			QVector3D p1 = pt1 - vec * graph[*ei]->getWidth() / 2.0f;
			QVector3D p2 = pt2 - vec * graph[*ei]->getWidth() / 2.0f;
			QVector3D p3 = pt2 + vec * graph[*ei]->getWidth() / 2.0f;
			QVector3D normal = ucore::Util::calculateNormal(p0, p1, p2);

			if (i < num - 2) {
				QVector3D vec2 = graph[*ei]->polyline3D[i + 2] - pt2;
				vec2 = QVector3D(-vec2.y(), vec2.x(), 0.0f);
				vec2.normalize();

				QVector3D p4 = pt2 - vec2 * graph[*ei]->getWidth() / 2.0f;
				QVector3D p5 = pt2 + vec2 * graph[*ei]->getWidth() / 2.0f;

				p2 = (p2 + p4) / 2;
				p3 = (p3 + p5) / 2;
			}

			//renderable2->addQuad(p0, p1, p2, p3, normal, color);
			renderable2->addQuad(p0, p1, p2, p3, normal, 0, 1, 0, 1);
		}
	}

	renderables.push_back(renderable1);
	renderables.push_back(renderable2);
}

/*void RoadGraph::generateMesh() {
	if (!modified) return;

	renderables.clear();

	renderables.push_back(RenderablePtr(new Renderable(GL_TRIANGLES)));
	renderables.push_back(RenderablePtr(new Renderable(GL_POINTS, 10.0f)));

	// road edge
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		if (!graph[*ei]->valid) continue;

		RoadEdgePtr edge = graph[*ei];

		float height;
		switch (edge->type) {
		case RoadEdge::TYPE_HIGHWAY:
			height = highwayHeight;
			break;
		case RoadEdge::TYPE_BOULEVARD:
			height = avenueHeight;
			break;
		case RoadEdge::TYPE_AVENUE:
			height = avenueHeight;
			break;
		case RoadEdge::TYPE_STREET:
			height = avenueHeight;
			break;
		}

		QColor color = graph[*ei]->color;
		QColor bgColor = graph[*ei]->bgColor;

		// draw the border of the road segment
		if ((showHighways && edge->type == RoadEdge::TYPE_HIGHWAY) || (showBoulevard && edge->type ==  RoadEdge::TYPE_BOULEVARD) || (showAvenues && edge->type ==  RoadEdge::TYPE_AVENUE) || (showLocalStreets && edge->type ==  RoadEdge::TYPE_STREET)) {
			addMeshFromEdge(renderables[0], edge, widthBase * (1.0f + curbRatio), bgColor, height * 0.5f);
			addMeshFromEdge(renderables[0], edge, widthBase, color, height);
		}
	}

	// road vertex
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi) {
		if (!graph[*vi]->valid) continue;

		if (graph[*vi]->seed) {
			addMeshFromVertex(renderables[1], graph[*vi], QColor(0, 0, 255), 2.0f);
		}
	}

	modified = false;
}
*/

/**
 * Add a mesh for the specified edge.
 */
 /*
void RoadGraph::addMeshFromEdge(RenderablePtr renderable, RoadEdgePtr edge, float widthBase, QColor color, float height) {
	Vertex v;

	// define the width of the road segment
	float width;
	switch (edge->type) {
	case RoadEdge::TYPE_HIGHWAY:
		width = widthBase * 2.0f;
		break;
	case RoadEdge::TYPE_BOULEVARD:
	case RoadEdge::TYPE_AVENUE:
		width = widthBase * 1.5f;
		break;
	case RoadEdge::TYPE_STREET:
		width = widthBase * 1.0f;
		break;
	}

	int num = edge->polyLine.size();

	// draw the edge
	for (int i = 0; i < num - 1; ++i) {
		QVector2D pt1 = edge->polyLine[i];
		QVector2D pt2 = edge->polyLine[i + 1];
		QVector2D vec = pt2 - pt1;
		vec = QVector2D(-vec.y(), vec.x());
		vec.normalize();

		QVector2D p0 = pt1 + vec * width * 0.5f;
		QVector2D p1 = pt1 - vec * width * 0.5f;
		QVector2D p2 = pt2 - vec * width * 0.5f;
		QVector2D p3 = pt2 + vec * width * 0.5f;

		v.color[0] = color.redF();
		v.color[1] = color.greenF();
		v.color[2] = color.blueF();
		v.color[3] = color.alphaF();
		v.normal[0] = 0.0f;
		v.normal[1] = 0.0f;
		v.normal[2] = 1.0f;

		v.location[2] = height;

		v.location[0] = p0.x();
		v.location[1] = p0.y();
		renderable->vertices.push_back(v);

		v.location[0] = p1.x();
		v.location[1] = p1.y();
		renderable->vertices.push_back(v);

		v.location[0] = p2.x();
		v.location[1] = p2.y();
		renderable->vertices.push_back(v);

		v.location[0] = p0.x();
		v.location[1] = p0.y();
		renderable->vertices.push_back(v);

		v.location[0] = p2.x();
		v.location[1] = p2.y();
		renderable->vertices.push_back(v);

		v.location[0] = p3.x();
		v.location[1] = p3.y();
		renderable->vertices.push_back(v);
	}
}
*/
/**
 * Add a mesh for the specified edge.
 */
/*
void RoadGraph::addMeshFromVertex(RenderablePtr renderable, RoadVertexPtr vertex, QColor color, float height) {
	Vertex v;

	// draw the vertex
	v.color[0] = color.redF();
	v.color[1] = color.greenF();
	v.color[2] = color.blueF();
	v.color[3] = color.alphaF();
	v.normal[0] = 0.0f;
	v.normal[1] = 0.0f;
	v.normal[2] = 1.0f;

	v.location[0] = vertex->pt.x();
	v.location[1] = vertex->pt.y();
	v.location[2] = height;

	renderable->vertices.push_back(v);
}
*/

void RoadGraph::clear() {
	graph.clear();

	setModified();
}

void RoadGraph::setZ(float z) {
	// define the width per lane
	float widthBase2;
	if (z < 450.0f) {
		widthBase2 = 2.0f;
	} else if (z < 900.0f) {
		widthBase2 = 4.0f;
	} else if (z < 1620.0f) {
		widthBase2 = 10.0f;
	} else if (z < 5760.0f) {
		widthBase2 = 12.0f;
	} else {
		widthBase2 = 24.0f;
	}
	if (widthBase != widthBase2) {
		widthBase = widthBase2;
		setModified();
	}

	// define the curb ratio
	float curbRatio2;
	if (z < 2880.0f) {
		curbRatio2 = 0.4f;
	} else {
		curbRatio2 = 0.8f;
	}
	if (curbRatio != curbRatio2) {
		curbRatio = curbRatio2;
		setModified();
	}

	// define the height
	float highwayHeight2 = (float)((int)(z * 0.012f)) * 0.1f;
	if (highwayHeight != highwayHeight2) {
		highwayHeight = highwayHeight2;
		avenueHeight = highwayHeight2 * 0.66f;
		setModified();
	}
}

/**
 * 道路網のGeometryを更新した場合は、必ずこの関数を実行して、3D Geometryを更新すること。
 */
void RoadGraph::adaptToTerrain(MyTerrain* terrain) {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi) {
		float z = terrain->getValue(graph[*vi]->pt.x(), graph[*vi]->pt.y());
		graph[*vi]->pt3D = QVector3D(graph[*vi]->pt.x(), graph[*vi]->pt.y(), z + 10);
	}

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		graph[*ei]->polyline3D.clear();

		for (int i = 0; i < graph[*ei]->polyline.size(); ++i) {
			float z = terrain->getValue(graph[*ei]->polyline[i].x(), graph[*ei]->polyline[i].y());
			graph[*ei]->polyline3D.push_back(QVector3D(graph[*ei]->polyline[i].x(), graph[*ei]->polyline[i].y(), z + 10));
		}
	}

	setModified();
}
