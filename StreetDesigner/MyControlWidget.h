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

#include <QtGui/QMainWindow>
#include "../Core/ControlWidget.h"
#include "ui_MyControlWidget.h"

class ucore::UrbanMain;
class ucore::UrbanGeometry;

namespace US {

class MyControlWidget : public ucore::ControlWidget {
	Q_OBJECT

private:
	Ui::MyControlWidget ui;

public:
	MyControlWidget(MainWindow* mainWin, ucore::UrbanGeometry* urbanGeometry);
	QString getName();

public slots:
	//void showElevation(bool flag);
	void showTerrain(bool flag);
	void showRoads(bool flag);
	void showBlocks(bool flag);
	void showParcels(bool flag);
	void showBuildings(bool flag);
	void showTrees(bool flag);
	void generateAll();
	void newCity();
	void saveCity();
	void loadCity();

private:
	void disableAllButtons();
	void enableAllButtons();
};

} // namespace US