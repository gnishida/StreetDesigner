/*********************************************************************
This file is part of QtUrban.

    QtUrban is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    QtUrban is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QtUrban.  If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#pragma once

#include "glew.h"
#include <vector>
#include "camera_3d.h"
#include "TextureManager.h"

namespace mylib {

class Texture;

class SimpleSkyBox {
private:
	static std::vector<Texture*> textures;
	float sideLength;

public:
	SimpleSkyBox(float sideLength);
	~SimpleSkyBox();

	void render(Camera3D* camera, TextureManager* textureManager);
};

} // namespace mylib