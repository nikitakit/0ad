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

#ifndef INCLUDED_OBJECTENTRY
#define INCLUDED_OBJECTENTRY

class CModelAbstract;
class CSkeletonAnim;
class CObjectBase;
class CObjectManager;
struct SPropPoint;

#include <map>
#include <set>
#include <vector>

#include "lib/file/vfs/vfs_path.h"
#include "ps/CStr.h"
#include "ps/Overlay.h"

class CObjectEntry
{
public:
	CObjectEntry(CObjectBase* base);
	~CObjectEntry();

	// Construct this actor, using the specified variation selections
	bool BuildVariation(const std::vector<std::set<CStr> >& selections,
		const std::vector<u8>& variationKey, CObjectManager& objectManager);

	// Base actor. Contains all the things that don't change between
	// different variations of the actor.
	CObjectBase* m_Base;

	VfsPath m_TextureName;
	// model name
	VfsPath m_ModelName;
	// colour (used when doing alpha-channel colouring, but not doing player-colour)
	CColor m_Color;
		// (probable TODO: make colour a per-model thing, rather than per-objectEntry,
		// so we can have lots of colour variations without wasting memory on
		// lots of objectEntries)

	std::wstring m_ProjectileModelName;

	// Returns a randomly-chosen animation matching the given name.
	// If none is found, returns NULL.
	CSkeletonAnim* GetRandomAnimation(const CStr& animationName) const;

	// Returns all the animations matching the given name.
	std::vector<CSkeletonAnim*> GetAnimations(const CStr& animationName) const;

	// corresponding model
	CModelAbstract* m_Model;

	// Whether this object is outdated, due to hotloading of its base object.
	// (If true then CObjectManager won't reuse this object from its cache.)
	bool m_Outdated;

private:
	typedef std::multimap<CStr, CSkeletonAnim*> SkeletonAnimMap;
	SkeletonAnimMap m_Animations;
		// TODO: something more memory-efficient than storing loads of similar strings for each unit?
};


#endif
