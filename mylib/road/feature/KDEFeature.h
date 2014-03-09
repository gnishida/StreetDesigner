#pragma once

#include <vector>
#include <QDomNode>
#include <boost/shared_ptr.hpp>
#include "AbstractFeature.h"
#include "KDEFeatureItem.h"
#include "../../common/Polygon2D.h"
#include "../RoadEdge.h"

class KDEFeature : public AbstractFeature {
public:
	int group_id;
	float _avenueDensity;						// how many vertices in 1km x 1km area
	float _streetDensity;						// how many vertices in 1km x 1km area
	std::vector<KDEFeatureItem> _avenueItems;
	std::vector<KDEFeatureItem> _streetItems;

public:
	KDEFeature();
	KDEFeature(int group_id);
	~KDEFeature() {}

	void setDensity(int roadType, float density);
	void addItem(int roadType, const KDEFeatureItem &item);
	size_t numItems(int roadType) const;

	const std::vector<KDEFeatureItem>& items(int roadType) const;
	const float density(int roadType) const;

	void rotate(float deg);
	void scale(const Polygon2D &area);

	void load(QDomNode& node);
	void loadAvenue(QDomNode& node);
	void loadStreet(QDomNode& node);
	void save(QDomDocument& doc, QDomNode& node);
	void saveAvenue(QDomDocument& doc, QDomNode& node);
	void saveStreet(QDomDocument& doc, QDomNode& node);
};

typedef boost::shared_ptr<KDEFeature> KDEFeaturePtr;