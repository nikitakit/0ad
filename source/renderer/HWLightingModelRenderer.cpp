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

#include "precompiled.h"

#include "lib/ogl.h"
#include "maths/Vector3D.h"
#include "maths/Vector4D.h"

#include "graphics/Color.h"
#include "graphics/LightEnv.h"
#include "graphics/Model.h"
#include "graphics/ModelDef.h"

#include "renderer/HWLightingModelRenderer.h"
#include "renderer/Renderer.h"
#include "renderer/RenderModifiers.h"
#include "renderer/VertexArray.h"


struct ShaderModelDef : public CModelDefRPrivate
{
	/// Indices are the same for all models, so share them
	VertexIndexArray m_IndexArray;

	ShaderModelDef(const CModelDefPtr& mdef);
};


ShaderModelDef::ShaderModelDef(const CModelDefPtr& mdef)
	: m_IndexArray(GL_STATIC_DRAW)
{
	m_IndexArray.SetNumVertices(mdef->GetNumFaces()*3);
	m_IndexArray.Layout();
	ModelRenderer::BuildIndices(mdef, m_IndexArray.GetIterator());
	m_IndexArray.Upload();
	m_IndexArray.FreeBackingStore();
}


struct ShaderModel
{
	/// Dynamic per-CModel vertex array
	VertexArray m_Array;

	/// Position and normals are recalculated on CPU every frame
	VertexArray::Attribute m_Position;
	VertexArray::Attribute m_Normal;

	/// UV is stored per-CModel in order to avoid space wastage due to alignment
	VertexArray::Attribute m_UV;

	ShaderModel() : m_Array(GL_DYNAMIC_DRAW) { }
};


struct ShaderModelRendererInternals
{
	/// Previously prepared modeldef
	ShaderModelDef* shadermodeldef;
};


// Construction and Destruction
ShaderModelRenderer::ShaderModelRenderer()
{
	m = new ShaderModelRendererInternals;
	m->shadermodeldef = 0;
}

ShaderModelRenderer::~ShaderModelRenderer()
{
	delete m;
}


// Build model data (and modeldef data if necessary)
void* ShaderModelRenderer::CreateModelData(CModel* model)
{
	CModelDefPtr mdef = model->GetModelDef();
	ShaderModelDef* shadermodeldef = (ShaderModelDef*)mdef->GetRenderData(m);

	if (!shadermodeldef)
	{
		shadermodeldef = new ShaderModelDef(mdef);
		mdef->SetRenderData(m, shadermodeldef);
	}

	// Build the per-model data
	ShaderModel* shadermodel = new ShaderModel;

	shadermodel->m_Position.type = GL_FLOAT;
	shadermodel->m_Position.elems = 3;
	shadermodel->m_Array.AddAttribute(&shadermodel->m_Position);

	shadermodel->m_UV.type = GL_FLOAT;
	shadermodel->m_UV.elems = 2;
	shadermodel->m_Array.AddAttribute(&shadermodel->m_UV);

	shadermodel->m_Normal.type = GL_FLOAT;
	shadermodel->m_Normal.elems = 3;
	shadermodel->m_Array.AddAttribute(&shadermodel->m_Normal);

	shadermodel->m_Array.SetNumVertices(mdef->GetNumVertices());
	shadermodel->m_Array.Layout();

	// Fill in static UV coordinates
	VertexArrayIterator<float[2]> UVit = shadermodel->m_UV.GetIterator<float[2]>();

	ModelRenderer::BuildUV(mdef, UVit);

	return shadermodel;
}


// Fill in and upload dynamic vertex array
void ShaderModelRenderer::UpdateModelData(CModel* model, void* data, int updateflags)
{
	ShaderModel* shadermodel = (ShaderModel*)data;

	if (updateflags & RENDERDATA_UPDATE_VERTICES)
	{
		// build vertices
		VertexArrayIterator<CVector3D> Position = shadermodel->m_Position.GetIterator<CVector3D>();
		VertexArrayIterator<CVector3D> Normal = shadermodel->m_Normal.GetIterator<CVector3D>();

		ModelRenderer::BuildPositionAndNormals(model, Position, Normal);

		// upload everything to vertex buffer
		shadermodel->m_Array.Upload();
	}
}


// Cleanup per-model data.
// Note that per-CModelDef data is deleted by the CModelDef itself.
void ShaderModelRenderer::DestroyModelData(CModel* UNUSED(model), void* data)
{
	ShaderModel* shadermodel = (ShaderModel*)data;

	delete shadermodel;
}


// Setup one rendering pass
void ShaderModelRenderer::BeginPass(int streamflags)
{
	ENSURE(streamflags == (streamflags & (STREAM_POS|STREAM_NORMAL|STREAM_UV0)));

	if (streamflags & STREAM_POS)
		glEnableClientState(GL_VERTEX_ARRAY);

	if (streamflags & STREAM_NORMAL)
		glEnableClientState(GL_NORMAL_ARRAY);

	if (streamflags & STREAM_UV0)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Cleanup one rendering pass
void ShaderModelRenderer::EndPass(int streamflags)
{
	if (streamflags & STREAM_POS)
		glDisableClientState(GL_VERTEX_ARRAY);

	if (streamflags & STREAM_NORMAL)
		glDisableClientState(GL_NORMAL_ARRAY);

	if (streamflags & STREAM_UV0)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	CVertexBuffer::Unbind();
}


// Prepare UV coordinates for this modeldef
void ShaderModelRenderer::PrepareModelDef(int UNUSED(streamflags), const CModelDefPtr& def)
{
	m->shadermodeldef = (ShaderModelDef*)def->GetRenderData(m);

	ENSURE(m->shadermodeldef);
}


// Render one model
void ShaderModelRenderer::RenderModel(int streamflags, CModel* model, void* data)
{
	CModelDefPtr mdldef = model->GetModelDef();
	ShaderModel* shadermodel = (ShaderModel*)data;

	u8* base = shadermodel->m_Array.Bind();
	GLsizei stride = (GLsizei)shadermodel->m_Array.GetStride();

	u8* indexBase = m->shadermodeldef->m_IndexArray.Bind();

	if (streamflags & STREAM_POS)
		glVertexPointer(3, GL_FLOAT, stride, base + shadermodel->m_Position.offset);

	if (streamflags & STREAM_NORMAL)
		glNormalPointer(GL_FLOAT, stride, base + shadermodel->m_Normal.offset);

	if (streamflags & STREAM_UV0)
		glTexCoordPointer(2, GL_FLOAT, stride, base + shadermodel->m_UV.offset);

	// render the lot
	size_t numFaces = mdldef->GetNumFaces();

	if (!g_Renderer.m_SkipSubmit)
	{
		pglDrawRangeElementsEXT(GL_TRIANGLES, 0, (GLuint)mdldef->GetNumVertices()-1,
					   (GLsizei)numFaces*3, GL_UNSIGNED_SHORT, indexBase);
	}

	// bump stats
	g_Renderer.m_Stats.m_DrawCalls++;
	g_Renderer.m_Stats.m_ModelTris += numFaces;
}

