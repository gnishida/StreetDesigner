#include <road/GraphUtil.h>
#include "ClipBoard.h"

void ClipBoard::clear() {
	_empty = true;
}

bool ClipBoard::empty() const {
	return _empty;
}

void ClipBoard::copy(RoadGraph& roads) {
	GraphUtil::copyRoads(roads, _roads);
}

void ClipBoard::copy(RoadGraph& roads, Polygon2D& area) {
	GraphUtil::copyRoads(roads, _roads);
	GraphUtil::extractRoads2(_roads, area);
	GraphUtil::clean(_roads);
}

void ClipBoard::paste(RoadGraph& roads) {
	GraphUtil::copyRoads(_roads, roads);
}
