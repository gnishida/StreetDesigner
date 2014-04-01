#include "PolylineBuilder.h"

PolylineBuilder::PolylineBuilder() {
	_selecting = false;
}

void PolylineBuilder::start(const QVector2D& pt) {
	_polyline.clear();
	_polyline.push_back(pt);

	_selecting = true;
}

/**
 * Add pt to this polyline.
 */
void PolylineBuilder::addPoint(const QVector2D& pt) {
	_polyline.push_back(pt);
}

void PolylineBuilder::moveLastPoint(const QVector2D& pt) {
	if (_polyline.size() == 0) return;

	_polyline[_polyline.size() - 1] = pt;
}

void PolylineBuilder::end() {
	_selecting = false;

	if (_polyline.size() >= 2 && (_polyline[_polyline.size() - 2] - _polyline.last()).lengthSquared() < 1.0f) {
		_polyline.pop_back();
	}
}

void PolylineBuilder::cancel() {
	_polyline.clear();
	_selecting = false;
}

bool PolylineBuilder::selected() const {
	return !_selecting && _polyline.size() >= 3;
}

bool PolylineBuilder::selecting() const {
	return _selecting;
}

Polyline2D PolylineBuilder::polyline() const {
	return _polyline;
}
