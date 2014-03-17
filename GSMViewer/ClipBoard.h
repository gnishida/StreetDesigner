#pragma once

#include <common/Polygon2D.h>
#include <road/RoadGraph.h>

class ClipBoard {
public:
	bool _empty;
	RoadGraph _roads;

public:
	ClipBoard() : _empty(true) {}
	~ClipBoard() {}

	void clear();
	bool empty() const;
	void copy(RoadGraph& roads);
	void copy(RoadGraph& roads, Polygon2D& area);
	void paste(RoadGraph& roads);
};

