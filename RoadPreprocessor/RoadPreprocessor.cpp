#include <iostream>
#include <QFile>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <road/GraphUtil.h>
#include <road/RoadGraph.h>
#include <road/RoadVertex.h>
#include <road/RoadEdge.h>
#include "RoadPreprocessor.h"
#include "OSMRoadsParser.h"

RoadPreprocessor::RoadPreprocessor() {
}

void RoadPreprocessor::preprocess(QString filename, const QVector2D &lonlat, const BBox &range) {
	RoadGraph roads;

	OSMRoadsParser parser(&roads, lonlat, range);

	// read OSM file
	QXmlSimpleReader reader;
	reader.setContentHandler(&parser);
	QFile file(filename);
	QXmlInputSource source(&file);
	reader.parse(source);

	// degreeが2の頂点は、全てエッジの一部にしちゃう。
	GraphUtil::reduce(roads);
	GraphUtil::clean(roads);

	// write OSM file
	QString output = filename.split("/").last().split(".").at(0) + ".gsm";
	GraphUtil::saveRoads(roads, output);
}

