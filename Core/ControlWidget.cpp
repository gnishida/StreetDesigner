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

#include "ControlWidget.h"
#include <QtOpenGL>
#include "UrbanMain.h"

namespace ucore {

ControlWidget::ControlWidget(const QString& title, UrbanMain* urbanMain, UrbanGeometry* urbanGeometry) : QDockWidget(title, (QWidget*)urbanMain) {
	this->urbanMain = urbanMain;
	this->urbanGeometry = urbanGeometry;

	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	layout->addWidget(new QPushButton(tr("test")));
}

ControlWidget::~ControlWidget(void) {
}

void ControlWidget::showControlWindow() {
	show();
	urbanMain->addDockWidget(Qt::LeftDockWidgetArea, this);
}

void ControlWidget::hideControlWindow() {
	hide();
}

} // namespace ucore