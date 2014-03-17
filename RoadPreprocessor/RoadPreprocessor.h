#pragma once

#include <vector>
#include <QString>
#include <common/BBox.h>

class RoadGraph;

class RoadPreprocessor {
public:
	RoadPreprocessor();

	void preprocess(QString filename, const QVector2D &lonlat, const BBox &range);

private:
	void writeGSM(FILE* fp, RoadGraph *roads);
};

