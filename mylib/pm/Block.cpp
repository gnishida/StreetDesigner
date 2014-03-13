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

#include "../render/Terrain.h"
#include "../render/Renderable.h"
#include "../render/RenderableConcave.h"
#include "Block.h"

namespace pm {

Block::Block() : GeometryObject() {
	color = QColor(255, 255, 128);
}

Block::~Block() {
	clear();
}

void Block::setContour(const Polygon3D& contour) {
	this->contour = contour;

	setModified();
}

/*ParcelGraphVertexDesc Block::addParcel(Parcel* parcel) {
	ParcelGraphVertexDesc desc;
	desc = boost::add_vertex(parcels);
	parcels[desc] = parcel;

	return desc;
}*/

/**
 * Clear
 */
void Block::clear() {
	contour.clear();
	roadWidths.clear();
	clearParcels();
	setModified();
}

void Block::clearParcels() {
	/*
	ParcelGraphVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(parcels); vi != vend; ++vi) {
		delete parcels[*vi];
	}
	parcels.clear();
	*/
}

void Block::adaptToTerrain(mylib::Terrain* terrain) {
	for (int i = 0; i < contour.size(); ++i) {
		float z = terrain->getValue(contour[i].x(), contour[i].y());
		contour[i].setZ(z + 5.0f);
	}

	/*
	ParcelGraphVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(parcels); vi != vend; ++vi) {
		parcels[*vi]->adaptToTerrain(terrain);
	}
	*/

	setModified();
}

void Block::_generateMeshVertices(mylib::TextureManager* textureManager) {
	if (contour.size() < 3) return;

	renderables.push_back(new mylib::RenderableConcave(contour, color));
}

} // namespace pm