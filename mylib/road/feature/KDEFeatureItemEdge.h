#pragma once

#include "../../common/Polyline2D.h"

class KDEFeatureItemEdge {
public:
	Polyline2D edge;
	int lanes;
	bool deadend;
	bool onBoundary;

public:
	KDEFeatureItemEdge(const Polyline2D &edge, int lanes, bool deadend, bool onBoundary) : edge(edge), lanes(lanes), deadend(deadend), onBoundary(onBoundary) {}
	~KDEFeatureItemEdge() {}

	void scale(float scaleX, float scaleY, const QVector2D &orig);
};

