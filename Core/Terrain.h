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

#include "GeometryObject.h"
#include "InfoLayer.h"

namespace ucore {

class Terrain : public InfoLayer, public GeometryObject {
protected:
	/** The information layer which is shown on the top of the terrain */
	ucore::InfoLayer* infoLayer;

public:
	Terrain(int width, int depth);
	~Terrain();

	InfoLayer* getInfoLayer() { return infoLayer; }
	void setInfoLayer(InfoLayer* infoLayer);
};

} // namespace ucore