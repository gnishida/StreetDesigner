#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <random>
#include "GridFeature.h"
#include "../../common/Util.h"

GridFeature::GridFeature(int group_id) : AbstractFeature() {
	_type = TYPE_GRID;
	this->group_id = group_id;

	accmLenCount1 = 0;
	accmLenCount2 = 0;
}

/**
 * グリッド方向の近似値を使って、グリッドの角度を仮にセットする。
 */
void GridFeature::setAngle(float angle) {
	angle1 = angle;
	angle2 = angle + M_PI * 0.5f;

	accmDirCount = 0;
	accmDir1 = QVector2D(0, 0);
	accmDir2 = QVector2D(0, 0);

	length1.clear();
	length2.clear();
	accmLenCount1 = 0;
	accmLenCount2 = 0;
}

/**
 * エッジ方向を追加しながら、グリッド方向の精度を上げる。
 * ただし、当該エッジの方向が、仮の方向から閾値よりも大きく外れている場合は、そのエッジの方向は無視する。
 * グリッドの方向に加えて、長さの情報も収集し、ヒストグラムに入れていく。
 */
void GridFeature::addEdge(const QVector2D& edge_vec, float threshold) {
	QVector2D dir = edge_vec.normalized();

	float a1 = atan2f(dir.y(), dir.x());
	float a2 = atan2f(-dir.y(), -dir.x());

	float diff1 = Util::diffAngle(angle1, a1);
	float diff2 = Util::diffAngle(angle1, a2);
	float diff3 = Util::diffAngle(angle2, a1);
	float diff4 = Util::diffAngle(angle2, a2);
	float min_diff = std::min(std::min(diff1, diff2), std::min(diff3, diff4));

	// エッジの方向が、仮の方向から閾値よりも大きく外れている場合は、そのエッジの方向は無視する。
	if (min_diff > threshold) return;

	// エッジの方向を、累積方向ベクトルに追加していく
	// また、長さも、ヒストグラムに追加していく
	if (diff1 == min_diff) {
		accmDir1 += dir;
		accmDir2 += QVector2D(-dir.y(), dir.x());
		length1[(int)(edge_vec.length() / 20) * 20 + 10] += 1;
		accmLenCount1++;
	} else if (diff2 == min_diff) {
		accmDir1 -= dir;
		accmDir2 -= QVector2D(-dir.y(), dir.x());
		length1[(int)(edge_vec.length() / 20) * 20 + 10] += 1;
		accmLenCount1++;
	} else if (diff3 == min_diff) {
		accmDir1 += QVector2D(dir.y(), -dir.x());
		accmDir2 += dir;
		length2[(int)(edge_vec.length() / 20) * 20 + 10] += 1;
		accmLenCount2++;
	} else {
		accmDir1 -= QVector2D(dir.y(), -dir.x());
		accmDir2 -= dir;
		length2[(int)(edge_vec.length() / 20) * 20 + 10] += 1;
		accmLenCount2++;
	}

	accmDirCount++;
}

/**
 * エッジの追加が終わったら、この関数を呼んで、グリッド方向をより正確な値に更新する。
 * また、グリッドの長さヒストグラムのnormalizeする。
 */
void GridFeature::computeFeature() {
	accmDir1 /= (float)accmDirCount;
	accmDir2 /= (float)accmDirCount;

	angle1 = atan2f(accmDir1.y(), accmDir1.x());
	angle2 = atan2f(accmDir2.y(), accmDir2.x());

	// グリッドの長さをnormalizeする
	for (QMap<float, float>::iterator it = length1.begin(); it != length1.end(); ++it) {
		length1[it.key()] /= (float)accmLenCount1;
	}
	for (QMap<float, float>::iterator it = length2.begin(); it != length2.end(); ++it) {
		length2[it.key()] /= (float)accmLenCount2;
	}

	// グリッドの長さの累積確率分布を生成
	float apd = 0.0f;
	for (QMap<float, float>::iterator it = length1.begin(); it != length1.end(); ++it) {
		apd += length1[it.key()];
		length1[it.key()] = apd;
	}
	apd = 0.0f;
	for (QMap<float, float>::iterator it = length2.begin(); it != length2.end(); ++it) {
		apd += length2[it.key()];
		length2[it.key()] = apd;
	}

	// 累積方向ベクトルをリセットする
	accmDirCount = 0;
	accmDir1 = QVector2D(0, 0);
	accmDir2 = QVector2D(0, 0);

	// 累積長カウンタをリセットする
	accmLenCount1 = 0;
	accmLenCount2 = 0;

	// デバッグ情報を出力
	std::cout << "Grid Feature:" << std::endl;
	std::cout << "  angle 1: " << angle1 * 180.0f / M_PI << std::endl;
	std::cout << "  angle 2: " << angle2 * 180.0f / M_PI << std::endl;
	std::cout << "  length 1:" << std::endl;
	for (QMap<float, float>::iterator it = length1.begin(); it != length1.end(); ++it) {
		std::cout << "      " << it.key() << ": " << length1[it.key()] << std::endl;
	}
	std::cout << "  length 2:" << std::endl;
	for (QMap<float, float>::iterator it = length2.begin(); it != length2.end(); ++it) {
		std::cout << "      " << it.key() << ": " << length2[it.key()] << std::endl;
	}
}

/**
 * 与えられたエッジの方向が、グリッド方向に近いかどうかチェックする。
 *
 * @param threshold			与えられたエッジの方向とグリッド方向の差が、このしきい値以下であれば、trueを返却する
 */
