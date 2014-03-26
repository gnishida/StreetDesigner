#include <boost/geometry/geometry.hpp> 
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/polygon/polygon.hpp>
#include "Polygon3D.h"
#include "Polyline3D.h"
#include "Util.h"

const QVector3D & Polygon3D::last() const {
	return at(size() - 1);
}

QVector3D & Polygon3D::last() {
	return at(size() - 1);
}

void Polygon3D::correct() {
	boost::geometry::correct(*this);
}

float Polygon3D::area() const {
	return fabs(boost::geometry::area(*this));
}

QVector2D Polygon3D::centroid() const {
	QVector2D ret;
	boost::geometry::centroid(*this, ret);

	return ret;
}

bool Polygon3D::contains(const QVector3D &pt) const {
	return boost::geometry::within(pt, *this);
}

bool Polygon3D::contains(const QVector3D &pt) {
	return boost::geometry::within(pt, *this);
}

bool Polygon3D::contains(const Polygon3D &polygon) const {
	for (int i = 0; i < polygon.size(); ++i) {
		if (!contains(polygon[i])) return false;
	}

	return true;
}

BBox Polygon3D::envelope() const {
	BBox bbox;
	boost::geometry::envelope(*this, bbox);
	return bbox;
}

/**
 * 当該ポリゴンを移動する。
 */
void Polygon3D::translate(float x, float y) {
	Polygon3D temp = *this;
	this->clear();

	boost::geometry::strategy::transform::translate_transformer<QVector3D, QVector3D> translate(x, y);
    boost::geometry::transform(temp, *this, translate);
}

/**
 * 移動したポリゴンを返却する。
 *
 * @param x			X軸方向の移動距離
 * @param y			Y軸方向の移動距離
 * @ret				移動したポリゴン
 */
void Polygon3D::translate(float x, float y, Polygon3D &ret) const {
	boost::geometry::strategy::transform::translate_transformer<QVector3D, QVector3D> translate(x, y);
    boost::geometry::transform(*this, ret, translate);
}

/**
 * 原点を中心に、指定した角度だけ時計回りに回転する。
 *
 * @param angle		時計回りの回転角度[degree]
 */
void Polygon3D::rotate(float angle) {
	Polygon3D temp = *this;
	this->clear();

	boost::geometry::strategy::transform::rotate_transformer<QVector3D, QVector3D, boost::geometry::degree> rotate(angle);
    boost::geometry::transform(temp, *this, rotate);
}

/**
 * 原点を中心に、指定した角度だけ時計回りに回転したポリゴンを返却する。
 *
 * @param angle		時計回りの回転角度[degree]
 * @ret				回転したポリゴン
 */
void Polygon3D::rotate(float angle, Polygon3D &ret) const {
	boost::geometry::strategy::transform::rotate_transformer<QVector3D, QVector3D, boost::geometry::degree> rotate(angle);
    boost::geometry::transform(*this, ret, rotate);
}

/**
 * 指定された点を中心に、指定された角度だけ時計回りに回転したポリゴンを返却する。
 *
 * @param angle		時計回りの回転角度[degree]
 * @param orig		回転中心
 */
void Polygon3D::rotate(float angle, const QVector2D &orig) {
	translate(-orig.x(), -orig.y());
	rotate(angle);
	translate(orig.x(), orig.y());
}

/**
 * このポリゴンを三角形または凸四角形の集合に分割する。
 * 2Dでのtessellationを行い、得られる図形のZ座標は全て0となる。
 * また、tessellateされた各図形の頂点はclosedで、CCWオーダである。
 */
