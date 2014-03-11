﻿#include "PolygonBuilder.h"

PolygonBuilder::PolygonBuilder() {
	_selecting = false;
}

void PolygonBuilder::start(const QVector2D& pt) {
	_polyline.clear();
	_polyline.push_back(pt);

	_selecting = true;
}

void PolygonBuilder::addPoint(const QVector2D& pt) {
	_polyline.push_back(pt);
}

void PolygonBuilder::moveLastPoint(const QVector2D& pt) {
	if (_polyline.size() == 0) return;

	_polyline[_polyline.size() - 1] = pt;
}

void PolygonBuilder::end() {
	_selecting = false;
}

void PolygonBuilder::cancel() {
	_polyline.clear();
	_selecting = false;
}

bool PolygonBuilder::selected() const {
	return !_selecting && _polyline.size() >= 3;
}

bool PolygonBuilder::selecting() const {
	return _selecting;
}

Polyline2D PolygonBuilder::polyline() const {
	return _polyline;
}

Polygon2D PolygonBuilder::polygon() const {
	Polygon2D area;

	for (int i = 0; i < _polyline.size(); ++i) {
		area.push_back(_polyline[i]);
	}

	area.correct();

	return area;
}

const Polyline3D& PolygonBuilder::polyline3D() const {
	return _polyline3D;
}

void PolygonBuilder::adaptToTerrain(MyTerrain* terrain) {
	_polyline3D.clear();

	for (int i = 0; i < _polyline.size(); ++i) {
		float z = terrain->getValue(_polyline[i].x(), _polyline[i].y());
		_polyline3D.push_back(QVector3D(_polyline[i].x(), _polyline[i].y(), z + 100));
	}
}
