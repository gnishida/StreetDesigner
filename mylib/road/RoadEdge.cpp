#include "../common/Util.h"
#include "RoadEdge.h"

RoadEdge::RoadEdge(unsigned int type, unsigned int lanes, bool oneWay, bool link, bool roundabout) {
	this->type = type;
	this->lanes = lanes;
	this->oneWay = oneWay;
	this->link = link;
	this->roundabout = roundabout;

	this->color = QColor(255, 255, 255);

	// initialize other members
	this->valid = true;
	this->properties["seed"] = false;
	this->properties["shapeType"] = SHAPE_DEFAULT;
	this->properties["group"] = -1;
	this->properties["gridness"] = 0.0f;
	this->properties["fullyPaired"] = false;

	// default color
	switch (type) {
	case TYPE_HIGHWAY:
		color = QColor(255, 225, 104);
		bgColor = QColor(229, 153, 21);
		break;
	case TYPE_BOULEVARD:
		color = QColor(248, 213, 169);
		bgColor = QColor(210, 170, 119);
		break;
	case TYPE_AVENUE:
		color = QColor(247, 247, 185);
		bgColor = QColor(203, 202, 149);
		break;
	case TYPE_STREET:
		color = QColor(255, 255, 255);
		bgColor = QColor(217, 209, 201);
		break;
	} 
}

RoadEdge::~RoadEdge() {
}

float RoadEdge::getLength() {
	float length = 0.0f;
	for (int i = 0; i < polyline.size() - 1; i++) {
		length += (polyline[i + 1] - polyline[i]).length();
	}

	return length;
}

/**
 * Add a point to the polyline of the road segment.
 *
 * @param pt		new point to be added
 */
void RoadEdge::addPoint(const QVector2D &pt) {
	polyline.push_back(pt);
}

float RoadEdge::getWidth(float widthPerLane) {
	/*
	switch (type) {
	case TYPE_HIGHWAY:
		return widthPerLane * 2.0f * lanes;
	case TYPE_BOULEVARD:
	case TYPE_AVENUE:
		return widthPerLane * 1.5f * lanes;
	case TYPE_STREET:
		return widthPerLane * lanes;
	default:
		return 0.0f;
	}*/

	return widthPerLane * 2.0f;
}

/**
 * Check whether the point resides in this road segment.
 * Return true if so, false otherwise.
 *
 * @param pos		the point
 * @return			true if the point is inside the road segment, false otherwise
 */
bool RoadEdge::containsPoint(const QVector2D &pos, float widthPerLane, int& index) {
	for (int i = 0; i < polyline.size() - 1; i++) {
		QVector2D p0(polyline[i].x(), polyline[i].y());
		QVector2D p1(polyline[i + 1].x(), polyline[i + 1].y());

		if (Util::pointSegmentDistanceXY(p0, p1, pos) <= getWidth(widthPerLane)) {
			// find the closest point
			float min_dist = std::numeric_limits<float>::max();
			for (int j = 0; j < polyline.size(); j++) {
				float dist = (polyline[j] - pos).length();
				if (dist < min_dist) {
					min_dist = dist;
					index = j;
				}
			}

			return true;
		}
	}

	return false;
}