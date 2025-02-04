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

#include "precompiled.h"

#include "BinarySerializer.h"

#include "SerializedScriptTypes.h"

#include "lib/alignment.h"
#include "ps/CLogger.h"

#include "scriptinterface/ScriptInterface.h"
#include "scriptinterface/ScriptExtraHeaders.h" // for JSDOUBLE_IS_INT32

CBinarySerializerScriptImpl::CBinarySerializerScriptImpl(ScriptInterface& scriptInterface, ISerializer& serializer) :
	m_ScriptInterface(scriptInterface), m_Serializer(serializer), m_Rooter(m_ScriptInterface),
	m_ScriptBackrefsArena(8*MiB), m_ScriptBackrefs(backrefs_t::key_compare(), ScriptBackrefsAlloc(m_ScriptBackrefsArena)), m_ScriptBackrefsNext(1)
{
}

void CBinarySerializerScriptImpl::HandleScriptVal(jsval val)
{
	JSContext* cx = m_ScriptInterface.GetContext();

	switch (JS_TypeOfValue(cx, val))
	{
	case JSTYPE_VOID:
	{
		m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_VOID);
		break;
	}
	case JSTYPE_NULL: // This type is never actually returned (it's a JS2 feature)
	{
		m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_NULL);
		break;
	}
	case JSTYPE_OBJECT:
	{
		if (JSVAL_IS_NULL(val))
		{
			m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_NULL);
			break;
		}

		JSObject* obj = JSVAL_TO_OBJECT(val);

		// If we've already serialized this object, just output a reference to it
		u32 tag = GetScriptBackrefTag(obj);
		if (tag)
		{
			m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_BACKREF);
			m_Serializer.NumberU32_Unbounded("tag", tag);
			break;
		}

		if (JS_IsArrayObject(cx, obj))
		{
			m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_ARRAY);
			// TODO: probably should have a more efficient storage format
		}
		else
		{
			m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_OBJECT);

			//			if (JS_GetClass(cx, obj))
			//			{
			//				LOGERROR("Cannot serialise JS objects of type 'object' with a class");
			//				throw PSERROR_Serialize_InvalidScriptValue();
			//			}
			// TODO: ought to complain only about non-standard classes
			// TODO: probably ought to do something cleverer for classes, prototypes, etc
			// (See Trac #406, #407)
		}

		// Find all properties (ordered by insertion time)

		// (Note that we don't do any rooting, because we assume nothing is going to trigger GC.
		// I'm not absolute certain that's necessarily a valid assumption.)

		AutoJSIdArray ida (cx, JS_Enumerate(cx, obj));
		if (!ida.get())
			throw PSERROR_Serialize_ScriptError("JS_Enumerate failed");

		m_Serializer.NumberU32_Unbounded("num props", (uint32_t)ida.length());

		for (size_t i = 0; i < ida.length(); ++i)
		{
			jsid id = ida[i];

			jsval idval, propval;

			// Get the property name as a string
			if (!JS_IdToValue(cx, id, &idval))
				throw PSERROR_Serialize_ScriptError("JS_IdToValue failed");
			JSString* idstr = JS_ValueToString(cx, idval);
			if (!idstr)
				throw PSERROR_Serialize_ScriptError("JS_ValueToString failed");

			ScriptString("prop name", idstr);

			// Use LookupProperty instead of GetProperty to avoid the danger of getters
			// (they might delete values and trigger GC)
			if (!JS_LookupPropertyById(cx, obj, id, &propval))
				throw PSERROR_Serialize_ScriptError("JS_LookupPropertyById failed");

			HandleScriptVal(propval);
		}

		break;
	}
	case JSTYPE_FUNCTION:
	{
		LOGERROR(L"Cannot serialise JS objects of type 'function'");
		throw PSERROR_Serialize_InvalidScriptValue();
	}
	case JSTYPE_STRING:
	{
		m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_STRING);
		ScriptString("string", JSVAL_TO_STRING(val));
		break;
	}
	case JSTYPE_NUMBER:
	{
		// For efficiency, handle ints and doubles separately.
		if (JSVAL_IS_INT(val))
		{
			m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_INT);
			m_Serializer.NumberI32_Unbounded("value", (int32_t)JSVAL_TO_INT(val));
		}
		else
		{
			ENSURE(JSVAL_IS_DOUBLE(val));

			// If the value fits in an int, serialise as an int
			jsdouble d = JSVAL_TO_DOUBLE(val);
			int32_t i;
			if (JSDOUBLE_IS_INT32(d, &i))
			{
				m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_INT);
				m_Serializer.NumberI32_Unbounded("value", i);
			}
			// Otherwise serialise as a double
			else
			{
				m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_DOUBLE);
				m_Serializer.NumberDouble_Unbounded("value", d);
			}
		}
		break;
	}
	case JSTYPE_BOOLEAN:
	{
		m_Serializer.NumberU8_Unbounded("type", SCRIPT_TYPE_BOOLEAN);
		JSBool b = JSVAL_TO_BOOLEAN(val);
		m_Serializer.NumberU8_Unbounded("value", b ? 1 : 0);
		break;
	}
	case JSTYPE_XML:
	{
		LOGERROR(L"Cannot serialise JS objects of type 'xml'");
		throw PSERROR_Serialize_InvalidScriptValue();
	}
	default:
	{
		debug_warn(L"Invalid TypeOfValue");
		throw PSERROR_Serialize_InvalidScriptValue();
	}
	}
}

void CBinarySerializerScriptImpl::ScriptString(const char* name, JSString* string)
{
	JSContext* cx = m_ScriptInterface.GetContext();
	size_t length;
	const jschar* chars = JS_GetStringCharsAndLength(cx, string, &length);

	if (!chars)
		throw PSERROR_Serialize_ScriptError("JS_GetStringCharsAndLength failed");

#if BYTE_ORDER != LITTLE_ENDIAN
#error TODO: probably need to convert JS strings to little-endian
#endif

	// Serialize strings directly as UTF-16, to avoid expensive encoding conversions
	m_Serializer.NumberU32_Unbounded("string length", (uint32_t)length);
	m_Serializer.RawBytes(name, (const u8*)chars, length*2);
}

u32 CBinarySerializerScriptImpl::GetScriptBackrefTag(JSObject* obj)
{
	// To support non-tree structures (e.g. "var x = []; var y = [x, x];"), we need a way
	// to indicate multiple references to one object(/array). So every time we serialize a
	// new object, we give it a new non-zero tag; when we serialize it a second time we just
	// refer to that tag.
	//
	// The tags are stored in a map. Maybe it'd be more efficient to store it inline in the object
	// somehow? but this works okay for now

	std::pair<backrefs_t::iterator, bool> it = m_ScriptBackrefs.insert(std::make_pair(obj, m_ScriptBackrefsNext));

	// If it was already there, return the tag
	if (!it.second)
		return it.first->second;

	// If it was newly inserted, we need to make sure it gets rooted
	// for the duration that it's in m_ScriptBackrefs
	m_Rooter.Push(it.first->first);
	m_ScriptBackrefsNext++;
	// Return a non-tag number so callers know they need to serialize the object
	return 0;
}
