#include <boost/geometry/geometry.hpp> 
#include "BBox.h"

void BBox::addPoint(const QVector2D& pt) {
	boost::geometry::expand(*this, pt);
}

bool BBox::contains(const QVector2D& pt) const {
	return boost::geometry::within(pt, *this);
}

QVector2D BBox::midPt() const {
	QVector2D ret;
	boost::geometry::centroid(*this, ret);

	return ret;
}

float BBox::dx() const {
	return maxPt.x() - minPt.x();
}

float BBox::dy() const {
	return maxPt.y() - minPt.y();
}

float BBox::area() const {
	return boost::geometry::area(*this);
}
