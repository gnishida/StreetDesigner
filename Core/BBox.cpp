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

#include "BBox.h"
#include <limits>

namespace ucore {

BBox::BBox() {
	minPt.setX(std::numeric_limits<float>::max());
	minPt.setY(std::numeric_limits<float>::max());
	minPt.setZ(std::numeric_limits<float>::max());
	maxPt.setX(-std::numeric_limits<float>::max());
	maxPt.setY(-std::numeric_limits<float>::max());
	maxPt.setZ(-std::numeric_limits<float>::max());
}

BBox::~BBox() {
}

/**
 * update the bounding box by combining aother bounding box.
 *
 * @param other aother bounding box
 */
void BBox::combineWithBBox(const BBox& other) {	
	minPt.setX(qMin(minPt.x(), other.minPt.x()));
	minPt.setY(qMin(minPt.y(), other.minPt.y()));
	minPt.setZ(qMin(minPt.z(), other.minPt.z()));

	maxPt.setX(qMax(maxPt.x(), other.maxPt.x()));
	maxPt.setY(qMax(maxPt.y(), other.maxPt.y()));
	maxPt.setZ(qMax(maxPt.z(), other.maxPt.z()));
}

/**
 * update the bounding box by adding a new point.
 *
 * @param newPt new point
 */
void BBox::addPoint(const QVector3D& newPt) {
	minPt.setX(qMin(minPt.x(), newPt.x()));
	minPt.setY(qMin(minPt.y(), newPt.y()));
	minPt.setZ(qMin(minPt.z(), newPt.z()));

	maxPt.setX(qMax(maxPt.x(), newPt.x()));
	maxPt.setY(qMax(maxPt.y(), newPt.y()));
	maxPt.setZ(qMax(maxPt.z(), newPt.z()));
}

bool BBox::contains(const QVector3D &pt) const {
	if (pt.x() < minPt.x() || pt.y() < minPt.y() || pt.z() < minPt.z()) return false;
	if (pt.x() > maxPt.x() || pt.y() > maxPt.y() || pt.z() > maxPt.z()) return false;

	return true;
}

} // namespace ucore