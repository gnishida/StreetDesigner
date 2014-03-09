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

#include <qstring.h>
#include <vector>

namespace ucore {

class InfoLayer;
class Terrain;

class InfoLayerManager {
protected:
	int selected;
	std::vector<InfoLayer*> infoLayers;
	Terrain* terrain;

protected:
	InfoLayerManager();
	~InfoLayerManager();
	InfoLayerManager(const InfoLayerManager& ref);
	InfoLayerManager& operator=(const InfoLayerManager& ref);

public:
	static InfoLayerManager* getInstance();
	InfoLayer* get(int index);
	InfoLayer* getSelected();
	void add(InfoLayer* infoLayer);
	void select(int index);
	size_t size();
	void clear();

	void setTerrain(Terrain* terrain);
	Terrain* getTerrain() { return terrain; }

	void addValue(int x, int y, float change);
};

} // namespace ucore