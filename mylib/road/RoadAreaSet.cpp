#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include "RoadAreaSet.h"
#include "GraphUtil.h"

const size_t RoadAreaSet::size() const {
	return areas.size();
}

RoadArea& RoadAreaSet::operator[](int index) {
	return areas[index];
}

void RoadAreaSet::add(const RoadArea &area) {
	areas.push_back(area);
}

void RoadAreaSet::clear() {
	roads.clear();
	areas.clear();
}

void RoadAreaSet::remove(int index) {
	areas.erase(areas.begin() + index);
}

bool RoadAreaSet::contains(const QVector2D &pt) const {
	for (int i = 0; i < areas.size(); ++i) {
		if (areas[i].area.contains(pt)) return true;
	}

	return false;
}

Polygon2D RoadAreaSet::unionArea() const {
	Polygon2D ret;
	if (areas.size() == 0) return ret;
	
	std::vector<boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > > output;
	for (int i = 0; i < areas.size(); ++i) {
		boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > temp;

		if (output.size() > 0) {
			for (int j = 0; j < output[0].outer().size(); ++j) {
				temp.outer().push_back(output[0].outer()[j]);
			}
		}

		output.clear();
		
		//Polygon2D temp;
		boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > b = areas[i].area.convertToBoostPolygon();
		boost::geometry::union_(temp, b, output);
	}

	for (int i = 0; i < output[0].outer().size(); ++i) {
		ret.push_back(QVector2D(output[0].outer()[i].x(), output[0].outer()[i].y()));
	}

	return ret;
}

void RoadAreaSet::selectArea(const QVector2D &pt) {
	selectedIndex = -1;
	for (int i = 0; i < areas.size(); ++i) {
		if (areas[i].area.contains(pt)) {
			selectedIndex = i;
			break;
		}
	}
}

void RoadAreaSet::selectLastArea() {
	selectedIndex = areas.size() - 1;
}

RoadArea& RoadAreaSet::selectedArea() {
	return areas[selectedIndex];
}

void RoadAreaSet::setZ(float z) {
	roads.setZ(z);
	for (int i = 0; i < areas.size(); ++i) {
		areas[i].roads.setZ(z);
	}
}

void RoadAreaSet::addRoads(int roadType, int lanes, bool oneWay, const Polyline2D &polyline) {
	GraphUtil::addEdge(roads, polyline, roadType, lanes, oneWay);
	GraphUtil::planarify(roads);
	GraphUtil::clean(roads);
}

void RoadAreaSet::mergeRoads() {
	for (int i = 0; i < areas.size(); ++i) {
		GraphUtil::mergeRoads(roads, areas[i].roads);
		areas[i].roads.clear();
	}
}

/**
 * 与えられたfeatureノード配下のXML情報に基づいて、グリッド特徴量を設定する。
 */
void RoadAreaSet::load(QString filename) {
	// ファイル名からディレクトリ部を取得
	QString dirname;
	int index = filename.lastIndexOf("/");
	if (index > 0) {
		dirname = filename.mid(0, index);
	}

	QFile file(filename);

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	if (root.hasAttribute("roads")) {
		QString roadFilename = dirname + "/" + root.attribute("roads");
		GraphUtil::loadRoads(roads, roadFilename);
	}

	QDomNode node = root.firstChild();
	while (!node.isNull()) {
		if (node.toElement().tagName() == "area") {
			RoadArea area;
			area.load(node);
			areas.push_back(area);
		}

		node = node.nextSibling();
	}
}

void RoadAreaSet::save(QString filepath) {
	// ファイルパスからファイル名を取得
	QString name;
	int index = filepath.lastIndexOf("/");
	if (index >= 0) {
		name = filepath.mid(index + 1);
	} else {
		name = filepath;
	}

	QDomDocument doc;

	QString roadFilename = name + ".gsm";
	GraphUtil::saveRoads(roads, filepath + ".gsm");

	QDomElement root = doc.createElement("areas");
	root.setAttribute("roads", roadFilename);
	doc.appendChild(root);

	for (int i = 0; i < areas.size(); ++i) {
		areas[i].save(doc, root);
	}

	// write the dom to the file
	QFile file(filepath);
	file.open(QIODevice::WriteOnly);

	QTextStream out(&file);
	doc.save(out, 4);
}

