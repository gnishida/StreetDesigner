#pragma once

#include <vector>
#include <QVector2D>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "common.h"

class BBox {
public:
	QVector2D minPt, maxPt;

public:
	BBox() {}
	~BBox() {}

	void addPoint(const QVector2D& pt);
	bool contains(const QVector2D& pt) const;
	QVector2D midPt() const;
	float dx() const;
	float dy() const;
	float area() const;
};

/**
 * BBoxをBoostのboxの代替として使用
 */
BOOST_GEOMETRY_REGISTER_BOX(BBox, QVector2D, minPt, maxPt)

