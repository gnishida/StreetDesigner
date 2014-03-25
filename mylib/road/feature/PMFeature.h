#pragma once

#include <vector>
#include <QVector2D>
#include <QMap>
#include <QDomNode>

class PMFeature {
public:
	QMap<float, float> avenueLengths;
	QMap<int, float> avenueNumDirections;
	QMap<float, float> streetLengths;
	QMap<int, float> streetNumDirections;

public:
	PMFeature() {}
	~PMFeature() {}

	void load(QString filename);
	void load(QDomNode& node);
	void loadAvenue(QDomNode& node);
	void loadStreet(QDomNode& node);
	
	void save(QString filename);
	void save(QDomDocument& doc, QDomNode& node);
	void saveAvenue(QDomDocument& doc, QDomNode& node);
	void saveStreet(QDomDocument& doc, QDomNode& node);
};

