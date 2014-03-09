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

#include "../Core/Polygon3D.h"
#include "../Core/Block.h"
#include <QThread.h>
#include <QMatrix4x4>



class MainWindow;
class MyUrbanGeometry;

class MyBuildingGenerator : public QThread {
protected:
	MainWindow* mainWin;
	MyUrbanGeometry* urbanGeometry;

public:
	MyBuildingGenerator(MainWindow* mainWin, MyUrbanGeometry* urbanGeometry);
	~MyBuildingGenerator();

	void run();

private:
	bool generateParcelBuildings(ucore::Block* block, ucore::Parcel* parcel);
	bool generateBlockBuildings(ucore::Block* block);
	bool generateBuildingBase(float width, float depth, ucore::Loop3D& contour, QMatrix4x4& mat, ucore::Loop3D* outBuildingBase);
};
