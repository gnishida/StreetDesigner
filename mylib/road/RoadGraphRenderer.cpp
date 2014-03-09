#include "RoadGraphRenderer.h"
#include <QtOpenGL>

RoadGraphRenderer::RoadGraphRenderer() {
}

void RoadGraphRenderer::render(std::vector<RenderablePtr>& renderables) {
	for (int i = 0; i < renderables.size(); i++) {
		renderOne(renderables[i]);
	}
}

void RoadGraphRenderer::renderOne(RenderablePtr renderable) {
	if (renderable->type == GL_LINE_STIPPLE) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(1 , 0xF0F0);
		glLineWidth(renderable->size);
	} else {
		glDisable(GL_LINE_STIPPLE);
	}

	if (renderable->type == GL_LINES || renderable->type == GL_LINE_STRIP) {
		glLineWidth(renderable->size);
	}

	if (renderable->type == GL_POINTS) {
		glPointSize(renderable->size);
	}

	if (renderable->type != GL_LINE_STIPPLE) {
		glBegin(renderable->type);
	} else {
		glBegin(GL_LINE_STRIP);
	}
	for (int j = 0; j < renderable->vertices.size(); ++j) {
		glColor3f(renderable->vertices[j].color[0], renderable->vertices[j].color[1], renderable->vertices[j].color[2]);
		glNormal3f(renderable->vertices[j].normal[0], renderable->vertices[j].normal[1], renderable->vertices[j].normal[2]);
		glVertex3f(renderable->vertices[j].location[0], renderable->vertices[j].location[1], renderable->vertices[j].location[2]);
	}
	glEnd();
}

/**
 * 与えられたポリゴンに基づいて、閉じた領域を描画する。
 * ただし、ポリゴンデータ自体は、閉じていなくて良い。
 */
void RoadGraphRenderer::renderArea(const Polygon2D& area, GLenum lineType, const QColor& color, float height) {
	std::vector<RenderablePtr> renderables;
	renderables.push_back(RenderablePtr(new Renderable(lineType, 3.0f)));
	renderables.push_back(RenderablePtr(new Renderable(GL_POINTS, 10.0f)));

	Vertex v;

	v.color[0] = color.redF();
	v.color[1] = color.greenF();
	v.color[2] = color.blueF();
	v.color[3] = color.alphaF();
	v.normal[0] = 0.0f;
	v.normal[1] = 0.0f;
	v.normal[2] = 1.0f;

	for (int i = 0; i < area.size(); i++) {
		v.location[0] = area[i].x();
		v.location[1] = area[i].y();
		v.location[2] = height;
		renderables[0]->vertices.push_back(v);
		renderables[1]->vertices.push_back(v);
	}

	v.location[0] = area[0].x();
	v.location[1] = area[0].y();
	v.location[2] = height;
	renderables[0]->vertices.push_back(v);

	render(renderables);
}

void RoadGraphRenderer::renderPoint(const QVector2D& pt, const QColor& color, float height) {
	RenderablePtr renderable = RenderablePtr(new Renderable(GL_POINTS, 10.0f));

	Vertex v;

	v.location[0] = pt.x();
	v.location[1] = pt.y();
	v.location[2] = height;
	v.color[0] = color.redF();
	v.color[1] = color.greenF();
	v.color[2] = color.blueF();
	v.color[3] = color.alphaF();
	v.normal[0] = 0.0f;
	v.normal[1] = 0.0f;
	v.normal[2] = 1.0f;

	renderable->vertices.push_back(v);

	renderOne(renderable);
}

void RoadGraphRenderer::renderPolyline(const Polyline2D& polyline, const QColor& color, GLenum lineType, float height) {
	std::vector<RenderablePtr> renderables;
	renderables.push_back(RenderablePtr(new Renderable(lineType, 3.0f)));
	renderables.push_back(RenderablePtr(new Renderable(GL_POINTS, 10.0f)));
	
	Vertex v;
	v.color[0] = color.redF();
	v.color[1] = color.greenF();
	v.color[2] = color.blueF();
	v.color[3] = color.alphaF();
	v.normal[0] = 0.0f;
	v.normal[1] = 0.0f;
	v.normal[2] = 1.0f;

	// add lines
	for (int i = 0; i < polyline.size(); i++) {
		v.location[0] = polyline[i].x();
		v.location[1] = polyline[i].y();
		v.location[2] = height;

		renderables[0]->vertices.push_back(v);
		renderables[1]->vertices.push_back(v);
	}

	render(renderables);
}

void RoadGraphRenderer::renderConcave(Polygon2D& polygon, const QColor& color, float height) {
	std::vector<RenderablePtr> renderables;
	renderables.push_back(RenderablePtr(new Renderable(GL_TRIANGLES)));

	if (polygon.size() < 3) return;

	Vertex v;
	v.color[0] = color.redF();
	v.color[1] = color.greenF();
	v.color[2] = color.blueF();
	v.color[3] = color.alphaF();
	v.normal[0] = 0.0f;
	v.normal[1] = 0.0f;
	v.normal[2] = 1.0f;

	std::vector<Polygon2D> trapezoids = polygon.tessellate();

	for (int i = 0; i < trapezoids.size(); ++i) {
		if (trapezoids[i].size() < 3) continue;

		for (int j = 1; j < trapezoids[i].size() - 1; ++j) {
			v.location[0] = trapezoids[i][0].x();
			v.location[1] = trapezoids[i][0].y();
			v.location[2] = height;
			renderables[0]->vertices.push_back(v);

			v.location[0] = trapezoids[i][j].x();
			v.location[1] = trapezoids[i][j].y();
			v.location[2] = height;
			renderables[0]->vertices.push_back(v);

			v.location[0] = trapezoids[i][j+1].x();
			v.location[1] = trapezoids[i][j+1].y();
			v.location[2] = height;
			renderables[0]->vertices.push_back(v);
		}
	}

	render(renderables);
}