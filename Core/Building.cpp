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

#include "Building.h"
#include "Terrain.h"
#include <assert.h>

namespace ucore {

Building::Building(const Loop3D& footprint, int stories, float height) : GeometryObject(footprint[0]) {
	assert(footprint.size() >= 3);

	this->footprint.setContour(footprint);
	this->stories = stories;
	this->height = height;
	//elevation = 0;

	int colorF = qrand()%100+20;
	this->color = QColor(colorF, colorF, colorF);

	// These are for parametric buildings and should be revised after refining this class.
	rotateZ = atan2((footprint[1] - footprint[0]).y(), (footprint[1] - footprint[0]).x());
	//origin = footprint[0];
	width = (footprint[1] - footprint[0]).length();
	depth = (footprint[footprint.size() - 1] - footprint[0]).length();
}

Building::~Building() {
}

void Building::clear() {
	stories = 0;
	height = 0;
	footprint.clear();
}

void Building::adaptToTerrain(Terrain* terrain) {
	float elevation = (std::numeric_limits<float>::max)();

	for (int i = 0; i < footprint.size(); ++i) {
		float e = terrain->getValue(footprint[i].x(), footprint[i].y());
		if (e < elevation) elevation = e;
	}

	pt.setZ(elevation);
	//origin.setZ(elevation);

	setModified();
}

} // namespace ucore