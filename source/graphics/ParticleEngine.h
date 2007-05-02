/////////////////////////////////////////////////////
//	File Name:	ParticleEngine.h
//	Date:		6/29/05
//	Author:		Will Dull
//	Purpose:	The particle engine system.
//				controls and maintain particles
//				through emitters that are passed
//				into each of the main functions.
/////////////////////////////////////////////////////

#ifndef _PARTICLEENGINE_H_
#define _PARTICLEENGINE_H_

#include "ParticleEmitter.h"
#include "lib/res/graphics/tex.h"
#include "lib/res/graphics/ogl_tex.h"
#include "graphics/Texture.h"

#include "ps/CLogger.h"
#include "ps/Loader.h"

#include "lib/ogl.h"
#include "renderer/Renderer.h"

// Different textures
enum PText { DEFAULTTEXT, MAX_TEXTURES };
// Different emitters
enum PEmit { DEFAULTEMIT, MAX_EMIT };

class CParticleEngine
{
public:
	virtual ~CParticleEngine(void);


	/// @return instance of the singleton class
	static CParticleEngine* GetInstance();

	/// delete the instance of the singleton class
	static void DeleteInstance();

	/// @return true on success, false on failure
	bool InitParticleSystem(void);

	/**
	 * add the emitter to the engine's list.
	 * @return indicator of success.
	 **/
	bool AddEmitter(CEmitter *emitter, int type = DEFAULTTEXT, int ID = DEFAULTEMIT);

	/// @return emitter with the given ID or 0 if not found.
	CEmitter* FindEmitter(int ID);

	/**
	 * Check if the emitters are ready to be deleted and removed.
	 * If not, call Update() on them.
	 **/
	void UpdateEmitters();

	/// render each emitter and their particles
	void RenderParticles();

	/**
	 * destroy all active emitters on screen.
	 * @param fade if true, allows emitters to fade out. if false,
	 * they disappear instantly.
	 **/
	void DestroyAllEmitters(bool fade = true);

	/// do cleanup that's not done in the destructor.
	void Cleanup();

	void EnterParticleContext(void);
	void LeaveParticleContext(void);

	int GetTotalParticles() { return totalParticles; }
	void SetTotalParticles(int particles) { totalParticles = particles; }
	void AddToTotalParticles(int addAmount) { totalParticles += addAmount; }
	void SubToTotalParticles(int subAmount) { totalParticles -= subAmount; }

private:
	CParticleEngine(void);
	static CParticleEngine*  m_pInstance;    // The singleton instance

	CTexture idTexture[MAX_TEXTURES];
	int totalParticles;					// Total Amount of particles of all emitters.

	struct tEmitterNode
	{
		CEmitter *pEmitter;
		tEmitterNode *prev, *next;
		int ID;
	};

	tEmitterNode *m_pHead;
	friend class CEmitter;
};

#endif
