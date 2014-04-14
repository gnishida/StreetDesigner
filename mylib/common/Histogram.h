#pragma once

#include "HistogramBin.h"

class Histogram {
public:
	float step;
	std::vector<HistogramBin> bins;

public:
	Histogram() : step(300.0f) {}
	Histogram(float step, int num);
	~Histogram() {}

	void clear();
	void addBin(float min_value, float max_value);
	HistogramBin& getBin(float value);
};

