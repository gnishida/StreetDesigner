#include "RoadArea.h"

RoadArea::RoadArea() {
}

RoadArea::RoadArea(const Polygon2D &area) {
	this->area = area;
}

void RoadArea::clear() {
	roads.clear();
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、グリッド特徴量を設定する。
 */
void RoadArea::load(QDomNode& node) {
	area.clear();
	roads.clear();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "point") {
			QVector2D pt(child.toElement().attribute("x").toFloat(), child.toElement().attribute("y").toFloat());
			area.push_back(pt);
		}

		child = child.nextSibling();
	}
}

void RoadArea::save(QDomDocument& doc, QDomNode& parent) {
	QDomElement node_area = doc.createElement("area");
	parent.appendChild(node_area);

	for (int i = 0; i < area.size(); ++i) {
		QDomElement node_point = doc.createElement("point");
		node_point.setAttribute("x", area[i].x());
		node_point.setAttribute("y", area[i].y());
		node_area.appendChild(node_point);
	}
}

void RoadArea::adaptToTerrain(mylib::Terrain* terrain) {
	area3D.clear();

	for (int i = 0; i < area.size(); ++i) {
		float z = terrain->getValue(area[i].x(), area[i].y());
		area3D.push_back(QVector3D(area[i].x(), area[i].y(), z + 100));
	}
}
