#pragma once

#include <qstring.h>
#include <qvector2d.h>

class Texture {
private:
	QString fileName;
	//float size;
	QVector2D size;
	unsigned int id;

public:
	Texture(unsigned int id, QString filename, const QVector2D& size);
	~Texture();

	QString getFileName() { return fileName; }
	//void setFileName(QString fileName) { this->fileName = fileName; }
	float getSizeX() { return size.x(); }
	float getSizeY() { return size.y(); }
	void setSize(float x, float y) { this->size.setX(x); this->size.setY(y); }
	unsigned int getId() { return id; }
	//void setId(unsigned int urn id) { this->urn id = urn id; }

	//void bind(bool repeat = false);
	//void bind(float xSize, float ySize, bool repeat = false);
};

