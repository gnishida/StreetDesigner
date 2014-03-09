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
#include "Polygon3D.h"

namespace ucore {

class Parcel;
class Terrain;

class Building : public GeometryObject {
protected:
	Polygon3D footprint;
	int stories;
	float height;
	float width;
	float depth;
	//QVector3D origin;

public:
	Building(const Loop3D& footprint, int stories, float height);
	~Building();

	/** getter/setter for footprint */
	const Polygon3D& getFootprint() const { return footprint; }
	Polygon3D& getFootprint() { return footprint; }

	/** getter/setter for stories */
	int getStories() { return stories; }
	void setStories(int stories) { this->stories = stories; }

	/** getter/setter for height */
	float getHeight() { return height; }
	void setHeight(float height) { this->height = height; }

	void adaptToTerrain(Terrain* terrain);
	void clear();

protected:
	void generateQuad(float x, float y, float z, float w, float d, float h);
};

} // namespace ucore