#pragma once

#include <vector>
#include <Qvector2D>
#include <QColor>
#include <boost/shared_ptr.hpp>
#include "../common/Polyline3D.h"
#include "../common/Polyline2D.h"

class RoadEdge {
public:
	static enum { SHAPE_DEFAULT = 0, SHAPE_GRID, SHAPE_RADIAL, SHAPE_PLAZA };
	static enum { TYPE_OTHERS = 0, TYPE_STREET = 1, TYPE_AVENUE = 2, TYPE_BOULEVARD = 4, TYPE_HIGHWAY = 8 };

public:
	int type;
	int lanes;
	bool oneWay;
	bool link;
	bool roundabout;
	Polyline2D polyline;
	Polyline3D polyline3D;
	QColor color;
	QColor bgColor;

	bool valid;			// if this edge is valid
	bool seed;			// if this edge is used as a seed of a forest

	int shapeType;		// 0 - default / 1 - grid / 2 - radial / 3 - plaza
	int group;			// to which tree in the forest this edge belongs to
	float gridness;		// how much it looks like grid
	bool fullyPaired;	// if this edge has a corresponding edge

public:
	RoadEdge(unsigned int type, unsigned int lanes, bool oneWay = false, bool link = false, bool roundabout = false);
	~RoadEdge();
	
	float getLength();

	void addPoint(const QVector2D &pt);
	float getWidth(float widthPerLane = 10.0f);

	bool containsPoint(const QVector2D &pos, float widthPerLane, int& index);
};

typedef boost::shared_ptr<RoadEdge> RoadEdgePtr;