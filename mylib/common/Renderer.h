#pragma once

#include "Polygon2D.h"
#include "Polyline2D.h"
#include "Polyline3D.h"
#include "Renderable.h"
#include "../../Core/Renderable.h"

class Renderer {
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

