#include "AbstractFeature.h"

AbstractFeature::AbstractFeature() {
	_weight = 0.0f;
}

int AbstractFeature::type() const {
	return _type;
}

const QVector2D& AbstractFeature::center() const {
	return _center;
}

void AbstractFeature::setCenter(const QVector2D &center) {
	_center = center;
}

float AbstractFeature::weight() const {
	return _weight;
}

void AbstractFeature::setWeight(float weight) {
	_weight = weight;
}

const Polygon2D& AbstractFeature::area() const {
	return _area;
}

void AbstractFeature::setArea(const Polygon2D &area) {
	_area.clear();

	QVector2D center = area.centroid();
	for (int i = 0; i < area.size(); ++i) {
		_area.push_back(area[i] - center);
	}
}

/**
 * Read an center node to the specified Dom document under the parent node.
 */
void AbstractFeature::loadCenter(QDomNode &node) {
	_center.setX(node.toElement().attribute("x").toFloat());
	_center.setY(node.toElement().attribute("x").toFloat());
}

/**
 * Read an area node to the specified Dom document under the parent node.
 */
void AbstractFeature::loadArea(QDomNode &node) {
	_area.clear();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "point") {
			_area.push_back(QVector2D(child.toElement().attribute("x").toFloat(), child.toElement().attribute("y").toFloat()));
		}

		child = child.nextSibling();
	}
}

/**
 * Write an center node to the specified Dom document under the parent node.
 */
void AbstractFeature::saveCenter(QDomDocument &doc, QDomNode &parent) {
	QDomElement node_center = doc.createElement("center");
	node_center.setAttribute("x", _center.x());
	node_center.setAttribute("y", _center.y());

	parent.appendChild(node_center);
}

/**
 * Write an area node to the specified Dom document under the parent node.
 */
void AbstractFeature::saveArea(QDomDocument &doc, QDomNode &parent) {
	QDomElement node_area = doc.createElement("area");
	parent.appendChild(node_area);

	for (int i = 0; i < _area.size(); ++i) {
		QDomElement node_point = doc.createElement("point");
		node_point.setAttribute("x", _area[i].x());
		node_point.setAttribute("y", _area[i].y());

		node_area.appendChild(node_point);
	}
}
