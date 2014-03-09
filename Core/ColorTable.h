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

#include <qcolor.h>
#include <vector>

namespace ucore {

class ColorTableEntry {
protected:
	float value;
	QColor color;

public:
	ColorTableEntry(double value, int r, int g, int b);
	~ColorTableEntry();
	float getValue();
	QColor getColor();
};

class ColorTable {
public:
	static enum {SCALE_LINEAR = 0, SCALE_LOG10};

protected:
	std::vector<ColorTableEntry> entries;
	int scale;

public:
	ColorTable();
	~ColorTable();

	void setScale(int scale);
	void addRGBPoint(double value, int r, int g, int b);
	virtual QColor getColor(double value) = 0;

protected:
	QColor getRGB(double value);
};

} // namespace ucore