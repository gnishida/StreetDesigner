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

#include "Util.h"
#include "Polygon3D.h"
#include <QGLWidget>
#include <qfile>
#include <qtextstream>

namespace ucore {

const float Util::MTC_FLOAT_TOL = 1e-6f;

Util::Util() {
}

Util::~Util() {
}

const QVector3D Util::perp(const QVector3D &v) {
	qreal min = fabs(v.x());
	QVector3D cardinalAxis(1.0, 0.0, 0.0);

	if(fabs(v.y()) < min) {
		min = fabs(v.y());
		cardinalAxis = QVector3D(0.0, 1.0, 0.0);
	}

	if(fabs(v.z()) < min) {
		cardinalAxis = QVector3D(0.0, 0.0, 1.0);
	}

	return QVector3D::crossProduct(v, cardinalAxis);
}

/** 
 * drawCone Funtion to draw a cone
 *  d--> axis vector of the cone (direction)
 *  a--> apex (end of cone)
 *  h--> height
 *  r--> radious
 *  n--> number of divisions
 */
void Util::drawCone(const QVector3D &d, const QVector3D &a, const qreal h, const qreal rd, const int n) {
	QVector3D c = a + (-d * h);
	QVector3D e0 = perp(d);
	QVector3D e1 = QVector3D::crossProduct(e0, d);
	qreal angInc = 360.0 / n * (M_PI/180.0f);//M_PI_DIV180;

	// draw cone top
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(a.x(), a.y(), a.z());
	for(int i = 0; i <= n; ++i) {
		qreal rad = angInc * i;
		QVector3D p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		glVertex3f(p.x(), p.y(), p.z());
	}
	glEnd();

	// draw cone bottom
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(c.x(), c.y(), c.z());
	for(int i = n-1; i >= 0; --i) {
		qreal rad = angInc * i;
		QVector3D p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
		glVertex3f(p.x(), p.y(), p.z());
	}
	glEnd();
}

bool Util::loadPolygonsFromFile(QString filename, std::vector<Polygon3D> &polygons) {
	std::vector<QVector3D> pts;
	std::vector< std::vector<int> > idxs;

	int numPgons;

	if (!readSegmentsFromFile(filename, pts, idxs)) {
		return false;
	} else {
		numPgons = idxs.size();
		if( (pts.size() < 1) || (numPgons < 1) ){
			std::cout << "ERROR reading file " << filename.toLocal8Bit().data() << ". Not enough points or polygons.\n";
			return false;
		}
	}			

	for (int i = 0; i < numPgons; ++i) {
		int numPts = (idxs[i].size())/2;				
		if (numPts < 3) {
			std::cout << "WARNING reading file " << filename.toLocal8Bit().data() << ". Polygon contains fewer than 2 points and has been ignored.\n";
			continue;
		}

		Polygon3D tmpPgon;
		QVector3D tmpPt;
		for (int j = 0; j < numPts; ++j) {
			tmpPt = pts[idxs[i][2*j]];
			tmpPgon.push_back(tmpPt);
		}

		polygons.push_back(tmpPgon);
	}

	return true;
}

/**
 * Returns only the first polygon in the file
 */
bool Util::loadPolygonFromFile(QString filename, Polygon3D &polygon) {
	std::vector<Polygon3D> tmpPolygons;
	if (!loadPolygonsFromFile(filename, tmpPolygons)) {
		return false;
	}

	if (tmpPolygons.size() < 1) {
		std::cout << "ERROR: file " << filename.toLocal8Bit().data() << " does not contain any polygons.\n";
		return false;
	}
	polygon = tmpPolygons.at(0);
	return true;
}

bool Util::readSegmentsFromFile(QString filename, std::vector<QVector3D>& pts, std::vector<std::vector<int> >& idxsSets) {
	//read initial edges from file
	QFile initialEdgesFile(filename);
	if (!initialEdgesFile.open(QIODevice::ReadOnly | QIODevice::Text))			{
		std::cout << "ERROR: Cannot open the file " << filename.toLocal8Bit().data() << " for reading\n";
		return false;
	}

	idxsSets.clear();
	pts.clear();

	QTextStream stream( &initialEdgesFile );

	QString line;

	int numVertices;
	int numEdges;
	int numObjects;

	//read number of objects
	line = stream.readLine();
	if(!line.isNull()){
		numObjects = line.toInt();
	}

	for(int objIdx = 0; objIdx < numObjects; ++objIdx){

		std::vector<int> tmpIdxVector;

		//read number of vertices
		line = stream.readLine();	
		if(!line.isNull()){
			numVertices = line.toInt();
		}

		//read number of edges
		line = stream.readLine();	
		if(!line.isNull()){
			numEdges = line.toInt();
		}			

		//read in the vertices	
		for(int i=0; i<numVertices; ++i){				
			line = stream.readLine();	
			if(!line.isNull()){
				QStringList attList = line.split(" ");
				if(attList.size() != 4){
					std::cout << "ERROR: Point entry must contain point index and three coordinates\n";
					continue;
				}

				int pointIdx = attList.at(0).toInt();

				QVector3D tmpPt;
				tmpPt = QVector3D(attList.at(1).toFloat(),
					attList.at(2).toFloat(),
					attList.at(3).toFloat());	

				pts.push_back(tmpPt);
			}
		}	

		//read in the edges
		int vtxAIdx, vtxBIdx;			
		for(int i=0; i<numEdges; ++i){				
			line = stream.readLine();	
			if(!line.isNull()){
				QStringList attList = line.split(" ");
				if(attList.size() != 2){
					std::cout << "ERROR: Edges must be defined by two vertex indexes\n";
					continue;
				}			
				vtxAIdx = attList.at(0).toInt();
				vtxBIdx = attList.at(1).toInt();					

				tmpIdxVector.push_back(vtxAIdx);
				tmpIdxVector.push_back(vtxBIdx);
			}
		}	

		if(tmpIdxVector.size()%2 != 0){
			std::cout << "ERROR reading file " << filename.toLocal8Bit().data() << ". Number of segment indexes must be even.\n";
			return false;
		}

		idxsSets.push_back(tmpIdxVector);
	}

	return true;
}


bool Util::isIdxWithinBMatrix(int r, int c, tBoostMatrix &m) {
	return (r < m.size1() && c < m.size2() && r>=0 && c>=0);
}

float Util::deg2rad(float deg) {
	return (deg * M_PI) / 180.0f;
}

float Util::rad2deg(float rad) {
	return ((rad*180.0f) / M_PI);
}

double Util::sumElementsinRow(int r, const tBoostMatrix &m) {
	assert(r < m.size1());
	double sum = 0.0f;

	for (int i = 0; i < m.size2(); ++i) {
		sum += m(r, i);
	}
	return sum;
}

double Util::sumElementsinColumn(int c, const tBoostMatrix &m) {
	assert(c < m.size2());
	double sum = 0.0f;

	for (int i = 0; i < m.size1(); ++i) {
		sum += m(i, c);
	}
	return sum;
}

void Util::printBMatrix(const tBoostMatrix& m) {	
	for (int i = 0; i < m.size1(); ++i) {				
		for (int j = 0; j < m.size2(); ++j) {
			std::cout << m(i, j) << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void Util::initBMatrix(tBoostMatrix &m) {
	for (int i = 0; i < m.size1(); ++i) {
		for (int j = 0; j < m.size2(); ++j) {
			m(i, j) = 0;
		}
	}
}

/**
* Returns index of the point in pointArray that is closest to point
**/
float Util::getClosestPointInArray(const std::vector<QVector3D>& pointArray, const QVector3D& point, int &idx) {
	idx = -1;
	float minDist = FLT_MAX;
	float curDist;
	for (int i = 0; i < pointArray.size(); ++i) {
		curDist = (point-pointArray[i]).lengthSquared();
		if (curDist < minDist) {
			minDist = curDist;
			idx = i;
		}
	}
	return sqrt(minDist);
}

QVector3D Util::calculateNormal(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2) {
	return QVector3D::normal((p1-p0),(p2-p1));
}

/**
 * Distance from segment ab to point c
 */
float Util::pointSegmentDistanceXY(const QVector3D& a, const QVector3D& b, const QVector3D& c) {
	QVector3D closestPt;
	return pointSegmentDistanceXY(a, b, c, closestPt);
}

float Util::pointSegmentDistanceXY(const QVector3D& a, const QVector3D& b, const QVector3D& c, QVector3D& closestPtInAB) {
	float dist;		

	float r_numerator = (c.x()-a.x())*(b.x()-a.x()) + (c.y()-a.y())*(b.y()-a.y());
	float r_denomenator = (b.x()-a.x())*(b.x()-a.x()) + (b.y()-a.y())*(b.y()-a.y());
	float r = r_numerator / r_denomenator;
	//
	float px = a.x() + r*(b.x()-a.x());
	float py = a.y() + r*(b.y()-a.y());
	//    
	float s =  ((a.y()-c.y())*(b.x()-a.x())-(a.x()-c.x())*(b.y()-a.y()) ) / r_denomenator;

	float distanceLine = fabs(s)*sqrt(r_denomenator);

	// (xx,yy) is the point on the lineSegment closest to (cx,cy)
	closestPtInAB.setX(px);
	closestPtInAB.setY(py);
	closestPtInAB.setZ(0.0f);

	if ((r >= 0) && (r <= 1)) {
		dist = distanceLine;
	} else {
		float dist1 = (c.x()-a.x())*(c.x()-a.x()) + (c.y()-a.y())*(c.y()-a.y());
		float dist2 = (c.x()-b.x())*(c.x()-b.x()) + (c.y()-b.y())*(c.y()-b.y());
		if (dist1 < dist2) {	
			dist = sqrt(dist1);
		} else {
			dist = sqrt(dist2);
		}
	}

	return abs(dist);
}

/**
 * Computes the intersection between two line segments on the XY plane.
 * Segments must intersect within their extents for the intersection to be valid. z coordinate is ignored.
 *
 * @param a one end of the first line
 * @param b another end of the first line
 * @param c one end of the second line
 * @param d another end of the second line
 * @param tab
 * @param tcd
 * @param segmentOnly
 * @param intPoint	the intersection
 * @return true if two lines intersect / false otherwise
 **/
bool Util::segmentSegmentIntersectXY(const QVector2D& a, const QVector2D& b, const QVector2D& c, const QVector2D& d, float *tab, float *tcd, bool segmentOnly, QVector2D& intPoint) {
	QVector2D u = b - a;
	QVector2D v = d - c;

	if (u.lengthSquared() < MTC_FLOAT_TOL || v.lengthSquared() < MTC_FLOAT_TOL) {
		return false;
	}

	float numer = v.x()*(c.y()-a.y()) + v.y()*(a.x()-c.x());
	float denom = u.y()*v.x() - u.x()*v.y();

	if (denom == 0.0f)  {
		// they are parallel
		*tab = 0.0f;
		*tcd = 0.0f;
		return false;
	}

	float t0 = numer / denom;

	QVector2D ipt = a + t0*u;
	QVector2D tmp = ipt - c;
	float t1;
	if (QVector2D::dotProduct(tmp, v) > 0.0f){
		t1 = tmp.length() / v.length();
	}
	else {
		t1 = -1.0f * tmp.length() / v.length();
	}

	//Check if intersection is within segments
	if(segmentOnly && !( (t0 >= MTC_FLOAT_TOL) && (t0 <= 1.0f-MTC_FLOAT_TOL) && (t1 >= MTC_FLOAT_TOL) && (t1 <= 1.0f-MTC_FLOAT_TOL) ) ){
		return false;
	}

	*tab = t0;
	*tcd = t1;
	QVector2D dirVec = b-a;

	intPoint = a+(*tab)*dirVec;
	return true;
}

bool Util::rayTriangleIntersect(const QVector3D& rayPivot, const QVector3D& rayDirection, const QVector3D& p0, const QVector3D& p1, const QVector3D& p2) {
	QVector3D intPt;			
	return rayTriangleIntersect(rayPivot, rayDirection, p0, p1, p2, intPt);
}

bool Util::rayTriangleIntersect(const QVector3D& rayPivot, const QVector3D& rayDirection, const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, QVector3D& intPt) {
	double t;
	QVector3D n = QVector3D::crossProduct(p1 - p0, p2 - p1);
	QVector3D p3 = rayPivot + rayDirection;
	return planeIntersectWithLine(rayPivot, p3, n, p0, t, intPt);
}

/**
 * Check the intersection between a line segment and a plane.
 * The line segment is specified by two points, p1 and p2, while the plane is specified by a point p0 on the plane and the normal vector n.
 *
 * @param p1	the first point to specify the line segment
 * @param p2	the second point to specify the line segment
 * @param n		the normal vector of the plane
 * @param p0	a point on the plane
 * @param t		the ratio of the length between the first point of the line segment and the intersection to the length of the line segment. In other words, if this value is greater than or equal to zero and less than or equal to one, the intersection is within the line segment. Otherwise, the intersection is out of the line segment.
 * @param x		the intersection point
 * @return		true if the line segment and the plane intersect with each other. Otherwise, false.
 */
bool Util::planeIntersectWithLine(const QVector3D& p1, const QVector3D& p2, const QVector3D& n, const QVector3D& p0, double& t, QVector3D &x) {
	double num, den, p21[3];
	double fabsden, fabstolerance;

	// Compute line vector
	p21[0] = p2.x() - p1.x();
	p21[1] = p2.y() - p1.y();
	p21[2] = p2.z() - p1.z();

	// Compute denominator.  If ~0, line and plane are parallel.
	num = QVector3D::dotProduct(n, p0 - p1);
	den = n.x() * p21[0] + n.y() * p21[1] + n.z() * p21[2];

	// If denominator with respect to numerator is "zero", then the line and
	// plane are considered parallel.

	// trying to avoid an expensive call to fabs()
	if (den < 0.0)
		fabsden = -den;
	else
		fabsden = den;

	if (num < 0.0)
		fabstolerance = -num * 1.0e-06;
	else
		fabstolerance = num * 1.0e-06;
	if (fabsden <= fabstolerance) {
		t = DBL_MAX;
		return false;
	}

	// valid intersection
	t = num / den;

	x.setX(p1.x() + t * p21[0]);
	x.setY(p1.y() + t * p21[1]);
	x.setZ(p1.z() + t * p21[2]);

	if (t >= 0.0 && t <= 1.0) return true;
	else return false;
}

/**
 * Angle between 3 points A-B-C
 **/
float Util::angleThreePoints(const QVector3D& pa, const QVector3D& pb, const QVector3D& pc) {
	float a = (pb - pc).length();
	float b = (pa - pc).length();
	float c = (pa - pb).length();
	return acos(0.999f * (a*a + c*c - b*b) / (2.0f*a*c));
}

/**
 * Random numbers.		
 */
float Util::genRand(void) {
	return rand() / (float(RAND_MAX) + 1);
}

float Util::genRand(float a, float b) {
	return genRand() * (b - a) + a;
}

float Util::genRandNormal(float mean, float variance) {
	float m = mean;
	float s = sqrt(variance);

	/* mean m, standard deviation s */
	float x1, x2, w, y1;
	static float y2;
	static int use_last = 0;

	if (use_last) {	/* use value from previous call */
		y1 = y2;
		use_last = 0;
	} else {
		do {
			x1 = 2.0 * genRand(0.0f, 1.0f) - 1.0;
			x2 = 2.0 * genRand(0.0f, 1.0f) - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return m + y1 * s;
}

/**
 * Render circle parallel to XY plane, centered in X, Y, Z, and with radius r
 */
void Util::renderCircle(float x, float y, float z, float radius) {	
	int circle_points = 100;
	float angle;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			

	glBegin(GL_POLYGON);		
	for (int i = 0; i < circle_points; ++i) {    
		angle = 2 * M_PI * i / circle_points; 
		glVertex3f(x+radius*cos(angle), y+radius*sin(angle), z); 
	}	
	glEnd();	

	glLineWidth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Util::renderPolyline(const Loop3D& loopIn, bool closed = true) {
	if (closed) {
		glBegin(GL_LINE_LOOP);
	} else {
		glBegin(GL_LINE_STRIP);
	}

	for (int i = 0; i < loopIn.size(); ++i) {
		glVertex3f(loopIn[i].x(), loopIn[i].y(), loopIn[i].z());
	}			
	glEnd();	
}

/**
 * computes the area of a 3D planar polygon.
 *
 * @param pin the vertices of the polygon
 * @return	the area of the polygon
 */
float Util::area3D_Polygon(const Loop3D& pin) {
	int n = pin.size();

	if (n < 3) {
		return 0.0f;
	}

	QVector3D normVec = Polygon3D::getLoopNormalVector(pin);

	float area = 0.0f;
	float an, ax, ay, az;  // abs value of normal and its coords
	int   coord;           // coord to ignore: 1=x, 2=y, 3=z
	int   i, j, k;         // loop indices

	// select largest abs coordinate to ignore for projection
	ax = (normVec.x()>0 ? normVec.x() : -normVec.x());     // abs x-coord
	ay = (normVec.y()>0 ? normVec.y() : -normVec.y());     // abs y-coord
	az = (normVec.z()>0 ? normVec.z() : -normVec.z());     // abs z-coord

	coord = 3;                     // ignore z-coord
	if (ax > ay) {
		if (ax > az) coord = 1;    // ignore x-coord
	} else if (ay > az) {
		coord = 2;					// ignore y-coord
	}

	// compute area of the 2D projection
	//for (i=1, j=2, k=0; i<=n; i++, j++, k++)
	for (k = 0; k < n; ++k) {
		i = (k+1) % n;
		j = (k+2) % n;
		switch (coord) {
		case 1:
			area += (pin[i].y() * (pin[j].z() - pin[k].z()));
			continue;
		case 2:
			area += (pin[i].x() * (pin[j].z() - pin[k].z()));
			continue;
		case 3:
			area += (pin[i].x() * (pin[j].y() - pin[k].y()));
			continue;
		}
	}

	// scale to get area before projection
	an = sqrt( ax*ax + ay*ay + az*az);  // length of normal vector
	switch (coord) {
	case 1:
		area *= (an / (2*ax));
		break;
	case 2:
		area *= (an / (2*ay));
		break;
	case 3:
		area *= (an / (2*az));
	}
	return fabs(area);
}

bool Util::isPointWithinLoop(const std::vector<QVector3D>& loop, const QVector3D& pt) {
	int i, j, c = 0;
	for (i = 0, j = loop.size()-1; i < loop.size(); j = ++i) {
		if ( ((loop[i].y()>pt.y()) != (loop[j].y()>pt.y())) &&
			(pt.x() < (loop[j].x()-loop[i].x()) * (pt.y()-loop[i].y()) / (loop[j].y()-loop[i].y()) + loop[i].x()) )
			c = !c;
	}
	return c;
}

/**
* Checks if contour A is within contour B
**/
bool Util::is2DRingWithin2DRing(const boost::geometry::ring_type<Polygon3D>::type& contourA, const boost::geometry::ring_type<Polygon3D>::type& contourB) {
	for (int i = 0; i < contourA.size(); ++i) {
		if (!boost::geometry::within(contourA[i], contourB)) {
			return false;
		}
	}
	return true;
}

double Util::angleBetweenVectors(const QVector3D &vec1, const QVector3D &vec2) {	
	return acos( 0.999*(QVector3D::dotProduct(vec1, vec2)) / ( vec1.length() * vec2.length() ) );
}

/**
* Given three non colinear points p0, p1, p2, this function computes
* the intersection between the lines A and B. Line A is the line parallel to the segment p0-p1
* and at a distance d01 from that segment. Line B is the line parallel to the segment
* p1-p2 at a distance d12 from that segment.
* Returns true if point is successfully computed
**/
bool Util::getIrregularBisector(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, float d01, float d12, QVector3D& intPt) {
	double alpha;
	double theta;
	double L;

	QVector3D p1_p0;
	QVector3D p1_p2;
	QVector3D p1_p2_perp;
	QVector3D crossP;

	p1_p0 = p0 - p1;
	p1_p0.setZ(0.0f);

	p1_p2 = p2 - p1;
	p1_p2.setZ(0.0f);

	p1_p2_perp.setX( -p1_p2.y() );
	p1_p2_perp.setY(  p1_p2.x() );
	p1_p2_perp.setZ( 0.0f );

	alpha = angleBetweenVectors(p1_p0, p1_p2);				

	if (!(alpha == alpha)) {
		return false;
	}				

	theta = atan2( sin(alpha), (d01 / d12) + cos(alpha) );				
	L = d12 / sin(theta);

	//This is done to handle convex vs. concave angles in polygon
	crossP = QVector3D::crossProduct(p1_p2, p1_p0);

	if (crossP.z() > 0) {
		intPt = p1 + (p1_p2.normalized())*L*cos(theta) + (p1_p2_perp.normalized())*d12;
	} else {
		intPt = p1 - (p1_p2.normalized()) * L * cos(theta) + (p1_p2_perp.normalized()) * d12;
	}
	return true;
}


float Util::barycentricInterpolation(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2, const QVector2D& p) {
	QVector3D vec1 = p1 - p0;
	QVector3D vec2 = p2 - p0;
	QVector2D vec = p - QVector2D(p0.x(), p0.y());

	float s, t;
	float den = vec1.x() * vec2.y() - vec2.x() * vec1.y();

	if (den >= MTC_FLOAT_TOL) {
		s = (vec.x() * vec2.y() - vec2.x() * vec.y()) / den;
		t = (vec1.x() * vec.y() - vec.x() * vec1.y()) / den;
		return p0.z() + vec1.z() * s + vec2.z() * t;
	}

	return 0.0f;
}

/**
 * Convert Colors from HSV to RGB.
 * cs.rit.edu/~ncs/color/t_convert.html
 * r,g,b values are from 0 to 1
 * h = [0,360], s = [0,1], v = [0,1]
 *		if s == 0, then h = -1 (undefined)
 */
void Util::HSVtoRGB(float *r, float *g, float *b, float h, float s, float v) {
	int i;
	float f, p, q, t;
	if (s == 0) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch (i) {
	case 0:
		*r = v;
		*g = t;
		*b = p;
		break;
	case 1:
		*r = q;
		*g = v;
		*b = p;
		break;
	case 2:
		*r = p;
		*g = v;
		*b = t;
		break;
	case 3:
		*r = p;
		*g = q;
		*b = v;
		break;
	case 4:
		*r = t;
		*g = p;
		*b = v;
		break;
	default:		// case 5:
		*r = v;
		*g = p;
		*b = q;
		break;
	}
}

} // namespace ucore