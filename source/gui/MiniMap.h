/* Copyright (C) 2011 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_MINIMAP
#define INCLUDED_MINIMAP

#include "gui/GUI.h"

class CCamera;
class CTerrain;

class CMiniMap : public IGUIObject
{
    GUI_OBJECT(CMiniMap)
public:
    CMiniMap();
    virtual ~CMiniMap();
protected:
    virtual void Draw();

	/**
	 * @see IGUIObject#HandleMessage()
	 */
	virtual void HandleMessage(SGUIMessage &Message);

    // create the minimap textures
    void CreateTextures();

    // rebuild the terrain texture map
    void RebuildTerrainTexture();

    // destroy and free any memory and textures
    void Destroy();

	void SetCameraPos();

	void FireWorldClickEvent(int button, int clicks);

    // the terrain we are mini-mapping
    const CTerrain* m_Terrain;

	const CCamera* m_Camera;
	
	//Whether or not the mouse is currently down
	bool m_Clicking;
	bool m_Hovering;

    // minimap texture handles
    GLuint m_TerrainTexture;

    // texture data
    u32* m_TerrainData;

    // whether we need to regenerate the terrain texture
    bool m_TerrainDirty;

    ssize_t m_Width, m_Height;

    // map size
    ssize_t m_MapSize;

    // texture size
    GLsizei m_TextureSize;

	// 1.f if map is circular or 1.414f if square (to shrink it inside the circle)
	float m_MapScale;

	void DrawTexture(float coordMax, float angle, float x, float y, float x2, float y2, float z);

	void DrawViewRect();

	void GetMouseWorldCoordinates(float& x, float& z);

	float GetAngle();
};

#endif
