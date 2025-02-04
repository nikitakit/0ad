/* Copyright (C) 2009 Wildfire Games.
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

#include "precompiled.h"
#include "JSI_Sound.h"
#include "maths/Vector3D.h"

#include "lib/utf8.h"
#include "lib/res/sound/snd_mgr.h"
#include "lib/res/h_mgr.h"	// h_filename
#include "ps/Filesystem.h"


JSI_Sound::JSI_Sound(const VfsPath& pathname)
{
	m_Handle = snd_open(g_VFS, pathname);

	// if open failed, we still have to return a valid non-null object to
	// the script, so just reset the handle to 0 so all subsequent method
	// calls will do nothing
	if (m_Handle < 0)
	{
		m_Handle = 0;
		return;
	}

	(void)snd_set_pos(m_Handle, 0,0,0, true);
}

JSI_Sound::~JSI_Sound()
{
	(void)this->Free(0, 0, 0);
}


bool JSI_Sound::SetGain(JSContext* cx, uintN argc, jsval* argv)
{
	if (! m_Handle)
		return false;

	ENSURE(argc >= 1); // FIXME
	float gain;
	if (! ToPrimitive<float>(cx, argv[0], gain))
		return false;

	(void)snd_set_gain(m_Handle, gain);
	return true;
}

bool JSI_Sound::SetPitch(JSContext* cx, uintN argc, jsval* argv)
{
	if (! m_Handle)
		return false;

	ENSURE(argc >= 1); // FIXME
	float pitch;
	if (! ToPrimitive<float>(cx, argv[0], pitch))
		return false;

	(void)snd_set_pitch(m_Handle, pitch);
	return true;
}

bool JSI_Sound::SetPosition(JSContext* cx, uintN argc, jsval* argv)
{
	if (! m_Handle)
		return false;

	ENSURE(argc >= 1); // FIXME

	CVector3D pos;
	// absolute world coords
	if (ToPrimitive<CVector3D>(cx, argv[0], pos))
		(void)snd_set_pos(m_Handle, pos[0], pos[1], pos[2]);
	// relative, 0 offset - right on top of the listener
	// (we don't need displacement from the listener, e.g. always behind)
	else
		(void)snd_set_pos(m_Handle, 0,0,0, true);

	return true;
}


bool JSI_Sound::Fade(JSContext* cx, uintN argc, jsval* argv)
{
	if (! m_Handle)
		return false;

	ENSURE(argc >= 3); // FIXME
	float initial_gain, final_gain;
	float length;
	if (! (ToPrimitive<float>(cx, argv[0], initial_gain)
		&& ToPrimitive<float>(cx, argv[1], final_gain)
		&& ToPrimitive<float>(cx, argv[2], length)))
		return false;

	(void)snd_fade(m_Handle, initial_gain, final_gain, length, FT_S_CURVE);

	// HACK: snd_fade causes <m_Handle> to be automatically freed when a
	// fade to 0 has completed. however, we're still holding on to a
	// reference, which will cause a double-free warning when Free() is
	// called from the dtor. solution is to neuter our Handle by
	// setting it to 0 (ok since it'll be freed). this does mean that
	// no further operations can be carried out during that final fade.
	if (final_gain == 0.0f)
		m_Handle = 0;

	return true;
}

// start playing the sound (one-shot).
// it will automatically be freed when done.
bool JSI_Sound::Play(JSContext* UNUSED(cx), uintN UNUSED(argc), jsval* UNUSED(argv))
{
	if (! m_Handle)
		return false;

	(void)snd_play(m_Handle);
	// We can't do anything else with this sound now, since it's impossible to
	// know whether or not it's still valid (since it might have finished playing
	// already). So set it to 0, so we don't try doing anything (like freeing it)
	// in the future.
	m_Handle = 0;
	return true;
}

// request the sound be played until free() is called. returns immediately.
bool JSI_Sound::Loop(JSContext* UNUSED(cx), uintN UNUSED(argc), jsval* UNUSED(argv))
{
	if (! m_Handle)
		return false;

	(void)snd_set_loop(m_Handle, true);
	(void)snd_play(m_Handle);
	return true;
}

// stop sound if currently playing and free resources.
// doesn't need to be called unless played via loop() -
// sounds are freed automatically when done playing.
bool JSI_Sound::Free(JSContext* UNUSED(cx), uintN UNUSED(argc), jsval* UNUSED(argv))
{
	if (! m_Handle)
		return false;

	(void)snd_free(m_Handle);	// resets it to 0
	return true;
}


// Script-bound functions


void JSI_Sound::ScriptingInit()
{
	AddMethod<CStr, &JSI_Sound::ToString>("toString", 0);
	AddMethod<bool, &JSI_Sound::Play>("play", 0);
	AddMethod<bool, &JSI_Sound::Loop>("loop", 0);
	AddMethod<bool, &JSI_Sound::Free>("free", 0);
	AddMethod<bool, &JSI_Sound::SetGain>("setGain", 0);
	AddMethod<bool, &JSI_Sound::SetPitch>("setPitch", 0);
	AddMethod<bool, &JSI_Sound::SetPosition>("setPosition", 0);
	AddMethod<bool, &JSI_Sound::Fade>("fade", 0);

	CJSObject<JSI_Sound>::ScriptingInit("Sound", &JSI_Sound::Construct, 1);
}

CStr JSI_Sound::ToString(JSContext* UNUSED(cx), uintN UNUSED(argc), jsval* UNUSED(argv))
{
	return "[object Sound: " + (m_Handle ? utf8_from_wstring(h_filename(m_Handle).string()) : "(null)") + "]";
}

JSBool JSI_Sound::Construct(JSContext* cx, uintN argc, jsval* vp)
{
	JSU_REQUIRE_MIN_PARAMS(1);

	CStrW filename;
	if (! ToPrimitive<CStrW>(cx, JS_ARGV(cx, vp)[0], filename))
		return JS_FALSE;

	JSI_Sound* newObject = new JSI_Sound(filename);
	newObject->m_EngineOwned = false;
	JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(newObject->GetScript()));

	return JS_TRUE;
}
