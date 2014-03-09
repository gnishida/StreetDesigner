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

#include "InfoLayerManager.h"
#include "InfoLayer.h"
#include "Terrain.h"
#include <assert.h>

namespace ucore {

InfoLayerManager::InfoLayerManager() {
	selected = 0;
	terrain = NULL;
}

InfoLayerManager::~InfoLayerManager() {
	clear();
}

InfoLayerManager* InfoLayerManager::getInstance() {
	static InfoLayerManager instance;
	return &instance;
}

InfoLayer* InfoLayerManager::get(int index) {
	assert(index >= 0 && index < infoLayers.size());
	return infoLayers[index];
}

InfoLayer* InfoLayerManager::getSelected() {
	assert(selected >= 0 && selected < infoLayers.size());
	return infoLayers[selected];
}

void InfoLayerManager::add(InfoLayer* infoLayer) {
	infoLayers.push_back(infoLayer);
}

void InfoLayerManager::select(int selected) {
	this->selected = selected;
	
	if (terrain != NULL) {
		terrain->setInfoLayer(infoLayers[selected]);
		terrain->setModified();
	}
}

size_t InfoLayerManager::size() {
	return infoLayers.size();
}

void InfoLayerManager::clear() {
	for (int i = 0; i < infoLayers.size(); ++i) {
		delete infoLayers[i];
	}
	infoLayers.clear();
	selected = 0;
	terrain = NULL;
}

void InfoLayerManager::setTerrain(Terrain* terrain) {
	if (this->terrain != NULL) delete this->terrain;

	this->terrain = terrain;

	if (infoLayers.size() > 0) {
		infoLayers[0] = terrain;
	} else {
		infoLayers.push_back(terrain);
	}
}

void InfoLayerManager::addValue(int x, int y, float change) {
	InfoLayer* infoLayer = getSelected();
	infoLayer->addValue(x, y, change);

	if (terrain != NULL) {
		terrain->setModified();
	}
}

} // namespace ucore