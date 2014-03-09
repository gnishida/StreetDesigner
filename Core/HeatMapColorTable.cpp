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

#include "HeatMapColorTable.h"
#include <algorithm>

namespace ucore {

HeatMapColorTable::HeatMapColorTable(float minValue, float maxValue) : ColorTable() {
	this->maxValue = maxValue;
	this->minValue = minValue;
}

HeatMapColorTable::~HeatMapColorTable() {
}

QColor HeatMapColorTable::getColor(double value) {
	double t;
	if (scale == ColorTable::SCALE_LINEAR) {
		t = (value - minValue) / (maxValue - minValue);
	} else {
		t = log(value - minValue) / log(maxValue - minValue);
	}	

	if (t < 0.0) t = 0.0;
	else if (t > 1.0) t = 1.0;

	const float dx = 0.8f;

	double v = (6 - 2 * dx) * t + dx;
	double r = std::max(0.0, (3 - fabs(v - 4) - fabs(v - 5)) / 2);
	double g = std::max(0.0, (4 - fabs(v - 2) - fabs(v - 4)) / 2);
	double b = std::max(0.0, (3 - fabs(v - 1) - fabs(v - 2)) / 2);	
	return QColor(r * 255, g * 255, b * 255);
}

} // namespace ucore