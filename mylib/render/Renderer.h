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

#include <QVector2D>
#include "../common/Polyline3D.h"
#include "GeometryObject.h"

namespace mylib {

class TextureManager;

class Renderer {
public:
	Renderer();
	~Renderer();

	void render(GeometryObject* object, TextureManager* textureManager);

protected:
	void _render(GeometryObject* object, TextureManager* textureManager);
};

} // namespace mylib