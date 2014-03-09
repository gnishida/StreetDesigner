#include "RoadVertex.h"

RoadVertex::RoadVertex() {
	this->pt = QVector2D(0.0f, 0.0f);
	//this->virt = false;
	this->valid = true;
	this->seed = false;
	this->onBoundary = false;
}

RoadVertex::RoadVertex(const QVector2D &pt) {
	this->pt.setX(pt.x());
	this->pt.setY(pt.y());
	//this->virt = false;
	this->valid = true;
	this->seed = false;
	this->onBoundary = false;
}

const QVector2D& RoadVertex::getPt() const {
	return pt;
}

