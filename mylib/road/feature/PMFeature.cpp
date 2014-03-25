#include "PMFeature.h"


/**
 * ファイルから、グリッドの特徴量を読込む。
 */
void PMFeature::load(QString filename) {
	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode node = root.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName() == "feature") {
			if (node.toElement().attribute("type") == "generic") {
				load(node);
				break;
			}
		}

		node = node.nextSibling();
	}
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、グリッド特徴量を設定する。
 */
void PMFeature::load(QDomNode& node) {
	avenueLengths.clear();
	streetLengths.clear();
	avenueNumDirections.clear();
	streetNumDirections.clear();

	_weight = node.toElement().attribute("weight").toFloat();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "center") {
			loadCenter(child);
		} else if (child.toElement().tagName() == "area") {
			loadArea(child);
		} else if (child.toElement().tagName() == "avenue") {
			loadAvenue(child);
		} else if (child.toElement().tagName() == "street") {
			loadStreet(child);
		}

		child = child.nextSibling();
	}
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、Avenueの特徴量を設定する。
 */
void PMFeature::loadAvenue(QDomNode& node) {
	avenueLengths.clear();
	avenueNumDirections.clear();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "length") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float len = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				avenueLengths[len] = accm;

				child2 = child2.nextSibling();
			}
		} else if (child.toElement().tagName() == "numDirections") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float num = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				avenueNumDirections[num] = accm;

				child2 = child2.nextSibling();
			}
		}

		child = child.nextSibling();
	}
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、Local streetの特徴量を設定する。
 */
void PMFeature::loadStreet(QDomNode& node) {
	streetLengths.clear();
	streetNumDirections.clear();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "length") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float len = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				streetLengths[len] = accm;

				child2 = child2.nextSibling();
			}
		} else if (child.toElement().tagName() == "numDirections") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float num = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				streetNumDirections[num] = accm;

				child2 = child2.nextSibling();
			}
		}

		child = child.nextSibling();
	}
}

/**
 * 特徴量をxmlファイルに保存する。
 */
void PMFeature::save(QString filename) {
	QDomDocument doc;

	QDomElement root = doc.createElement("features");
	doc.appendChild(root);

	save(doc, root);

	// write the dom to the file
	QFile file(filename);
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);
	doc.save(out, 4);
}

void PMFeature::save(QDomDocument& doc, QDomNode& root) {
	QString str;

	QDomElement node_feature = doc.createElement("feature");
	node_feature.setAttribute("type", "generic");
	node_feature.setAttribute("weight", _weight);
	root.appendChild(node_feature);

	// write center node
	saveCenter(doc, node_feature);

	// write area node
	saveArea(doc, node_feature);

	// write avenue node
	QDomElement node_avenue = doc.createElement("avenue");
	node_feature.appendChild(node_avenue);
	saveAvenue(doc, node_avenue);

	// write street node
	QDomElement node_street = doc.createElement("street");
	node_feature.appendChild(node_street);
	saveStreet(doc, node_street);
}

void PMFeature::saveAvenue(QDomDocument& doc, QDomNode& node) {
	// write length node
	QDomElement node_length = doc.createElement("length");
	node.appendChild(node_length);

	for (QMap<float, float>::iterator it = avenueLengths.begin(); it != avenueLengths.end(); ++it) {
		QDomElement node_length_data = doc.createElement("data");
		node_length_data.setAttribute("key", it.key());
		node_length.appendChild(node_length_data);

		QString str;
		str.setNum(it.value());
		QDomText node_length_value = doc.createTextNode(str);
		node_length_data.appendChild(node_length_value);
	}

	// write numDirections node
	QDomElement node_numDirections = doc.createElement("numDirections");
	node.appendChild(node_numDirections);

	for (QMap<int, float>::iterator it = avenueNumDirections.begin(); it != avenueNumDirections.end(); ++it) {
		QDomElement node_numDirections_data = doc.createElement("data");
		node_numDirections_data.setAttribute("key", it.key());
		node_numDirections.appendChild(node_numDirections_data);

		QString str;
		str.setNum(it.value());
		QDomText node_numDirections_value = doc.createTextNode(str);
		node_numDirections_data.appendChild(node_numDirections_value);
	}
}

void PMFeature::saveStreet(QDomDocument& doc, QDomNode& node) {
	// write length node
	QDomElement node_length = doc.createElement("length");
	node.appendChild(node_length);

	for (QMap<float, float>::iterator it = streetLengths.begin(); it != streetLengths.end(); ++it) {
		QDomElement node_length_data = doc.createElement("data");
		node_length_data.setAttribute("key", it.key());
		node_length.appendChild(node_length_data);

		QString str;
		str.setNum(it.value());
		QDomText node_length_value = doc.createTextNode(str);
		node_length_data.appendChild(node_length_value);
	}

	// write numDirections node
	QDomElement node_numDirections = doc.createElement("numDirections");
	node.appendChild(node_numDirections);

	for (QMap<int, float>::iterator it = streetNumDirections.begin(); it != streetNumDirections.end(); ++it) {
		QDomElement node_numDirections_data = doc.createElement("data");
		node_numDirections_data.setAttribute("key", it.key());
		node_numDirections.appendChild(node_numDirections_data);

		QString str;
		str.setNum(it.value());
		QDomText node_numDirections_value = doc.createTextNode(str);
		node_numDirections_data.appendChild(node_numDirections_value);
	}
}
