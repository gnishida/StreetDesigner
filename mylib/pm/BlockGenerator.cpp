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

#include "BlockGenerator.h"
#include "Block.h"
#include "RoadGraph.h"
#include "UrbanGeometry.h"
#include "Util.h"

#ifndef Q_MOC_RUN
#include <boost/graph/planar_face_traversal.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#endif

namespace ucore {

BlockGenerator::BlockGenerator(UrbanGeometry* urbanGeometry) {
	this->urbanGeometry = urbanGeometry;
}

BlockGenerator::~BlockGenerator() {
}

} // namespace ucore