#pragma once

#include <QVector2D>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/linestring.hpp>

class Polyline2D : public std::vector<QVector2D> {
public:
	Polyline2D() {}
	~Polyline2D() {}

	const QVector2D & last() const;
	QVector2D & last();

	void translate(const QVector2D &offset);
	void rotate(float angle, const QVector2D &orig);
	void scale(float factor);
};

/**
 * Linestringを定義
 */
BOOST_GEOMETRY_REGISTER_LINESTRING(Polyline2D)
