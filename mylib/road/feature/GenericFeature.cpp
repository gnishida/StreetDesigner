#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include "GenericFeature.h"
#include "../../common/common.h"
#include "../../common/Util.h"
#include "../RoadEdge.h"

GenericFeature::GenericFeature() : AbstractFeature() {
	_type = TYPE_GENERIC;

	accmAvenueLenCount = 0;
	accmStreetLenCount = 0;
	accmAvenueDirCount = 0;
	accmStreetDirCount = 0;
}

GenericFeature::GenericFeature(int group_id) {
	_type = TYPE_GENERIC;
	this->group_id = group_id;

	_weight = 0.0f;
	accmAvenueLenCount = 0;
	accmStreetLenCount = 0;
	accmAvenueDirCount = 0;
	accmStreetDirCount = 0;
}

void GenericFeature::addEdge(float length, int roadType) {
	if (roadType == RoadEdge::TYPE_STREET) {
		streetLengths[(int)(length / 20) * 20 + 10] += 1;
		accmStreetLenCount++;
	} else {
		avenueLengths[(int)(length / 20) * 20 + 10] += 1;
		accmAvenueLenCount++;
	}

	_weight += length;
}

void GenericFeature::addNumDiretions(int numDirections, int roadType) {
	if (roadType == RoadEdge::TYPE_STREET) {
		streetNumDirections[numDirections] += 1;
		accmStreetDirCount++;
	} else {
		avenueNumDirections[numDirections] += 1;
		accmAvenueDirCount++;
	}
}

/**
 * エッジの追加が終わったら、この関数を呼んで、ヒストグラムのnormalizeする。
 */
void GenericFeature::computeFeature() {
	// エッジの長さのヒストグラムをnormalizeする
	for (QMap<float, float>::iterator it = streetLengths.begin(); it != streetLengths.end(); ++it) {
		streetLengths[it.key()] /= (float)accmStreetLenCount;
	}
	for (QMap<float, float>::iterator it = avenueLengths.begin(); it != avenueLengths.end(); ++it) {
		avenueLengths[it.key()] /= (float)accmAvenueLenCount;
	}

	// エッジの長さの累積確率分布を生成
	float apd = 0.0f;
	for (QMap<float, float>::iterator it = streetLengths.begin(); it != streetLengths.end(); ++it) {
		apd += streetLengths[it.key()];
		streetLengths[it.key()] = apd;
	}
	apd = 0.0f;
	for (QMap<float, float>::iterator it = avenueLengths.begin(); it != avenueLengths.end(); ++it) {
		apd += avenueLengths[it.key()];
		avenueLengths[it.key()] = apd;
	}

	// 交差点のdegreeのヒストグラムをnormalizeする
	for (QMap<int, float>::iterator it = streetNumDirections.begin(); it != streetNumDirections.end(); ++it) {
		streetNumDirections[it.key()] /= (float)accmStreetDirCount;
	}
	for (QMap<int, float>::iterator it = avenueNumDirections.begin(); it != avenueNumDirections.end(); ++it) {
		avenueNumDirections[it.key()] /= (float)accmAvenueDirCount;
	}

	// 交差点のdegreeの累積確率分布を生成
	apd = 0.0f;
	for (QMap<int, float>::iterator it = streetNumDirections.begin(); it != streetNumDirections.end(); ++it) {
		apd += streetNumDirections[it.key()];
		streetNumDirections[it.key()] = apd;
	}
	apd = 0.0f;
	for (QMap<int, float>::iterator it = avenueNumDirections.begin(); it != avenueNumDirections.end(); ++it) {
		apd += avenueNumDirections[it.key()];
		avenueNumDirections[it.key()] = apd;
	}

	// 累積カウンタをリセットする
	accmAvenueLenCount = 0;
	accmStreetLenCount = 0;
	accmAvenueDirCount = 0;
	accmStreetDirCount = 0;
}

std::vector<float> GenericFeature::getAngles(int num) const {
	std::vector<float> angles;

	float angle = Util::uniform_rand(0, M_PI * 2.0f);
	float angle_step = M_PI * 2.0f / num;

	for (int i = 0; i < num; ++i) {
		angles.push_back(angle + Util::uniform_rand(-0.2f, 0.2f));

		angle += angle_step;		
	}

	return angles;
}

/**
 * 指定された数のエッジ方向を生成する。ただし、dirで指定された方向には既にエッジがあるので、この方向は避ける。
 */
std::vector<float> GenericFeature::getAngles(int num, const QVector2D &dir) const {
	std::vector<float> angles;

	float angle = atan2f(dir.y(), dir.x());
	float angle_step = M_PI * 2.0f / (num + 1);

	for (int i = 0; i < num; ++i) {
		angle += angle_step;		

		angles.push_back(angle + Util::uniform_rand(-0.2f, 0.2f));
	}

	return angles;
}

/**
 * 指定された方向に垂直な２方向を生成する。
 */
std::vector<float> GenericFeature::getPerpendicularAngles(const QVector2D &dir) const {
	std::vector<float> angles;

	float angle = atan2f(dir.x(), -dir.y());
	float angle_step = M_PI;

	for (int i = 0; i < 2; ++i) {
		angles.push_back(angle + Util::uniform_rand(-0.2f, 0.2f));

		angle += angle_step;		
	}

	return angles;
}

/**
 * 指定された数のエッジ長を生成する。
 */
std::vector<float> GenericFeature::getLengths(int roadType, int num) const {
	std::vector<float> ret;

	for (int i = 0; i < num; ++i) {
		ret.push_back(generateLength(roadType, Util::uniform_rand()));
	}

	return ret;
}

/** 
 * 与えられたuniform random numberに基づいて、エッジの長さを生成する。
 */
float GenericFeature::generateLength(int roadType, float uniform_random_number) const {
	if (roadType == RoadEdge::TYPE_STREET) {
		for (QMap<float, float>::iterator it = streetLengths.begin(); it != streetLengths.end(); ++it) {
			if (uniform_random_number <= streetLengths[it.key()]) return it.key();
		}
	} else {
		for (QMap<float, float>::iterator it = avenueLengths.begin(); it != avenueLengths.end(); ++it) {
			if (uniform_random_number <= avenueLengths[it.key()]) return it.key();
		}
	}

	return 0.0f;
}

int GenericFeature::getDirections(int roadType) const {
	float uniform_random_number = Util::uniform_rand();

	for (QMap<int, float>::iterator it = avenueNumDirections.begin(); it != avenueNumDirections.end(); ++it) {
		if (uniform_random_number <= avenueNumDirections[it.key()]) return it.key();
	}

	return 0;
}

void GenericFeature::rotate(float deg) {
}

void GenericFeature::scale(const Polygon2D &area) {
}

/**
 * ファイルから、グリッドの特徴量を読込む。
 */
void GenericFeature::load(QString filename) {
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
void GenericFeature::load(QDomNode& node) {
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
void GenericFeature::loadAvenue(QDomNode& node) {
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
void GenericFeature::loadStreet(QDomNode& node) {
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
void GenericFeature::save(QString filename) {
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

void GenericFeature::save(QDomDocument& doc, QDomNode& root) {
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

void GenericFeature::saveAvenue(QDomDocument& doc, QDomNode& node) {
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

void GenericFeature::saveStreet(QDomDocument& doc, QDomNode& node) {
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
