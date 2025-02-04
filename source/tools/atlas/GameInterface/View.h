/* Copyright (C) 2010 Wildfire Games.
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

#ifndef INCLUDED_VIEW
#define INCLUDED_VIEW

#include <map>

#include "graphics/Camera.h"

#include "Messages.h"
#include "simulation2/system/Entity.h"

class CUnit;
class CSimulation2;

class ViewGame;
class ViewActor;

class View
{
public:
	virtual ~View();
	virtual void Update(float UNUSED(frameLength)) { };
	virtual void Render() { };
	virtual CCamera& GetCamera() = 0;
	virtual CSimulation2* GetSimulation2() { return NULL; }
	virtual entity_id_t GetEntityId(AtlasMessage::ObjectID obj) { return (entity_id_t)obj; }
	virtual bool WantsHighFramerate() { return false; }

	virtual void SetParam(const std::wstring& name, bool value);
	virtual void SetParam(const std::wstring& name, const AtlasMessage::Colour& value);
	virtual void SetParam(const std::wstring& name, const std::wstring& value);

	// These always return a valid (not NULL) object
	static View* GetView(int /*eRenderView*/ view);
	static View* GetView_None();
	static ViewGame* GetView_Game();
	static ViewActor* GetView_Actor();

	// Invalidates any View objects previously returned by this class
	static void DestroyViews();
};

//////////////////////////////////////////////////////////////////////////

class ViewNone : public View
{
public:
	virtual CCamera& GetCamera() { return dummyCamera; }
private:
	CCamera dummyCamera;
};

class SimState;

class ViewGame : public View
{
public:
	ViewGame();
	virtual ~ViewGame();
	virtual void Update(float frameLength);
	virtual void Render();
	virtual CCamera& GetCamera();
	virtual CSimulation2* GetSimulation2();
	virtual bool WantsHighFramerate();

	virtual void SetParam(const std::wstring& name, bool value);
	virtual void SetParam(const std::wstring& name, const std::wstring& value);

	void SetSpeedMultiplier(float speed);
	void SaveState(const std::wstring& label);
	void RestoreState(const std::wstring& label);
	std::wstring DumpState(bool binary);

private:
	float m_SpeedMultiplier;
	std::map<std::wstring, SimState*> m_SavedStates;
	std::string m_DisplayPassability;
};

class ActorViewer;

class ViewActor : public View
{
public:
	ViewActor();
	~ViewActor();

	virtual void Update(float frameLength);
	virtual void Render();
	virtual CCamera& GetCamera();
	virtual CSimulation2* GetSimulation2();
	virtual entity_id_t GetEntityId(AtlasMessage::ObjectID obj);
	virtual bool WantsHighFramerate();

	virtual void SetParam(const std::wstring& name, bool value);
	virtual void SetParam(const std::wstring& name, const AtlasMessage::Colour& value);

	void SetSpeedMultiplier(float speed);
	ActorViewer& GetActorViewer();

private:
	float m_SpeedMultiplier;
	CCamera m_Camera;
	ActorViewer* m_ActorViewer;
};

#endif // INCLUDED_VIEW
