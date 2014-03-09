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
#ifndef Q_MOC_RUN
#include <boost/graph/adjacency_list.hpp>
#endif
#include "GeometryObject.h"
#include "Polygon3D.h"
#include "BBox.h"
#include "Parcel.h"
#include <vector>

namespace ucore {

class Terrain;
class TextureManager;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Parcel*> ParcelGraph;				
typedef boost::graph_traits<ParcelGraph>::vertex_descriptor ParcelGraphVertexDesc;
typedef boost::graph_traits<ParcelGraph>::vertex_iterator ParcelGraphVertexIter;

class Block : public GeometryObject {
protected:
	/** Contour of the block */
	Polygon3D contour;

	/** Boundary road widths */
	std::vector<float> roadWidths;

	/** Bounding box */
	BBox bbox;

	/** BGL Graph of parcels into which block is subdivided */

	/** Parcels */
	ParcelGraph parcels;

	/** Random seed */
	int randSeed;

public:
	Block();
	~Block();
	
	/** getter/setter for contour */
	Polygon3D& getContour() { return contour; }
	void setContour(const Loop3D& contour);

	/** getter/setter for roadWidths */
	std::vector<float>& getRoadWidths() { return roadWidths; }
	void setRoadWidths(const std::vector<float>& roadWidths) { this->roadWidths = roadWidths; }

	/** getter/setter for bbox */
	BBox& getBBox() { return bbox; }

	/** getter/setter for parcels */
	ParcelGraph& getParcels() { return parcels; }
	Parcel* getParcel(ParcelGraphVertexDesc desc) { return parcels[desc]; }
	ParcelGraphVertexDesc addParcel(Parcel* parcel);

	/** gettter/setter for randSeed */
	inline int getRandSeed() { return randSeed; }
	inline void setRandSeed(int randSeed) { this->randSeed = randSeed; }

	/** Clear */
	void clear();
	void clearParcels();

	void adaptToTerrain(Terrain* terrain);

private:
	void _generateMeshVertices(TextureManager* textureManager);
};

} // namespace ucore