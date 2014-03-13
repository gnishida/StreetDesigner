#pragma once

#include "../common/Polygon2D.h"
#include "../common/Polygon3D.h"
#include "../common/Polyline2D.h"
#include "../common/Polyline3D.h"
#include "Renderable.h"

class RendererHelper {
public:
	unsigned int dispList;

public:
	void render(std::vector<mylib::Renderable*> renderables);
	void renderOne(mylib::Renderable* renderable);

	void renderArea(const Polygon3D& area, const QColor& color, GLenum lineType);
	void renderPoint(const QVector2D& pt, const QColor& color, float height);
	void renderPolyline(const Polyline3D& polyline, const QColor& color, GLenum lineType);

	//void tessellate(const Loop2D& polygon);
	//void renderConcave(Polygon2D& polygon, const QColor& color, float height);
};

