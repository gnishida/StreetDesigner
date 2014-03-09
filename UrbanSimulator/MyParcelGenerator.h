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

#include "../Core/Block.h"

class MainWindow;
class MyUrbanGeometry;

class MyParcelGenerator {
protected:
	MainWindow* mainWin;
	MyUrbanGeometry* urbanGeometry;

public:
	MyParcelGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry);
	~MyParcelGenerator();

	void run();

private:
	void subdivideBlockIntoParcels(ucore::Block* block);
	bool subdivideParcel(ucore::Block* block, ucore::Polygon3D& contour, float areaMean, float areaStd, float splitIrregularity, std::vector<ucore::Parcel*>& outParcels);
	void setParcelsAsParks(std::vector<ucore::Block*>& blocks);
};

