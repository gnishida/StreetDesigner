#pragma once

#include <vector>
#include <QVector2D>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/linestring.hpp> 

#ifndef M_PI
#define M_PI		3.141592653589793238
#endif

#ifndef SQR
#define SQR(x)		((x) * (x))
#endif

/**
 * QVector2DをBoostのpointの代替として使用
 */
BOOST_GEOMETRY_REGISTER_POINT_2D_GET_SET(QVector2D, float, boost::geometry::cs::cartesian, x, y, setX, setY)



