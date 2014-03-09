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

#include "InfoLayer.h"
#include "ColorTable.h"
#include <limits>

namespace ucore {

InfoLayer::InfoLayer(QString name, int width, int depth, ColorTable* colorTable) {
	this->name = name;
	this->width = width;
	this->depth = depth;
	this->maxValue = 0;
	this->minValue = 0;
	this->colorTable = colorTable;
}

InfoLayer::~InfoLayer() {
}

void InfoLayer::setValue(int x, int y, float value) {
	_setValue(x, y, value);
	
	if (value < minValue) minValue = value;
	if (value > maxValue) maxValue = value;
}

void InfoLayer::addValue(int x, int y, float change) {
	_addValue(x, y, change);

	float newValue = getValue(x, y);
	if (newValue < minValue) minValue = newValue;
	if (newValue > maxValue) maxValue = newValue;
}

} // namespace ucore