std::vector<Polygon3D> Polygon3D::tessellate() const {
	std::vector<Polygon3D> trapezoids;

	if (size() < 3) return trapezoids;

	typedef boost::polygon::point_data<double> point;
	typedef boost::polygon::polygon_set_data<double> polygon_set;
	typedef boost::polygon::polygon_with_holes_data<double> bpolygon;

	std::vector<point> pts;
	//pts.push_back(point(at(0).x(), at(0).y()));
	for (int i = size() - 1; i >= 0; i--) {
		pts.push_back(point(at(i).x(), at(i).y()));
	}

	bpolygon ply;
	boost::polygon::set_points( ply, pts.begin(), pts.end() );

  //using namespace gtl::operators;
	std::vector<bpolygon> ots;
	polygon_set plys;
	plys.insert(ply);
	plys.get_trapezoids(ots);

	for (int i = 0; i < ots.size(); ++i) {
		Polygon3D trapezoid;

		for (boost::polygon::polygon_with_holes_data<double>::iterator_type it = ots[i].begin(); it != ots[i].end(); ++it) {
			float z;

			// 直近の頂点を探してZ座標を決定する
			float min_dist = std::numeric_limits<float>::max();
			for (int j = 0; j < size(); ++j) {
				float dist = SQR(at(j).x() - (*it).x()) + SQR(at(j).y() - (*it).y());
				if (dist < min_dist) {
					min_dist = dist;
					z = at(j).z();
				}
			}

			trapezoid.push_back(QVector3D((*it).x(), (*it).y(), z));
		}

		if (trapezoid.size() >= 4) {
			// closedのポリゴンなので、最初の頂点を削除する
			trapezoid.erase(trapezoid.begin());

			// CWオーダなので、CCWオーダにする
			std::reverse(trapezoid.begin(), trapezoid.end());

			trapezoids.push_back(trapezoid);
		}
	}

	/*
	// create 2D polygon data
	std::vector<boost::polygon::point_data<float>  > polygon;
	polygon.resize(size());
	for (int i = 0; i < size(); i++) {
		polygon[i] = boost::polygon::construct<boost::polygon::point_data<float> >(at(i).x(), at(i).y());
	}

	// create 2D polygon with holes data
	boost::polygon::polygon_with_holes_data<float> temp;
	boost::polygon::set_points(temp, polygon.begin(), polygon.end());

	// create 2D polygon set
	boost::polygon::polygon_set_data<float> polygonSet;
	polygonSet.insert(temp);

	// tessellation
	std::vector<boost::polygon::polygon_with_holes_data<float> > results;
	polygonSet.get_trapezoids(results);

	for (int i = 0; i < results.size(); i++) {
		boost::polygon::polygon_with_holes_data<float>::iterator_type it = results[i].begin();
		Polygon3D trapezoid;
		while (it != results[i].end()) {
			float z = 0.0f;
			bool done = false;

			// 直近の頂点のZ座標を使ってZ座標を決定する
			for (int j = 0; j < size(); j++) {
				if (at(j).x() == (*it).x() && at(j).y() == (*it).y()) {
					z = at(j).z();
					done = true;
					break;
				}
			}

			if (!done) {
				int v1, v2;
				float s;
				findEdge((*it).x(), (*it).y(), v1, v2, s);
				z = at(v1).z() + (at(v2).z() - at(v1).z()) * s;
			}

			trapezoid.push_back(QVector3D((*it).x(), (*it).y(), z));
			it++;
		}

		if (trapezoid.size() < 3) continue;

		if (trapezoid.size() >= 3) trapezoids.push_back(trapezoid);
	}
	*/

	return trapezoids;
}

void Polygon3D::findEdge(float x, float y, int& v1, int& v2, float& s) const {
	float minDist = (std::numeric_limits<float>::max)();

	for (int i = 0; i < size(); i++) {
		int next = (i + 1) % size();

		float dist = Util::pointSegmentDistanceXY(at(i), at(next), QVector3D(x, y, 0));
		if (dist < minDist) {
			minDist = dist;
			v1 = i;
			v2 = next;
			if (fabs(at(next).x() - at(i).x()) > fabs(at(next).y() - at(i).y())) {
				s = (x - at(i).x()) / (at(next).x() - at(i).x());
			} else {
				if (at(next).y() - at(i).y() != 0.0f) {
					s = (y - at(i).y()) / (at(next).y() - at(i).y());                                
				} else {
					s = 0.0f;
				}
			}
		}
	}
}

/**
* @brief: Given a polygon, this function computes the polygon's inwards offset. The offset distance
* is not assumed to be constant and must be specified in the vector offsetDistances. The size of this
* vector must be equal to the number of vertices of the polygon.
* Note that the i-th polygon segment is defined by vertices i and i+1.
* The polygon vertices are assumed to be oriented clockwise
* @param[in] offsetDistances: Perpendicular distance from offset segment i to polygon segment i.
* @param[out] pgonInset: The vertices of the polygon inset
* @return insetArea: Returns the area of the polygon inset		
**/
void Polygon3D::computeInset(float offsetDistance, Polygon3D &pgonInset) {
	if (size() < 3) return;
	std::vector<float> offsetDistances(size(), offsetDistance);

	computeInset(offsetDistances, pgonInset);
}

void Polygon3D::computeInset(std::vector<float> offsetDistances, Polygon3D &pgonInset) {
	Polygon3D cleanPgon = *this;
	double tol = 0.01f;

	int prev, next;
	int cSz = cleanPgon.size();

	if (cSz < 3) return;

	//if offsets are zero, add a small epsilon just to avoid division by zero
	for (size_t i=0; i<offsetDistances.size(); ++i){
		if(fabs(offsetDistances[i]) < tol){
			offsetDistances[i] = tol;
		}
	}

	pgonInset.resize(cSz);
	QVector3D intPt;

	for (int cur = 0; cur < cSz; ++cur) {
		//point p1 is the point with index cur
		prev = (cur-1+cSz)%cSz; //point p0
		next = (cur+1)%cSz;	  //point p2

		Util::getIrregularBisector(cleanPgon[prev], cleanPgon[cur], cleanPgon[next], offsetDistances[prev], offsetDistances[cur], intPt);

		pgonInset[cur] = intPt;
	}
}
