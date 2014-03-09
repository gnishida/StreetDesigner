#pragma once

#include <QString>
#include <QHash>

class QGLWidget;

class Texture;

class TextureManager {
protected:
	QGLWidget* glWidget;
	//std::vector<Texture*> textures;
	QHash<QString, Texture*> textures;

protected:
	TextureManager(QGLWidget* glWidget);
	~TextureManager();
	TextureManager(const TextureManager& ref);
	TextureManager& operator=(const TextureManager& ref);

public:
	static TextureManager* getInstance(QGLWidget* glWidget);
	void clear();
	void initializeTextures(QGLWidget* glWidget, QString filename);
	Texture* get(QString name);
};
