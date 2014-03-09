#include "Texture.h"

Texture::Texture(unsigned int id, QString filename, const QVector2D& size) {
	this->id = id;
	this->fileName = filename;
	this->size = size;
}


Texture::~Texture() {
}

/*void Texture::bind(bool repeat) {
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode (GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind(float xSize, float ySize, bool repeat) {
	bool scale = false;
	float scaleX = 1.0f;
	float scaleY = 1.0f;

	if (xSize != size || ySize != size) {
		scale = true;
		scaleX = size / xSize;
		scaleY = size / ySize;
	}

	glMatrixMode (GL_TEXTURE);
	//glPushMatrix();
	glLoadIdentity();
	if (scale) {
		glScalef(scaleX, scaleY, 1);
	}
	glMatrixMode (GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, id);
}*/

