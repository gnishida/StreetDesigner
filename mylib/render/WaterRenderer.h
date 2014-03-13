/* ============================================================================
**
** Demonstration of water rendering
** Copyright (C) 2005  Julien Guertault
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
** ========================================================================= */

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

namespace mylib {

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

} // namespace mylib
