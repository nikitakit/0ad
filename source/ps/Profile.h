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

/*
 * GPG3-style hierarchical profiler
 */

#ifndef INCLUDED_PROFILE
#define INCLUDED_PROFILE

#include <vector>

#include "lib/adts/ring_buf.h"
#include "ps/Singleton.h"
#include "ps/ThreadUtil.h"

#define PROFILE_AMORTIZE_FRAMES 30
#define PROFILE_AMORTIZE_TURNS 1

class CProfileManager;
class CProfileNodeTable;

class CStr8;
class CStrW;

class CProfileNode
{
	friend class CProfileManager;
	friend class CProfileNodeTable;
	
	const char* name;

	int calls_frame_current;
	int calls_turn_current;
	RingBuf<int, PROFILE_AMORTIZE_FRAMES> calls_per_frame;
	RingBuf<int, PROFILE_AMORTIZE_TURNS> calls_per_turn;

	double time_frame_current;
	double time_turn_current;
	RingBuf<double, PROFILE_AMORTIZE_FRAMES> time_per_frame;
	RingBuf<double, PROFILE_AMORTIZE_TURNS> time_per_turn;

	long mallocs_frame_current;
	long mallocs_turn_current;
	RingBuf<long, PROFILE_AMORTIZE_FRAMES> mallocs_per_frame;
	RingBuf<long, PROFILE_AMORTIZE_TURNS> mallocs_per_turn;

	double start;
	long start_mallocs;
	int recursion;

	CProfileNode* parent;
	std::vector<CProfileNode*> children;
	std::vector<CProfileNode*> script_children;
	CProfileNodeTable* display_table;

public:
	typedef std::vector<CProfileNode*>::iterator profile_iterator;
	typedef std::vector<CProfileNode*>::const_iterator const_profile_iterator;

	CProfileNode( const char* name, CProfileNode* parent );
	~CProfileNode();

	const char* GetName() const { return name; }

	double GetFrameCalls() const;
	double GetFrameTime() const;
	double GetTurnCalls() const;
	double GetTurnTime() const;
	double GetFrameMallocs() const;
	double GetTurnMallocs() const;

	const CProfileNode* GetChild( const char* name ) const;
	const CProfileNode* GetScriptChild( const char* name ) const;
	const std::vector<CProfileNode*>* GetChildren() const { return( &children ); }
	const std::vector<CProfileNode*>* GetScriptChildren() const { return( &script_children ); }

	bool CanExpand();

	CProfileNode* GetChild( const char* name );
	CProfileNode* GetScriptChild( const char* name );
	CProfileNode* GetParent() const { return( parent ); }

	// Resets timing information for this node and all its children
	void Reset();
	// Resets frame timings for this node and all its children
	void Frame();
	// Resets turn timings for this node and all its children
	void Turn();
	// Enters the node
	void Call();
	// Leaves the node. Returns true if the node has actually been left
	bool Return();
};

class CProfileManager : public Singleton<CProfileManager>
{
	CProfileNode* root;
	CProfileNode* current;

	bool needs_structural_reset;

	void PerformStructuralReset();

public:
	CProfileManager();
	~CProfileManager();

	// Begins timing for a named subsection
	void Start( const char* name );
	void StartScript( const char* name );

	// Ends timing for the current subsection
	void Stop();

	// Resets all timing information
	void Reset();
	// Resets frame timing information
	void Frame();
	// Resets turn timing information
	// (Must not be called before Frame)
	void Turn();
	// Resets absolutely everything, at the end of this frame
	void StructuralReset();

	inline const CProfileNode* GetCurrent() { return( current ); }
	inline const CProfileNode* GetRoot() { return( root ); }
};

#define g_Profiler CProfileManager::GetSingleton()

class CProfileSample
{
public:
	CProfileSample( const char* name )
	{
		if (CProfileManager::IsInitialised())
		{
			// The profiler is only safe to use on the main thread
			ENSURE(ThreadUtil::IsMainThread());

			g_Profiler.Start( name );
		}
	}
	~CProfileSample()
	{
		if (CProfileManager::IsInitialised())
			g_Profiler.Stop();
	}
};

class CProfileSampleScript
{
public:
	CProfileSampleScript( const char* name )
	{
		if (CProfileManager::IsInitialised())
		{
			// The profiler is only safe to use on the main thread,
			// but scripts get run on other threads too so we need to
			// conditionally enable the profiler.
			// (This usually only gets used in debug mode so performance
			// doesn't matter much.)
			if (ThreadUtil::IsMainThread())
				g_Profiler.StartScript( name );
		}
	}
	~CProfileSampleScript()
	{
		if (CProfileManager::IsInitialised())
			if (ThreadUtil::IsMainThread())
				g_Profiler.Stop();
	}
};

// Put a PROFILE( xyz ) block at the start of all code to be profiled.
// Profile blocks last until the end of the containing scope.
#define PROFILE( name ) CProfileSample __profile( name )
// Cheat a bit to make things slightly easier on the user
#define PROFILE_START( name ) { CProfileSample __profile( name )
#define PROFILE_END( name ) }

#endif // INCLUDED_PROFILE
