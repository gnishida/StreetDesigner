#pragma once

#include "../common/Polygon2D.h"
#include "../common/Polyline2D.h"
#include "../common/Polyline3D.h"
#include "Renderable.h"

class MyRenderer {
public:
	unsigned int dispList;

public:
	void render(std::vector<ucore::Renderable*> renderables);
	void renderOne(ucore::Renderable* renderable);

	void renderArea(const Polygon2D& area, GLenum lineType, const QColor& color, float height);
	void renderPoint(const QVector2D& pt, const QColor& color, float height);
	void renderPolyline(const Polyline3D& polyline, const QColor& color, GLenum lineType);

	//void tessellate(const Loop2D& polygon);
	//void renderConcave(Polygon2D& polygon, const QColor& color, float height);
};

