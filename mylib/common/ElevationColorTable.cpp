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

#include "ElevationColorTable.h"

namespace ucore {

ElevationColorTable::ElevationColorTable() {
	addRGBPoint(-100, 0, 0, 0);
	addRGBPoint(-20, 30, 30, 105);
	addRGBPoint(-1E-6, 255, 255, 255);
	addRGBPoint(0, 153, 118, 105);
	addRGBPoint(200, 205, 188, 182);
	addRGBPoint(3000, 255, 255, 255);
}

ElevationColorTable::~ElevationColorTable() {
}

QColor ElevationColorTable::getColor(double value) {
	return getRGB(value);
}

} // namespace ucore