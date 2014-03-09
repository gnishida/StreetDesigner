/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#pragma once

#include <QVector2D>
#include <QVector3D>
#include <qmath.h>
#include "Polygon3D.h"
#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/multi/multi.hpp>
#endif

#ifndef BOOST_TYPEOF_SILENT
#define BOOST_TYPEOF_SILENT
#endif//BOOST_TYPEOF_SILENT

// We can conveniently use macro's to register point and ring
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(QVector3D, double, boost::geometry::cs::cartesian, x, y, setX, setY)
BOOST_GEOMETRY_REGISTER_RING(ucore::Loop3D)

namespace boost {
namespace geometry {

namespace traits {

template<> struct tag<ucore::Polygon3D> { typedef polygon_tag type; };

} // namespace traits

template<> struct ring_type<ucore::Polygon3D> { typedef ucore::Loop3D type; };

} // namespace geometry
} // namespace boost


namespace ucore {

typedef boost::numeric::ublas::matrix<double> tBoostMatrix;

class Loop3D;

class Util {
public:
	static const float MTC_FLOAT_TOL;

protected:
	Util();
	~Util();
	
public:
	static const QVector3D perp(const QVector3D& v);
	static void drawCone(const QVector3D& d, const QVector3D& a, const qreal h, const qreal rd, const int n);
	static bool loadPolygonsFromFile(QString filename, std::vector<Polygon3D>& polygons);
	static bool loadPolygonFromFile(QString filename, Polygon3D& polygon);
	static bool readSegmentsFromFile(QString filename, std::vector<QVector3D>& pts, std::vector<std::vector<int> >& idxsSets);
	static bool isIdxWithinBMatrix(int r, int c, tBoostMatrix& m);
	static float deg2rad(float deg);
	static float rad2deg(float rad);
	static double sumElementsinRow(int r, const tBoostMatrix& m);
	static double sumElementsinColumn(int c, const tBoostMatrix& m);
	static void printBMatrix(const tBoostMatrix& m);
	static void initBMatrix(tBoostMatrix& m);
	static float getClosestPointInArray(const std::vector<QVector3D>& pointArray, const QVector3D& point, int& idx);
	static QVector3D calculateNormal(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2);

	/** Distance from segment ab to point c */
	static float pointSegmentDistanceXY(const QVector3D& a, const QVector3D& b, const QVector3D& c);
	static float pointSegmentDistanceXY(const QVector3D& a, const QVector3D& b, const QVector3D& c, QVector3D& closestPtInAB);

	static bool segmentSegmentIntersectXY(const QVector2D& a, const QVector2D& b, const QVector2D& c, const QVector2D& d, float *tab, float *tcd, bool segmentOnly, QVector2D &intPoint);
	static bool rayTriangleIntersect(const QVector3D& rayPivot, const QVector3D& rayDirection, const QVector3D& p0, const QVector3D& p1, const QVector3D& p2);
	static bool rayTriangleIntersect(const QVector3D& rayPivot, const QVector3D& rayDirection, const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, QVector3D &intPt);
	static bool planeIntersectWithLine(const QVector3D& p1, const QVector3D& p2, const QVector3D& n, const QVector3D& p0, double &t, QVector3D &x);
	static float angleThreePoints(const QVector3D& pa, const QVector3D& pb, const QVector3D& pc);

	/* Random numbers */
	static float genRand();
	static float genRand(float a, float b);
	static float genRandNormal(float mean, float variance);

	/** Render circle parallel to XY plane, centered in X, Y, Z, and with radius r */
	static void renderCircle(float x, float y, float z, float radius);
	static void renderPolyline(const Loop3D& loopIn, bool closed);

	static float area3D_Polygon(const Loop3D& pin);
	static bool isPointWithinLoop(const std::vector<QVector3D>& loop, const QVector3D& pt);
	static bool is2DRingWithin2DRing(const boost::geometry::ring_type<Polygon3D>::type& contourA, const boost::geometry::ring_type<Polygon3D>::type& contourB);
	static double angleBetweenVectors(const QVector3D& vec1, const QVector3D& vec2);
	static bool getIrregularBisector(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, float d01, float d12, QVector3D& intPt);

	static float barycentricInterpolation(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, const QVector2D& p);

	static void HSVtoRGB(float* r, float* g, float* b, float h, float s, float v);
};

} // namespace ucore
