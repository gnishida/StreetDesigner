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

#include "Polygon3D.h"
#include "Util.h"
#include <QGLWidget>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QCoreApplication>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/multi/multi.hpp>

#include <boost/polygon/polygon.hpp>
#include <gl/GLU.h>



void Loop3D::close() {
	if (size() == 0) return;

	if (this->at(0) != this->at(size() - 1)) {
		push_back(this->at(0));
	}
}

void Loop3D::tessellate(std::vector<Loop3D>& trapezoids) const {
	trapezoids.clear();

	if (size() < 3) return;

	// create 2D polygon data
	std::vector<boost::polygon::point_data<double> > polygon;
	polygon.resize(size());
	for (int i = 0; i < size(); i++) {
		polygon[i] = boost::polygon::construct<boost::polygon::point_data<double> >(at(i).x(), at(i).y());
	}

	// create 2D polygon with holes data
	boost::polygon::polygon_with_holes_data<double> temp;
	boost::polygon::set_points(temp, polygon.begin(), polygon.end());

	// create 2D polygon set
	boost::polygon::polygon_set_data<double> polygonSet;
	polygonSet.insert(temp);

	// tessellation
	std::vector<boost::polygon::polygon_with_holes_data<double> > results;
	polygonSet.get_trapezoids(results);

	for (int i = 0; i < results.size(); i++) {
		boost::polygon::polygon_with_holes_data<double>::iterator_type it = results[i].begin();
		Loop3D trapezoid;
		Loop3D tex_coord;
		while (it != results[i].end()) {
			float z = 0.0f;
			bool done = false;

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

		Polygon3D::reorientFace(trapezoid);

		// The resulting polygon is usually closed, so remove the last point.
		if ((trapezoid[trapezoid.size() - 1] - trapezoid[0]).lengthSquared() == 0) {
			trapezoid.pop_back();
		}

		if (trapezoid.size() >= 3) trapezoids.push_back(trapezoid);
	}
}

void Loop3D::findEdge(float x, float y, int& v1, int& v2, float& s) const {
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

Loop3D Loop3D::createRectangle(float width, float height) {
	Loop3D rectangle;
	rectangle.push_back(QVector3D(-width/2.0f, -height/2.0f, 0.0f));
	rectangle.push_back(QVector3D(width/2.0f, -height/2.0f, 0.0f));
	rectangle.push_back(QVector3D(width/2.0f, height/2.0f, 0.0f));
	rectangle.push_back(QVector3D(-width/2.0f, height/2.0f, 0.0f));

	return rectangle;
}

Polygon3D::Polygon3D() {
	isNormalVecValid = false;
	isCentroidValid = false;
	//normalVec = QVector3D(0.0f, 0.0f, 0.0f);
	//centroid = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
}

Polygon3D::~Polygon3D() {
	contour.clear();
}

void Polygon3D::setContour(const Loop3D& contour) {
	this->contour = contour;

	// normalVec and centroid are to be updated later when they are requested
	isNormalVecValid = false;
	isCentroidValid = false;
}

void Polygon3D::clear() {
	contour.clear();
	isNormalVecValid = false;
	isCentroidValid = false;
	//normalVec = QVector3D(0.0f, 0.0f, 0.0f);
	//centroid = QVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
}

void Polygon3D::push_back(const QVector3D& point) {
	contour.push_back(point);

	// normalVec and centroid are to be updated later when they are requested
	isNormalVecValid = false;
	isCentroidValid = false;
}

int Polygon3D::size() const {
	return contour.size();
}

QVector3D& Polygon3D::getNormalVector() {	
	if (!isNormalVecValid) {
		normalVec = getLoopNormalVector(this->contour);
		isNormalVecValid = true;
	}
	return normalVec;
}

/**
 * Get center of vertices
 */
QVector3D& Polygon3D::getCentroid() {
	if (isCentroidValid) {
		return centroid;
	} else {
		QVector3D newCentroid(0.0f, 0.0f, 0.0f);
		int cSz = contour.size();
		for (int i = 0; i < cSz; ++i) {
			newCentroid = newCentroid + contour[i];
		}
		if (cSz > 0) {
			centroid = (newCentroid/cSz);						
		}			
		isCentroidValid = true;

		return centroid;
	}
}

void Polygon3D::renderContour() {	
	glBegin(GL_LINE_LOOP);
	for (size_t i = 0; i < contour.size(); ++i) {
		glVertex3f(contour[i].x(), contour[i].y(), contour[i].z());
	}
	glEnd();	
}

void Polygon3D::render() {		
	glBegin(GL_POLYGON);
	for (size_t i = 0; i < contour.size(); ++i) {
		glVertex3f(contour[i].x(), contour[i].y(), contour[i].z());
	}
	glEnd();			
}

void Polygon3D::renderNonConvex(bool reComputeNormal, float nx, float ny, float nz) {
	if (contour.size() < 3) return;

	QVector3D myNormal;
	if (reComputeNormal) {
		myNormal = this->getNormalVector();
	} else {
		myNormal.setX(nx);
		myNormal.setY(ny);
		myNormal.setZ(nz);
	}

	//Render inside fill			
	if (contour.size() == 3) {
		glBegin(GL_TRIANGLES);	
		for(size_t i=0; i<contour.size(); ++i){	
			glNormal3f(myNormal.x(), myNormal.y(), myNormal.z());
			glVertex3f(contour[i].x(), contour[i].y(), contour[i].z());			
		}
		glEnd();
	} else if (contour.size() == 4) {
		glBegin(GL_QUADS);	
		for(int i=0; i<contour.size(); ++i){	
			glNormal3f(myNormal.x(), myNormal.y(), myNormal.z());
			glVertex3f(contour[i].x(), contour[i].y(), contour[i].z());			
		}
		glEnd();
	} else {
		// create tessellator
		GLUtesselator *tess = gluNewTess();

		double *vtxData = new double[3*contour.size()];
		for (size_t i = 0; i < contour.size(); ++i) {
			vtxData[3*i]=contour[i].x();
			vtxData[3*i+1]=contour[i].y();
			vtxData[3*i+2]=contour[i].z();
		}

		// register callback functions
		gluTessCallback(tess, GLU_TESS_BEGIN, 
			(void (__stdcall *)(void))glBegin);
		gluTessCallback(tess, GLU_TESS_VERTEX,
			(void (__stdcall *)(void))glVertex3dv);
		gluTessCallback(tess, GLU_TESS_END, glEnd);

		// describe non-convex polygon
		gluTessBeginPolygon(tess, NULL);

		// contour
		gluTessBeginContour(tess);

		for (size_t i = 0; i < contour.size(); ++i) {
			//HACK
			glNormal3f(myNormal.x(), myNormal.y(), fabs(myNormal.z()));
			gluTessVertex(tess, &vtxData[3*i], &vtxData[3*i]);
		}
		gluTessEndContour(tess);

		gluTessEndPolygon(tess);

		// delete tessellator after processing
		gluDeleteTess(tess);

		delete [] vtxData;
	}
}

bool Polygon3D::isSelfIntersecting() {
	return isSelfIntersecting(this->contour);
}

bool Polygon3D::isSelfIntersecting(const Loop3D& contour) {
	boost::geometry::ring_type<Polygon3D>::type bg_pgon;
	boost::geometry::assign(bg_pgon, contour);
	boost::geometry::correct(bg_pgon);
	return boost::geometry::intersects(bg_pgon);
}

/**
 * Only works for polygons with no holes in them
 */
bool Polygon3D::splitMeWithPolyline(const std::vector<QVector3D>& pline, Loop3D &pgon1, Loop3D &pgon2) {
	bool polylineIntersectsPolygon = false;

	size_t plineSz = pline.size();
	size_t contourSz = this->contour.size();

	if(plineSz < 2 || contourSz < 3){
		//std::cout << "ERROR: Cannot split if polygon has fewer than three vertices of if polyline has fewer than two points\n.";
		return false;
	}

	QVector2D tmpIntPt;
	QVector3D firstIntPt;
	QVector3D secondIntPt;
	float tPline, tPgon;
	int firstIntPlineIdx    = -1;
	int secondIntPlineIdx   = -1;
	int firstIntContourIdx  = -1;
	int secondIntContourIdx = -1;
	int intCount = 0;

	//iterate along polyline
	for (int i = 0; i < plineSz - 1; ++i) {
		int iNext = i+1;

		for (int j = 0; j < contourSz; ++j) {
			int jNext = (j+1)%contourSz;

			if (Util::segmentSegmentIntersectXY(QVector2D(pline[i]), QVector2D(pline[iNext]), QVector2D(contour[j]), QVector2D(contour[jNext]), &tPline, &tPgon, true, tmpIntPt)) {
				polylineIntersectsPolygon = true;
				float z = contour[j].z() + (contour[jNext].z() - contour[j].z()) * tPgon;

				//first intersection
				if (intCount == 0) {
					firstIntPlineIdx = i;
					firstIntContourIdx = j;
					firstIntPt = QVector3D(tmpIntPt.x(), tmpIntPt.y(), z);
				} else if (intCount == 1) {
					secondIntPlineIdx = i;
					secondIntContourIdx = j;
					secondIntPt = QVector3D(tmpIntPt.x(), tmpIntPt.y(), z);
				} else {
					return false;
				}
				intCount++;
			}
		}
	}

	if (intCount != 2) return false;

	//Once we have intersection points and indexes, we reconstruct the two polygons
	pgon1.clear();
	pgon2.clear();
	int pgonVtxIte;
	int plineVtxIte;

	//If first polygon segment intersected has an index greater
	//	than second segment, modify indexes for correct reconstruction
	if (firstIntContourIdx > secondIntContourIdx) {
		secondIntContourIdx += contourSz;
	}

	//==== Reconstruct first polygon
	//-- append polygon contour
	pgon1.push_back(firstIntPt);
	pgonVtxIte = firstIntContourIdx;
	while (pgonVtxIte < secondIntContourIdx) {
		pgon1.push_back(contour[(pgonVtxIte+1)%contourSz]);
		pgonVtxIte++;
	}
	pgon1.push_back(secondIntPt);
	//-- append polyline points
	plineVtxIte = secondIntPlineIdx;
	while (plineVtxIte > firstIntPlineIdx) {
		pgon1.push_back(pline[(plineVtxIte)]);
		plineVtxIte--;
	}

	//==== Reconstruct second polygon
	//-- append polygon contour
	pgon2.push_back(secondIntPt);
	pgonVtxIte = secondIntContourIdx;
	while (pgonVtxIte < firstIntContourIdx + contourSz) {
		pgon2.push_back(contour[(pgonVtxIte+1) % contourSz]);
		pgonVtxIte++;
	}
	pgon2.push_back(firstIntPt);
	//-- append polyline points
	plineVtxIte = firstIntPlineIdx;
	while (plineVtxIte < secondIntPlineIdx) {
		pgon2.push_back(pline[(plineVtxIte + 1)]);
		plineVtxIte++;
	}

	//verify that two polygons are created after the split. If not, return false
	if (pgon1.size() < 3 || pgon2.size() < 3) {
		return false;
	}

	return polylineIntersectsPolygon;
}

/**
 * Only works for polygons with no holes in them
 */
bool Polygon3D::splitMeWithPolyline2(const std::vector<QVector3D>& pline, Loop3D &pgon1, Loop3D &pgon2) {
	bool polylineIntersectsPolygon = false;

	size_t plineSz = pline.size();
	size_t contourSz = this->contour.size();

	if(plineSz < 2 || contourSz < 3){
		//std::cout << "ERROR: Cannot split if polygon has fewer than three vertices of if polyline has fewer than two points\n.";
		return false;
	}

	QVector2D tmpIntPt;
	QVector2D firstIntPt;
	QVector2D secondIntPt;
	float tPline, tPgon;
	int firstIntPlineIdx    = -1;
	int secondIntPlineIdx   = -1;
	int firstIntContourIdx  = -1;
	int secondIntContourIdx = -1;
	int intCount = 0;

	//iterate along polyline
	for (int i = 0; i < plineSz - 1; ++i) {
		int iNext = i+1;

		for (int j = 0; j < contourSz; ++j) {
			int jNext = (j+1)%contourSz;

			if (Util::segmentSegmentIntersectXY(QVector2D(pline[i]), QVector2D(pline[iNext]), QVector2D(contour[j]), QVector2D(contour[jNext]), &tPline, &tPgon, true, tmpIntPt)) {
				polylineIntersectsPolygon = true;

				//first intersection
				if (intCount == 0) {
					firstIntPlineIdx = i;
					firstIntContourIdx = j;
					firstIntPt = tmpIntPt;
				} else if (intCount == 1) {
					secondIntPlineIdx = i;
					secondIntContourIdx = j;
					secondIntPt = tmpIntPt;
				} else {
					return false;
				}
				intCount++;
			}
		}
	}

	if (intCount != 2) return false;

	//Once we have intersection points and indexes, we reconstruct the two polygons
	pgon1.clear();
	pgon2.clear();
	int pgonVtxIte;
	int plineVtxIte;

	//If first polygon segment intersected has an index greater
	//	than second segment, modify indexes for correct reconstruction
	if (firstIntContourIdx > secondIntContourIdx) {
		secondIntContourIdx += contourSz;
	}

	//==== Reconstruct first polygon
	//-- append polygon contour
	pgon1.push_back(firstIntPt);
	pgonVtxIte = firstIntContourIdx;
	while (pgonVtxIte < secondIntContourIdx) {
		pgon1.push_back(contour[(pgonVtxIte+1)%contourSz]);
		pgonVtxIte++;
	}
	pgon1.push_back(secondIntPt);
	//-- append polyline points
	plineVtxIte = secondIntPlineIdx;
	while (plineVtxIte > firstIntPlineIdx) {
		pgon1.push_back(pline[(plineVtxIte)]);
		plineVtxIte--;
	}

	//==== Reconstruct second polygon
	//-- append polygon contour
	pgon2.push_back(secondIntPt);
	pgonVtxIte = secondIntContourIdx;
	while (pgonVtxIte < firstIntContourIdx + contourSz) {
		pgon2.push_back(contour[(pgonVtxIte+1) % contourSz]);
		pgonVtxIte++;
	}
	pgon2.push_back(firstIntPt);
	//-- append polyline points
	plineVtxIte = firstIntPlineIdx;
	while (plineVtxIte < secondIntPlineIdx) {
		pgon2.push_back(pline[(plineVtxIte + 1)]);
		plineVtxIte++;
	}

	//verify that two polygons are created after the split. If not, return false
	if (pgon1.size() < 3 || pgon2.size() < 3) {
		return false;
	}

	return polylineIntersectsPolygon;
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
float Polygon3D::computeInset(float offsetDistance, Loop3D &pgonInset, bool computeArea) {
	if(contour.size() < 3) return 0.0f;				
	std::vector<float> offsetDistances(contour.size(), offsetDistance);

	return computeInset(offsetDistances, pgonInset, computeArea);
}

float Polygon3D::computeInset(std::vector<float> offsetDistances, Loop3D &pgonInset, bool computeArea) {
	Loop3D cleanPgon;
	double tol = 0.01f;

	cleanPgon = this->contour;

	int prev, next;
	int cSz = cleanPgon.size();

	if (cSz < 3) return 0.0f;

	if (Polygon3D::reorientFace(cleanPgon)) {
		std::reverse(offsetDistances.begin(), offsetDistances.end());
	}

	//if offsets are zero, add a small epsilon just to avoid division by zero
	for(size_t i=0; i<offsetDistances.size(); ++i){
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

	//Compute inset area
	if(computeArea){

		boost::geometry::ring_type<Polygon3D>::type bg_contour;
		boost::geometry::ring_type<Polygon3D>::type bg_contour_inset;
		float contArea;
		float contInsetArea;

		if (pgonInset.size() > 0) {
			boost::geometry::assign(bg_contour_inset, pgonInset);
			boost::geometry::correct(bg_contour_inset);

			if (boost::geometry::intersects(bg_contour_inset)) {
				pgonInset.clear();
				return 0.0f;
			} else {

				boost::geometry::assign(bg_contour, cleanPgon);
				boost::geometry::correct(bg_contour);
				//if inset is not within polygon
				if (!Util::is2DRingWithin2DRing(bg_contour_inset, bg_contour)) {
					pgonInset.clear();
					return 0.0f;
				} else {
					contArea = fabs(boost::geometry::area(bg_contour));
					contInsetArea = fabs(boost::geometry::area(bg_contour_inset));

					if (contInsetArea < contArea) {
						//return boost::geometry::area(bg_contour_inset);
						return contInsetArea;
					} else {
						pgonInset.clear();
						return 0.0f;
					}
				}
			}
		} else {
			pgonInset.clear();
			return 0.0f;
		}
	}
	return 0.0f;

}

float Polygon3D::computeArea(bool parallelToXY) {
	return Polygon3D::computeLoopArea(this->contour, parallelToXY);
}

/**
 * Reorient polygon vertices so that they are ordered in CCW.
 *
 * @param onlyCheck true: the polygon is not modified
 * @return true if the original polygon is ordered in CW.
 **/
bool Polygon3D::reorientFace(bool onlyCheck) {
	return Polygon3D::reorientFace(contour, onlyCheck);
	/*
	int pfaceSz = contour.size();
	int next;
	float tmpSum = 0.0f;

	for (int i = 0; i < pfaceSz; ++i) {
		next = (i+1) % pfaceSz;
		tmpSum = tmpSum + (contour[next].x() - contour[i].x()) * (contour[next].y() + contour[i].y());
	}			

	if (tmpSum > 0.0f) {
		if (!onlyCheck) {
			std::reverse(contour.begin(), contour.end());
		}
		return true;
	} else {
		return false;
	}
	*/
}

QVector3D Polygon3D::getLoopNormalVector(const Loop3D& pin) {
	if (pin.size() >= 3) {
		return Util::calculateNormal(pin[0], pin[1], pin[2]);
	}
	return QVector3D(0, 0, 0);
}

/**
 * Reorient polygon vertices so that they are ordered in CCW.
 *
 * @param pface the vertices of the polygon
 * @param onlyCheck true: the polygon is not modified
 * @return true if the original polygon is ordered in CW.
 **/
bool Polygon3D::reorientFace(Loop3D& pface, bool onlyCheck) {
	int pfaceSz = pface.size();
	int next;
	float tmpSum = 0.0f;

	for (int i = 0; i < pfaceSz; ++i) {
		next = (i + 1) % pfaceSz;
		tmpSum = tmpSum + (pface[next].x() - pface[i].x()) * (pface[next].y() + pface[i].y());
	}			

	if (tmpSum > 0.0f) {				
		if (!onlyCheck) {
			std::reverse(pface.begin(), pface.end());
		}
		return true;
	}
	return false;
}

/**
 * @brief: Merge consecutive vertices that are within a distance threshold to each other
 * This function does not support Z coordinate. 
 **/
int Polygon3D::cleanLoop(const Loop3D& pin, Loop3D& pout, float threshold = 1.0f) {
	float thresholdSq = threshold * threshold;

	if (pin.size() < 3) {
		return 1;
	}

	boost::geometry::ring_type<Polygon3D>::type bg_pin;
	boost::geometry::ring_type<Polygon3D>::type bg_pout;
	boost::geometry::assign(bg_pin, pin);
	boost::geometry::correct(bg_pin);
	boost::geometry::simplify(bg_pin, bg_pout, threshold);

	//strategy::simplify::douglas_peucker

	//copy points back
	QVector3D tmpPt;
	for (size_t i = 0; i < bg_pout.size(); ++i) {
		tmpPt.setX(bg_pout[i].x());
		tmpPt.setY(bg_pout[i].y());
		pout.push_back(tmpPt);						
	}

	//remove last point
	if ((pout[0] - pout[pout.size()-1]).lengthSquared() < thresholdSq) {
		pout.pop_back();				
	}

	//clean angles
	int next, prev;
	QVector3D cur_prev, cur_next;
	float ang;
	float angleThreshold = 0.01f;
	for (size_t i = 0; i < pout.size(); ++i) {
		next = (i+1) % pout.size();
		prev = (i-1+pout.size()) % pout.size();
		cur_prev = pout[prev] - pout[i];
		cur_next = pout[next] - pout[i];

		ang = Util::angleBetweenVectors(cur_prev, cur_next);
		if ((fabs(ang) < angleThreshold) || (fabs(ang - M_PI ) < angleThreshold) || (!(ang==ang))) {
			pout.erase(pout.begin() + i);
			--i;
		}
	}

	return 0;
}

void Polygon3D::transformLoop(const Loop3D& pin, Loop3D& pout, QMatrix4x4& transformMat) {
	pout = pin;
	for (int i=0; i<pin.size(); ++i) {
		pout.at(i) = transformMat * pin.at(i);
	}
}

float Polygon3D::computeLoopArea(const Loop3D& pin, bool parallelToXY) {
	float _area = 0.0f;

	if (parallelToXY) {	
		boost::geometry::ring_type<Polygon3D>::type bg_pgon;
		boost::geometry::assign(bg_pgon, pin);
		bg_pgon.close();
		_area = fabs(boost::geometry::area(bg_pgon));
	} else {				
		_area = fabs(Util::area3D_Polygon(pin));
	}
	return _area;
}

/**
 * density: number of points to be generated per square meter
 */
void Polygon3D::sampleTriangularLoopInterior(const Loop3D& pin, std::vector<QVector3D>& pts, float density) {
	if (pin.size() == 3) {
		QVector3D tmpPt;
		QVector3D v1_minus_v0;
		QVector3D v2_minus_v0;				
		QVector3D crossP;

		v1_minus_v0 = pin.at(1) - pin.at(0);
		v2_minus_v0 = pin.at(2) - pin.at(0);

		//float loopArea = computeLoopArea(pin);
		float loopArea = 0.5f*(QVector3D::crossProduct(v1_minus_v0, v2_minus_v0).length());

		int numSamples = (int)(density*loopArea);

		float rand1, rand2;

		for (int i=0; i<numSamples; ++i) {
			rand1 = Util::genRand();
			rand2 = Util::genRand();
			if (rand1 + rand2 > 1.0f) {
				rand1 = 1.0 - rand1;
				rand2 = 1.0 - rand2;
			}

			tmpPt = pin.at(0) + rand1 * v1_minus_v0 + rand2 * v2_minus_v0;
			pts.push_back(tmpPt);
		}
	}
}

/**
* @brief: Get polygon axis aligned bounding box
* @return: The dimensions of the AABB 
**/
QVector3D Polygon3D::getLoopAABB(const Loop3D& pin, QVector3D& minCorner, QVector3D& maxCorner) {
	maxCorner = QVector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	minCorner = QVector3D( FLT_MAX,  FLT_MAX,  FLT_MAX);

	QVector3D curPt;

	for (int i = 0; i < pin.size(); ++i) {
		curPt = pin.at(i);
		if( curPt.x() > maxCorner.x() ){
			maxCorner.setX(curPt.x());
		}
		if( curPt.y() > maxCorner.y() ){
			maxCorner.setY(curPt.y());
		}
		if( curPt.z() > maxCorner.z() ){
			maxCorner.setZ(curPt.z());
		}
		//------------
		if( curPt.x() < minCorner.x() ){
			minCorner.setX(curPt.x());
		}
		if( curPt.y() < minCorner.y() ){
			minCorner.setY(curPt.y());
		}
		if( curPt.z() < minCorner.z() ){
			minCorner.setZ(curPt.z());
		}
	}
	return QVector3D(maxCorner - minCorner);
}

/**
* Get polygon oriented bounding box
**/
void Polygon3D::getLoopOBB(const Loop3D& pin, QVector3D& size, QMatrix4x4& xformMat) {
	float alpha = 0.0f;			
	float deltaAlpha = 0.05 * M_PI;
	float bestAlpha;

	Loop3D rotLoop;
	QMatrix4x4 rotMat;
	QVector3D minPt, maxPt;
	QVector3D origMidPt;
	QVector3D boxSz;
	QVector3D bestBoxSz;
	float curArea;
	float minArea = FLT_MAX;

	rotLoop = pin;
	Polygon3D::getLoopAABB(rotLoop, minPt, maxPt);
	origMidPt = 0.5f*(minPt + maxPt);

	int cSz = pin.size();
	QVector3D difVec;
	for (int i=0; i<pin.size(); ++i) {
		difVec = (pin.at((i+1) % cSz) - pin.at(i)).normalized();
		alpha = atan2(difVec.x(), difVec.y());
		rotMat.setToIdentity();				
		rotMat.rotate(Util::rad2deg(alpha), 0.0f, 0.0f, 1.0f);				

		transformLoop(pin, rotLoop, rotMat);
		boxSz = Polygon3D::getLoopAABB(rotLoop, minPt, maxPt);
		curArea = boxSz.x() * boxSz.y();
		if (curArea < minArea) {
			minArea = curArea;
			bestAlpha = alpha;
			bestBoxSz = boxSz;
		}
		//alpha += deltaAlpha;
	}

	xformMat.setToIdentity();											
	xformMat.rotate(Util::rad2deg(bestAlpha), 0.0f, 0.0f, 1.0f);
	xformMat.setRow(3, QVector4D(origMidPt.x(), origMidPt.y(), origMidPt.z(), 1.0f));			
	size = bestBoxSz;
}

void Polygon3D::getMyOBB(QVector3D& size, QMatrix4x4& xformMat) {
	Polygon3D::getLoopOBB(this->contour, size, xformMat);
}

void Polygon3D::extrudePolygon(const Loop3D& basePgon, float height, std::vector<Polygon3D>& pgonExtrusion) {	
	QVector3D zTransV(0.0f, 0.0f, height);
	int iNext;
	int pgonContourSz = basePgon.size();
	for (int i = 0; i < pgonContourSz; ++i) {
		iNext = (i+1) % pgonContourSz;

		//construct face
		Polygon3D tmpPgon1;
		Polygon3D tmpPgon2;

		tmpPgon1.contour.reserve(3); //pre allocate capacity for polygon contour
		tmpPgon1.contour.push_back(basePgon[i]);
		tmpPgon1.contour.push_back(basePgon[iNext]);
		tmpPgon1.contour.push_back(basePgon[iNext] + zTransV);

		tmpPgon2.contour.reserve(3); //pre allocate capacity for polygon contour
		tmpPgon2.contour.push_back(basePgon[i]);
		tmpPgon2.contour.push_back(basePgon[iNext] + zTransV);
		tmpPgon2.contour.push_back(basePgon[i] + zTransV);		

		//add two triangular faces to solid
		pgonExtrusion.push_back(tmpPgon1);
		pgonExtrusion.push_back(tmpPgon2);
	}
}

/**
 * Shortest distance from a point to a polygon
 */
float Polygon3D::distanceXYToPoint(const Loop3D& pin, const QVector3D& pt) {
	float minDist = FLT_MAX;
	float dist;
	int idxNext;

	for (size_t i = 0; i < pin.size(); ++i) {
		idxNext = (i+1) % (pin.size());
		dist = Util::pointSegmentDistanceXY(pin.at(i), pin.at(idxNext), pt);
		if (dist < minDist) {
			minDist = dist;
		}
	}
	return minDist;
}

/**
 * this function measures the minimum distance from the vertices of a contour A
 * to the edges of a contour B, i.e., it measures the distances from each vertex of A
 * to all the edges in B, and returns the minimum of such distances
 */
float Polygon3D::distanceXYfromContourAVerticesToContourB(const Loop3D& pA, const Loop3D& pB) {
	float minDist = FLT_MAX;
	float dist;

	for (size_t i = 0; i < pA.size(); ++i) {
		dist = Polygon3D::distanceXYToPoint(pB, pA.at(i));
		if (dist < minDist) {
			minDist = dist;
		}
	}
	return minDist;
}

