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

#ifndef INCLUDED_SCENARIOEDITOR
#define INCLUDED_SCENARIOEDITOR

#include "wx/toolbar.h"

#include "General/AtlasWindowCommandProc.h"
#include "General/Observable.h"
#include "Tools/Common/ObjectSettings.h"
#include "Tools/Common/Tools.h"
#include "CustomControls/FileHistory/FileHistory.h"
#include "SectionLayout.h"

class ScriptInterface;

class ScenarioEditor : public wxFrame
{
public:
	ScenarioEditor(wxWindow* parent, ScriptInterface& scriptInterface);
	void OnClose(wxCloseEvent& event);
	void OnTimer(wxTimerEvent& event);
	void OnIdle(wxIdleEvent& event);
	wxToolBar* OnCreateToolBar(long style, wxWindowID id, const wxString &name);
	
 	void OnNew(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnMRUFile(wxCommandEvent& event);

	void OnQuit(wxCommandEvent& event);
	void OnUndo(wxCommandEvent& event);
	void OnRedo(wxCommandEvent& event);

	void OnWireframe(wxCommandEvent& event);
	void OnMessageTrace(wxCommandEvent& event);
	void OnScreenshot(wxCommandEvent& event);
	void OnMediaPlayer(wxCommandEvent& event);
	void OnJavaScript(wxCommandEvent& event);
	void OnCameraReset(wxCommandEvent& event);
	void OnRenderPath(wxCommandEvent& event);
	void OnDumpState(wxCommandEvent& event);

	bool OpenFile(const wxString& name, const wxString& filename);

	void NotifyOnMapReload();

	static AtlasWindowCommandProc& GetCommandProc();

	static float GetSpeedModifier();

	ScriptInterface& GetScriptInterface() const { return m_ScriptInterface; }
	Observable<ObjectSettings>& GetObjectSettings() { return m_ObjectSettings; }
	Observable<AtObj>& GetMapSettings() { return m_MapSettings; }

	ToolManager& GetToolManager() { return m_ToolManager; }

	void SelectPage(const wxString& classname) { m_SectionLayout.SelectPage(classname); }

private:
	ScriptInterface& m_ScriptInterface;

	ToolManager m_ToolManager;

	wxTimer m_Timer;

	SectionLayout m_SectionLayout;

	Observable<ObjectSettings> m_ObjectSettings;
	Observable<AtObj> m_MapSettings;

	void SetOpenFilename(const wxString& filename);
	wxString m_OpenFilename;
	FileHistory m_FileHistory;

	DECLARE_EVENT_TABLE();
};

#endif // INCLUDED_SCENARIOEDITOR
