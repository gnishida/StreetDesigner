#pragma once

#include <QVector3D>
#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/linestring.hpp>

class Polyline3D : public std::vector<QVector3D> {
public:
	Polyline3D() {}
	~Polyline3D() {}

	const QVector3D & last() const;
	QVector3D & last();

	void translate(const QVector2D &offset);
	void rotate(float angle, const QVector2D &orig);
	void scale(float factor);
};

/**
 * Linestringを定義
 */
BOOST_GEOMETRY_REGISTER_LINESTRING(Polyline3D)
