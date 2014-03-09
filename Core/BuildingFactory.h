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

#include "Polygon3D.h"

namespace ucore {

class Building;

class BuildingFactory {
public:
	static enum { TYPE_SIMPLE = 0, TYPE_MTC };

protected:
	BuildingFactory();
	~BuildingFactory();
	BuildingFactory(const BuildingFactory& ref);
	BuildingFactory& operator=(const BuildingFactory& ref);

public:
	virtual Building* createBuilding(const Loop3D& footprint, int numStories, float height, int type = 0) = 0;
};

} // namespace ucore