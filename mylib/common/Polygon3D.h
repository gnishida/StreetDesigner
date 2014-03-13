#pragma once

#include <vector>
#include <QVector3D>
#include <boost/geometry.hpp>
#include <boost/polygon/polygon.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "BBox.h"

class Polygon3D : public std::vector<QVector3D> {
public:
	Polygon3D() {}
	~Polygon3D() {}

	void correct();
	float area() const;
	QVector2D centroid() const;
	bool contains(const QVector3D &pt) const;
	bool contains(const QVector3D &pt);
	bool contains(const Polygon3D &polygon) const;
	BBox envelope() const;
	void translate(float x, float y);
	void translate(float x, float y, Polygon3D &ret) const;
	void rotate(float angle);
	void rotate(float angle, Polygon3D &ret) const;
	void rotate(float angle, const QVector2D &orig);

	std::vector<Polygon3D> tessellate() const;

	float computeInset(float offsetDistance, Polygon3D &pgonInset, bool computeArea = true);
	float computeInset(std::vector<float> offsetDistances, Polygon3D &pgonInset, bool computeArea = true);
};

/**
 * Polygon3DをBoostのringの代替として使用
 */
BOOST_GEOMETRY_REGISTER_RING(Polygon3D)

