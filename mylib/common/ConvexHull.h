#pragma once

#include <vector>
#include <QVector2D>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <common/Polygon2D.h>

using boost::geometry::model::d2::point_xy;
using boost::geometry::make;

class ConvexHull {
private:
	boost::geometry::model::multi_point<point_xy<float> > all_points;

public:
	ConvexHull() {}
	~ConvexHull() {}

	void addPoint(const QVector2D& p);
	void convexHull(Polygon2D& hull);
};

