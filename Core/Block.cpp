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

#include "Block.h"
#include "Terrain.h"
#include "Renderable.h"
#include "RenderableConcave.h"

namespace ucore {

Block::Block() : GeometryObject() {
	color = QColor(255, 0, 0);
}

Block::~Block() {
	clear();
}

void Block::setContour(const Loop3D& contour) {
	this->contour.setContour(contour);
	bbox.recalculate(contour);
	setModified();
}

ParcelGraphVertexDesc Block::addParcel(Parcel* parcel) {
	ParcelGraphVertexDesc desc;
	desc = boost::add_vertex(parcels);
	parcels[desc] = parcel;

	return desc;
}

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
	ParcelGraphVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(parcels); vi != vend; ++vi) {
		delete parcels[*vi];
	}
	parcels.clear();
}

void Block::adaptToTerrain(Terrain* terrain) {
	for (int i = 0; i < contour.size(); ++i) {
		contour[i].setZ(terrain->getValue(contour[i].x(), contour[i].y()));
	}

	bbox.recalculate(contour.getContour());

	ParcelGraphVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(parcels); vi != vend; ++vi) {
		parcels[*vi]->adaptToTerrain(terrain);
	}

	setModified();
}

void Block::_generateMeshVertices(TextureManager* textureManager) {
	if (contour.size() < 3) return;

	renderables.push_back(new RenderableConcave(contour.getContour(), color));
}

} // namespace ucore