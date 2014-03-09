#pragma once

#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <QTime>

#define		RESOLUTION 125

static int	normals = 0;
static int	xold = 0;
static int	yold = 0;
static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;

static float	surface[6 * (RESOLUTION + 1) * (RESOLUTION + 1)];
static float	normal[6 * (RESOLUTION + 1) * (RESOLUTION + 1)];

static bool water_initialized = false;

class TextureManager;
class Texture;

class WaterRenderer {
private:
	int width;
	int depth;
	float elevation;
	Texture* texture;

public:
	WaterRenderer(int width, int depth, float elevation);
	~WaterRenderer();

	void setWidth(int width) { this->width = width; }
	void setDepth(int depth) { this->depth = depth; }

	//void initializeTextures(TextureManager* textureManager);

	/*
	** Function called to update rendering
	*/
	void renderMe(TextureManager* textureManager);

private:
	float z(const float x, const float y, const float t);

	/*
	** Function to load a Jpeg file.
	*/
	//int	load_texture (const char * filename, unsigned char * dest, const int format, const unsigned int size);		
};

