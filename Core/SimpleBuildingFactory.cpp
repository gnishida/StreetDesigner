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

#include "SimpleBuildingFactory.h"
#include "SimpleBuilding.h"

namespace ucore {

SimpleBuildingFactory::SimpleBuildingFactory() {
}

SimpleBuildingFactory::~SimpleBuildingFactory() {
}

BuildingFactory* SimpleBuildingFactory::getInstance() {
	static SimpleBuildingFactory instance;
	return &instance;
}

Building* SimpleBuildingFactory::createBuilding(const Loop3D& footprint, int numStories, float height, int type) {
	switch (type) {
	case TYPE_SIMPLE:
		return new SimpleBuilding(footprint, numStories, height);
	default:
		return new SimpleBuilding(footprint, numStories, height);
	}

	return NULL;
}

} // namespace ucore