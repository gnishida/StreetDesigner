#include <QDomDocument>
#include <QTextStream>
#include "../../common/Util.h"
#include "KDEFeature.h"

KDEFeature::KDEFeature() : AbstractFeature() {
	_type = TYPE_KDE;
}

KDEFeature::KDEFeature(int group_id) : AbstractFeature() {
	_type = TYPE_KDE;
	this->group_id = group_id;
}

void KDEFeature::setDensity(int roadType, float density) {
	switch (roadType) {
	case RoadEdge::TYPE_AVENUE:
		_avenueDensity = density;
		break;
	case RoadEdge::TYPE_STREET:
		_streetDensity = density;
		break;
	}
}

void KDEFeature::addItem(int roadType, const KDEFeatureItem &item) {
	switch (roadType) {
	case RoadEdge::TYPE_AVENUE:
		_avenueItems.push_back(item);
		break;
	case RoadEdge::TYPE_STREET:
		_streetItems.push_back(item);
		break;
	}
}

size_t KDEFeature::numItems(int roadType) const {
	if (roadType == RoadEdge::TYPE_AVENUE) {
		return _avenueItems.size();
	} else {
		return _streetItems.size();
	}
}

const std::vector<KDEFeatureItem>& KDEFeature::items(int roadType) const {
	switch (roadType) {
	case RoadEdge::TYPE_AVENUE:
		return _avenueItems;
	case RoadEdge::TYPE_STREET:
		return _streetItems;
	default:
		return _streetItems;
	}
}

const float KDEFeature::density(int roadType) const {
	switch (roadType) {
	case RoadEdge::TYPE_AVENUE:
		return _avenueDensity;
	case RoadEdge::TYPE_STREET:
		return _streetDensity;
	default:
		return _streetDensity;
	}
}

/**
 * 指定された角度[degree]だけ、交差点カーネルを時計回りに回転する。
 */
void KDEFeature::rotate(float deg) {
	QVector2D centroid = _area.centroid();
	_area.rotate(deg, centroid);

	for (int i = 0; i < _avenueItems.size(); ++i) {
		_avenueItems[i].rotate(deg, centroid);
	}

	for (int i = 0; i < _streetItems.size(); ++i) {
		_streetItems[i].rotate(deg, centroid);
	}
}

void KDEFeature::scale(const Polygon2D &area) {
	QVector2D centroid = _area.centroid();

	BBox bboxTarget = area.envelope();
	BBox bboxSource = _area.envelope();

	float scaleX = bboxTarget.dx() / bboxSource.dx() + 0.1f;
	float scaleY = bboxTarget.dy() / bboxSource.dy() + 0.1f;

	for (int i = 0; i < _avenueItems.size(); ++i) {
		_avenueItems[i].scale(scaleX, scaleY, centroid);

		QString str = QString("avenue_kernel_%1.png").arg(i);
		_avenueItems[i].imwrite(str);
	}

	for (int i = 0; i < _streetItems.size(); ++i) {
		_streetItems[i].scale(scaleX, scaleY, centroid);
	}

	for (int i = 0; i < _area.size(); ++i) {
		QVector2D vec = _area[i] - centroid;
		_area[i].setX(centroid.x() + vec.x() * scaleX);
		_area[i].setY(centroid.y() + vec.y() * scaleY);
	}
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、グリッド特徴量を設定する。
 */
void KDEFeature::load(QDomNode& node) {
	_avenueItems.clear();
	_streetItems.clear();

	_weight = node.toElement().attribute("weight").toFloat();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "center") {
			loadCenter(child);
		} else if (child.toElement().tagName() == "area") {
			loadArea(child);
		} else if (child.toElement().tagName() == "avenue") {
			_avenueDensity = child.toElement().attribute("density").toFloat();
			loadAvenue(child);
		} else if (child.toElement().tagName() == "street") {
			_streetDensity = child.toElement().attribute("density").toFloat();
			loadStreet(child);
		}

		child = child.nextSibling();
	}
}

void KDEFeature::loadAvenue(QDomNode& node) {
	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "item") {
			KDEFeatureItem item;
			item.id = child.toElement().attribute("id").toInt();
			item.pt = QVector2D(child.toElement().attribute("x").toFloat(), child.toElement().attribute("y").toFloat());
			item.streetSeed = child.toElement().attribute("streetSeed") == "true" ? true : false;
			item.territory = child.toElement().attribute("territory").toFloat();
			item.load(child);
			_avenueItems.push_back(item);
		}

		child = child.nextSibling();
	}
}

void KDEFeature::loadStreet(QDomNode& node) {
	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "item") {
			KDEFeatureItem item;
			item.id = child.toElement().attribute("id").toInt();
			item.pt = QVector2D(child.toElement().attribute("x").toFloat(), child.toElement().attribute("y").toFloat());
			item.territory = child.toElement().attribute("territory").toFloat();
			item.load(child);
			_streetItems.push_back(item);
		}

		child = child.nextSibling();
	}
}

void KDEFeature::save(QDomDocument& doc, QDomNode& root) {
	QString str;

	QDomElement node_feature = doc.createElement("feature");
	node_feature.setAttribute("type", "kde");
	node_feature.setAttribute("weight", _weight);
	root.appendChild(node_feature);

	// write center node
	saveCenter(doc, node_feature);

	// write area node
	saveArea(doc, node_feature);

	// write avenue items
	str.setNum(_avenueDensity);
	QDomElement node_avenue = doc.createElement("avenue");
	node_avenue.setAttribute("density", _avenueDensity);
	node_feature.appendChild(node_avenue);

	saveAvenue(doc, node_avenue);

	// write street items
	QDomElement node_street = doc.createElement("street");
	node_street.setAttribute("density", _streetDensity);
	node_feature.appendChild(node_street);

	saveStreet(doc, node_street);
}

void KDEFeature::saveAvenue(QDomDocument& doc, QDomNode& node) {
	for (int i = 0; i < _avenueItems.size(); ++i) {
		QDomElement node_item = doc.createElement("item");
		node_item.setAttribute("id", _avenueItems[i].id);
		node_item.setAttribute("x", _avenueItems[i].pt.x());
		node_item.setAttribute("y", _avenueItems[i].pt.y());
		node_item.setAttribute("streetSeed", _avenueItems[i].streetSeed ? "true" : "false");
		node_item.setAttribute("territory", _avenueItems[i].territory);
		node.appendChild(node_item);
		_avenueItems[i].save(doc, node_item);
	}
}

void KDEFeature::saveStreet(QDomDocument& doc, QDomNode& node) {
	for (int i = 0; i < _streetItems.size(); ++i) {
		QDomElement node_item = doc.createElement("item");
		node_item.setAttribute("id", _streetItems[i].id);
		node_item.setAttribute("x", _streetItems[i].pt.x());
		node_item.setAttribute("y", _streetItems[i].pt.y());
		node_item.setAttribute("streetSeed", "false");
		node_item.setAttribute("territory", _streetItems[i].territory);
		node.appendChild(node_item);
		_streetItems[i].save(doc, node_item);
	}
}