bool GridFeature::isClose(const QVector2D& dir, float threshold) {
	float a1 = atan2f(dir.y(), dir.x());
	float a2 = atan2f(-dir.y(), -dir.x());

	if (Util::diffAngle(angle1, a1) <= threshold || Util::diffAngle(angle1, a2) <= threshold || Util::diffAngle(angle2, a1) <= threshold || Util::diffAngle(angle2, a2) <= threshold) return true;
	else return false;
}

std::vector<float> GridFeature::getAngles() const {
	std::vector<float> ret;
	ret.push_back(angle1);
	ret.push_back(angle2);
	ret.push_back(angle1 + M_PI);
	ret.push_back(angle2 + M_PI);

	return ret;
}

std::vector<float> GridFeature::getLengths() const {
	std::vector<float> ret;
	ret.push_back(generateLength(0, Util::uniform_rand()));
	ret.push_back(generateLength(1, Util::uniform_rand()));
	ret.push_back(generateLength(2, Util::uniform_rand()));
	ret.push_back(generateLength(3, Util::uniform_rand()));

	return ret;
}

/** 
 * 与えられたuniform random numberに基づいて、エッジの長さを生成する。
 *
 * @param dir		0 - 第１象限 / 1 - 第２象限 / 2 - 第３象限 / 3 - 第４象限
 */
float GridFeature::generateLength(int dir, float uniform_random_number) const {
	if (dir == 0 || dir == 2) {
		for (QMap<float, float>::iterator it = length1.begin(); it != length1.end(); ++it) {
			if (uniform_random_number <= length1[it.key()]) return it.key();
		}
	} else {
		for (QMap<float, float>::iterator it = length2.begin(); it != length2.end(); ++it) {
			if (uniform_random_number <= length2[it.key()]) return it.key();
		}
	}

	return 0.0f;
}

void GridFeature::rotate(float deg) {
}

void GridFeature::scale(const Polygon2D &area) {
}

/**
 * ファイルから、グリッドの特徴量を読込む。
 */
void GridFeature::load(QString filename) {
	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode node = root.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName() == "feature") {
			if (node.toElement().attribute("type") == "grid") {
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
void GridFeature::load(QDomNode& node) {
	length1.clear();
	length2.clear();

	_weight = node.toElement().attribute("weight").toFloat();

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "center") {
			loadCenter(child);
		} else if (child.toElement().tagName() == "area") {
			loadArea(child);
		} else if (child.toElement().tagName() == "angle1") {
			angle1 = child.firstChild().nodeValue().toFloat();
		} else if (child.toElement().tagName() == "angle2") {
			angle2 = child.firstChild().nodeValue().toFloat();
		} else if (child.toElement().tagName() == "length1") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float len = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				length1[len] = accm;

				child2 = child2.nextSibling();
			}
		} else if (child.toElement().tagName() == "length2") {
			QDomNode child2 = child.firstChild();
			while (!child2.isNull()) {
				float len = child2.toElement().attribute("key").toFloat();
				float accm = child2.firstChild().nodeValue().toFloat();
				length2[len] = accm;

				child2 = child2.nextSibling();
			}
		}

		child = child.nextSibling();
	}
}

/**
 * 特徴量をxmlファイルに保存する。
 */
void GridFeature::save(QString filename) {
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

void GridFeature::save(QDomDocument& doc, QDomNode& root) {
	QString str;

	QDomElement node_feature = doc.createElement("feature");
	node_feature.setAttribute("type", "grid");
	node_feature.setAttribute("weight", _weight);
	root.appendChild(node_feature);

	// write center node
	saveCenter(doc, node_feature);

	// write area node
	saveArea(doc, node_feature);
	
	// write angle1 node
	QDomElement node_angle1 = doc.createElement("angle1");
	node_feature.appendChild(node_angle1);

	str.setNum(angle1);
	QDomText node_angle1_value = doc.createTextNode(str);
	node_angle1.appendChild(node_angle1_value);

	// write angle2 node
	QDomElement node_angle2 = doc.createElement("angle2");
	node_feature.appendChild(node_angle2);

	str.setNum(angle2);
	QDomText node_angle2_value = doc.createTextNode(str);
	node_angle2.appendChild(node_angle2_value);

	// write length1 node
	QDomElement node_length1 = doc.createElement("length1");
	node_feature.appendChild(node_length1);

	for (QMap<float, float>::iterator it = length1.begin(); it != length1.end(); ++it) {
		QDomElement node_length1_data = doc.createElement("data");
		node_length1_data.setAttribute("key", it.key());
		node_length1.appendChild(node_length1_data);

		QString str;
		str.setNum(it.value());
		QDomText node_length1_value = doc.createTextNode(str);
		node_length1_data.appendChild(node_length1_value);
	}

	// write length2 node
	QDomElement node_length2 = doc.createElement("length2");
	node_feature.appendChild(node_length2);

	for (QMap<float, float>::iterator it = length2.begin(); it != length2.end(); ++it) {
		QDomElement node_length2_data = doc.createElement("data");
		node_length2_data.setAttribute("key", it.key());
		node_length2.appendChild(node_length2_data);

		QString str;
		str.setNum(it.value());
		QDomText node_length2_value = doc.createTextNode(str);
		node_length2_data.appendChild(node_length2_value);
	}
}

/**
 * 領域を塗りつぶすための色を自動で生成する。
 */
QColor GridFeature::color() const {
	return QColor(0, 0, 255 - group_id * 64 % 255);
}

/**
 * 領域を表すポリゴンを返却する。
 */
Polygon2D GridFeature::polygon() const {
	return _polygon;
}

