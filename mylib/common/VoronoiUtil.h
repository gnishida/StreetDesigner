#pragma once

#include "Polygon2D.h"
#include <QVector2D>
#include <boost/polygon/voronoi.hpp>

class VoronoiVertex {
public:
	QVector2D pt;

public:
	VoronoiVertex(const QVector2D& pt) {
		this->pt = pt;
	}
};

namespace boost {
namespace polygon {
template <>
struct geometry_concept<VoronoiVertex> { typedef point_concept type; };
  
template <>
struct point_traits<VoronoiVertex> {
	typedef int coordinate_type;
   
	static inline coordinate_type get(const VoronoiVertex& v, orientation_2d orient) {
		return (orient == HORIZONTAL) ? v.pt.x() * 100 : v.pt.y() * 100;
	}
};
}
}

class VoronoiUtil {
protected:
	VoronoiUtil() {}
	~VoronoiUtil() {}

public:
	static void voronoi(std::vector<VoronoiVertex> points, Polygon2D& area, std::vector<Polygon2D>& faces);
};

