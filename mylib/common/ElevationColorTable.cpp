#include "ElevationColorTable.h"

namespace mylib {

ElevationColorTable::ElevationColorTable() {
	addRGBPoint(-100, 0, 0, 0);
	addRGBPoint(-20, 30, 30, 105);
	addRGBPoint(-1E-6, 255, 255, 255);
	addRGBPoint(0, 153, 118, 105);
	addRGBPoint(200, 205, 188, 182);
	addRGBPoint(3000, 255, 255, 255);
}

QColor ElevationColorTable::getColor(double value) {
	return getRGB(value);
}

} // namespace mylib