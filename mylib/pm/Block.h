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

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include "../render/GeometryObject.h"
#include "../render/Terrain.h"
#include "../render/TextureManager.h"
#include "../common/Polygon3D.h"
#include "../common/BBox.h"
//#include "Parcel.h"

/*typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Parcel*> ParcelGraph;				
typedef boost::graph_traits<ParcelGraph>::vertex_descriptor ParcelGraphVertexDesc;
typedef boost::graph_traits<ParcelGraph>::vertex_iterator ParcelGraphVertexIter;
*/

class Block : public mylib::GeometryObject {
protected:
	/** Contour of the block */
	Polygon3D contour;

	/** Boundary road widths */
	std::vector<float> roadWidths;

	/** Bounding box */
	BBox bbox;

	/** BGL Graph of parcels into which block is subdivided */

	/** Parcels */
	//ParcelGraph parcels;

	/** Random seed */
	//int randSeed;

public:
	Block();
	~Block();
	
	/** getter/setter for contour */
	Polygon3D& getContour() { return contour; }
	void setContour(const Polygon3D& contour);

	/** getter/setter for roadWidths */
	std::vector<float>& getRoadWidths() { return roadWidths; }
	void setRoadWidths(const std::vector<float>& roadWidths) { this->roadWidths = roadWidths; }

	/** getter/setter for bbox */
	BBox getBBox() { return contour.envelope(); }

	/** getter/setter for parcels */
	//ParcelGraph& getParcels() { return parcels; }
	//Parcel* getParcel(ParcelGraphVertexDesc desc) { return parcels[desc]; }
	//ParcelGraphVertexDesc addParcel(Parcel* parcel);

	/** Clear */
	void clear();
	void clearParcels();

	void adaptToTerrain(mylib::Terrain* terrain);

private:
	void _generateMeshVertices(mylib::TextureManager* textureManager);
};
