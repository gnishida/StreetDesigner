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

#include <qstring>

namespace ucore {

class ColorTable;

/**
 * InfoLayer is an abstract class which represents a layer and contains some information.
 * This abstract class does not provide implementation for how to store the information.
 * Therefore, each subclass should implement the actual storage for the information.
 */
class InfoLayer {
protected:
	QString name;
	int width;
	int depth;
	float maxValue;
	float minValue;
	ColorTable* colorTable;

public:
	InfoLayer(QString name, int width, int depth, ColorTable* colorTable);
	~InfoLayer();

	QString getName() { return name; }

	int getWidth() { return width; }
	int getDepth() { return depth; }

	ColorTable* getColorTable() { return colorTable; }

	float getMaxValue() { return maxValue; }
	float getMinValue() { return minValue; }

	void setValue(int x, int y, float value);
	void addValue(int x, int y, float change);

	virtual float getValue(int x, int y) = 0;

protected:
	virtual void _setValue(int x, int y, float value) = 0;
	virtual void _addValue(int x, int y, float change) = 0;
};

} // namespace ucore