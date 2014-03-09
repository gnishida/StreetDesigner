#pragma once

#include "AbstractFeature.h"
#include <vector>
#include <QVector2D>
#include <QMap>
#include <QDomNode>
#include <boost/shared_ptr.hpp>

class GenericFeature : public AbstractFeature {
public:
	int group_id;
	QMap<float, float> avenueLengths;
	QMap<int, float> avenueNumDirections;
	QMap<float, float> streetLengths;
	QMap<int, float> streetNumDirections;

	int accmAvenueLenCount;		// Avenueエッジ長の累積カウンタ
	int accmStreetLenCount;		// Local streetエッジ長の累積カウンタ
	int accmAvenueDirCount;		// Avenue交差点のdegree累積カウンタ
	int accmStreetDirCount;		// Avenue交差点のdegree累積カウンタ

public:
	GenericFeature();
	GenericFeature(int group_id);
	~GenericFeature() {}

	void addEdge(float length, int roadType);
	void addNumDiretions(int numDirections, int roadType);
	void computeFeature();

	std::vector<float> getAngles(int num) const;
	std::vector<float> getAngles(int num, const QVector2D &dir) const;
	std::vector<float> getPerpendicularAngles(const QVector2D &dir) const;
	std::vector<float> getLengths(int roadType, int num) const;
	float generateLength(int roadType, float uniform_random_number) const;
	int getDirections(int roadType) const;

	void rotate(float deg);
	void scale(const Polygon2D &area);

	void load(QString filename);
	void load(QDomNode& node);
	void loadAvenue(QDomNode& node);
	void loadStreet(QDomNode& node);
	
	void save(QString filename);
	void save(QDomDocument& doc, QDomNode& node);
	void saveAvenue(QDomDocument& doc, QDomNode& node);
	void saveStreet(QDomDocument& doc, QDomNode& node);
};

typedef boost::shared_ptr<GenericFeature> GenericFeaturePtr;