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

#include "ParametricBuildingFactory.h"
#include "School.h"
//#include "IHFawley.h"
#include "StripMall.h"

namespace pb {

ParametricBuildingFactory::ParametricBuildingFactory() {
}

ParametricBuildingFactory::~ParametricBuildingFactory() {
}

ucore::BuildingFactory* ParametricBuildingFactory::getInstance() {
	static ParametricBuildingFactory instance;
	return &instance;
}

ucore::Building* ParametricBuildingFactory::createBuilding(const ucore::Loop3D& footprint, int stories, float height, int type) {
	switch (type) {
	case TYPE_SCHOOL:
		return new School(footprint, stories, height);
	/*case TYPE_IHFAWLEY:
		return new IHFawley(footprint, stories, height);*/
	case TYPE_STRIP_MALL:
		return new StripMall(footprint, stories, height);
	default:
		return new School(footprint, stories, height);
	}

	return NULL;
}

} // namespace pb