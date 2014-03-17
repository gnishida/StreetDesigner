#include <vector>
#include <iostream>
#include <QVector2D>
#include <QFile>
#include <QString>
#include <time.h>
#include <road/RoadGraph.h>
#include <common/BBox.h>
#include "RoadPreprocessor.h"

using namespace std;

/**
 * argv[1]	the OSM file name
 * argv[2]	the file name that contains the ranges of the latitude and longitude
 */
int main(int argc, char** argv) {
	if (argc < 6) {
		cerr << "Usage: RoadPreprocessor <OSM file> <longitude> <latitude> <width> <height>" << endl;
		return 1;
	}

	QFile file(argv[1]);
	if (!file.open(QIODevice::ReadOnly)) {
		cerr << "File is not accessible: " << argv[1] << endl;
		return 1;
	}

	// タイマー計測開始
	time_t start = clock();

	float longitude = atof(argv[2]);
	float latitude = atof(argv[3]);
	int width = atoi(argv[4]);
	int height = atoi(argv[5]);

	// 範囲をセット
	BBox area;
	area.addPoint(QVector2D(-width/2, -height/2));
	area.addPoint(QVector2D(width/2, height/2));

	RoadPreprocessor rp;
	rp.preprocess(argv[1], QVector2D(longitude, latitude), area);

	// タイマー計測終了
	time_t end = clock();

	std::cout << "Elapsed: " << (double)(end-start)/CLOCKS_PER_SEC << " [sec]" << std::endl;

	return 0;
}