#include "Histogram.h"
#include <exception>

Histogram::Histogram(float step, int num) {
	this->step = step;
	bins.clear();
	for (int i = 0; i < num; ++i) {
		addBin(step * i, step * i + step);
	}
}

void Histogram::clear() {
	bins.clear();
}

void Histogram::addBin(float min_value, float max_value) {
	bins.push_back(HistogramBin(min_value, max_value));
}

HistogramBin& Histogram::getBin(float value) {
	for (int i = 0; i < bins.size(); ++i) {
		if (bins[i].withinRange(value)) return bins[i];
	}

	float range = bins[bins.size() - 1].max_range;
	while (true) {
		addBin(range, range + step);
		if (range > value) break;
		range += step;
	}

	return bins[bins.size() - 1];
}
