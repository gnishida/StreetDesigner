#pragma once

#include <QVector2D>
#include <QDomDocument>
#include <QDomNode>
#include <boost/shared_ptr.hpp>
#include "../../common/Polygon2D.h"

class AbstractFeature {
public:
	static enum { TYPE_GRID = 0, TYPE_RADIAL, TYPE_KDE, TYPE_GENERIC };

protected:
	int _type;
	QVector2D _center;
	float _weight;
	Polygon2D _area;

public:
	AbstractFeature();
	~AbstractFeature() {}

	int type() const;
	const QVector2D& center() const;
	void setCenter(const QVector2D &center);
	float weight() const;
	void setWeight(float weight);
	const Polygon2D& area() const;
	void setArea(const Polygon2D &area);

	virtual void rotate(float deg) = 0;
	virtual void scale(const Polygon2D &area) = 0;

	virtual void load(QDomNode& node) = 0;
	virtual void save(QDomDocument& doc, QDomNode& node) = 0;

protected:
	void loadCenter(QDomNode &node);
	void loadArea(QDomNode &node);

	void saveCenter(QDomDocument &doc, QDomNode &parent);
	void saveArea(QDomDocument &doc, QDomNode &parent);
};

typedef boost::shared_ptr<AbstractFeature> AbstractFeaturePtr;