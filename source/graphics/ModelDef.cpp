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

/*
 * Defines a raw 3d model.
 */

#include "precompiled.h"

#include "ModelDef.h"
#include "graphics/SkeletonAnimDef.h"
#include "ps/FileIo.h"
#include "maths/Vector4D.h"

CVector3D CModelDef::SkinPoint(const SModelVertex& vtx,
							   const CMatrix3D newPoseMatrices[])
{
	CVector3D result (0, 0, 0);

	for (int i = 0; i < SVertexBlend::SIZE && vtx.m_Blend.m_Bone[i] != 0xff; ++i)
	{
		result += newPoseMatrices[vtx.m_Blend.m_Bone[i]].Transform(vtx.m_Coords) * vtx.m_Blend.m_Weight[i];
	}

	return result;
}

CVector3D CModelDef::SkinNormal(const SModelVertex& vtx,
								const CMatrix3D newPoseMatrices[])
{
	// To be correct, the normal vectors apparently need to be multiplied by the
	// inverse of the transpose. Unfortunately inverses are slow.
	// If a matrix is orthogonal, M * M^T = I and so the inverse of the transpose
	// is the original matrix. But that's not entirely relevant here, because
	// the bone matrices include translation components and so they're not
	// orthogonal.
	// But that's okay because we have
	//   M = T * R
	// and want to find
	//   n' = (M^T^-1) * n
	//      = (T * R)^T^-1 * n
	//      = (R^T * T^T)^-1 * n
	//      = (T^T^-1 * R^T^-1) * n
	// R is indeed orthogonal so R^T^-1 = R. T isn't orthogonal at all.
	// But n is only a 3-vector, and from the forms of T and R (which have
	// lots of zeroes) I can convince myself that replacing T with T^T^-1 has no
	// effect on anything but the fourth component of M^T^-1 - and the fourth
	// component is discarded since it has no effect on n', and so we can happily
	// use n' = M*n.
	//
	// (This isn't very good as a proof, but it's better than assuming M is
	// orthogonal when it's clearly not.)

	CVector3D result (0, 0, 0);

	for (int i = 0; i < SVertexBlend::SIZE && vtx.m_Blend.m_Bone[i] != 0xff; ++i)
	{
		result += newPoseMatrices[vtx.m_Blend.m_Bone[i]].Rotate(vtx.m_Norm) * vtx.m_Blend.m_Weight[i];
	}
	
	// If there was more than one influence, the result is probably not going
	// to be of unit length (since it's a weighted sum of several independent
	// unit vectors), so we need to normalise it.
	// (It's fairly common to only have one influence, so it seems sensible to
	// optimise that case a bit.)
	if (vtx.m_Blend.m_Bone[1] != 0xff) // if more than one influence
		result.Normalize();

	return result;
}

void CModelDef::SkinPointsAndNormals(
		size_t numVertices,
		const VertexArrayIterator<CVector3D>& Position,
		const VertexArrayIterator<CVector3D>& Normal,
		const SModelVertex* vertices,
		const size_t* blendIndices,
		const CMatrix3D newPoseMatrices[])
{
	for (size_t j = 0; j < numVertices; ++j)
	{
		const SModelVertex& vtx = vertices[j];

		Position[j] = newPoseMatrices[blendIndices[j]].Transform(vtx.m_Coords);
		Normal[j] = newPoseMatrices[blendIndices[j]].Rotate(vtx.m_Norm);

		// If there was more than one influence, the result is probably not going
		// to be of unit length (since it's a weighted sum of several independent
		// unit vectors), so we need to normalise it.
		// (It's fairly common to only have one influence, so it seems sensible to
		// optimise that case a bit.)
		if (vtx.m_Blend.m_Bone[1] != 0xff) // if more than one influence
			Normal[j].Normalize();
	}
}

void CModelDef::BlendBoneMatrices(
		CMatrix3D boneMatrices[])
{
	for (size_t i = 0; i < m_NumBlends; ++i)
	{
		const SVertexBlend& blend = m_pBlends[i];
		CMatrix3D& boneMatrix = boneMatrices[m_NumBones + i];
		boneMatrix.Blend(boneMatrices[blend.m_Bone[0]], blend.m_Weight[0]);
		boneMatrix.AddBlend(boneMatrices[blend.m_Bone[1]], blend.m_Weight[1]);
		for (size_t j = 2; j < SVertexBlend::SIZE && blend.m_Bone[j] != 0xFF; ++j)
		{
			boneMatrix.AddBlend(boneMatrices[blend.m_Bone[j]], blend.m_Weight[j]);
		}
	}
}

// CModelDef Constructor
CModelDef::CModelDef() :
	m_NumVertices(0), m_pVertices(0), m_NumFaces(0), m_pFaces(0), m_NumBones(0), m_Bones(0),
	m_NumBlends(0), m_pBlends(0), m_pBlendIndices(0),
	m_Name(L"[not loaded]")
{
}

// CModelDef Destructor
CModelDef::~CModelDef()
{
	for(RenderDataMap::iterator it = m_RenderData.begin(); it != m_RenderData.end(); ++it)
		delete it->second;
	delete[] m_pVertices;
	delete[] m_pFaces;
	delete[] m_Bones;
	delete[] m_pBlends;
	delete[] m_pBlendIndices;
}

// FindPropPoint: find and return pointer to prop point matching given name; 
// return null if no match (case insensitive search)
const SPropPoint* CModelDef::FindPropPoint(const char* name) const
{
	for (size_t i = 0; i < m_PropPoints.size(); ++i)
		if (m_PropPoints[i].m_Name == name)
			return &m_PropPoints[i];

	return 0;
}

// Load: read and return a new CModelDef initialised with data from given file
CModelDef* CModelDef::Load(const VfsPath& filename, const VfsPath& name)
{
	CFileUnpacker unpacker;

	// read everything in from file
	unpacker.Read(filename,"PSMD");
			
	// check version
	if (unpacker.GetVersion()<FILE_READ_VERSION) {
		throw PSERROR_File_InvalidVersion();
	}

	std::auto_ptr<CModelDef> mdef (new CModelDef());
	mdef->m_Name = name;

	// now unpack everything 
	mdef->m_NumVertices = unpacker.UnpackSize();
	mdef->m_pVertices=new SModelVertex[mdef->m_NumVertices];
	unpacker.UnpackRaw(mdef->m_pVertices,sizeof(SModelVertex)*mdef->m_NumVertices);
	
	mdef->m_NumFaces = unpacker.UnpackSize();
	mdef->m_pFaces=new SModelFace[mdef->m_NumFaces];
	unpacker.UnpackRaw(mdef->m_pFaces,sizeof(SModelFace)*mdef->m_NumFaces);
	
	mdef->m_NumBones = unpacker.UnpackSize();
	if (mdef->m_NumBones)
	{
		mdef->m_Bones=new CBoneState[mdef->m_NumBones];
		unpacker.UnpackRaw(mdef->m_Bones,mdef->m_NumBones*sizeof(CBoneState));

		mdef->m_pBlendIndices = new size_t[mdef->m_NumVertices];
		std::vector<SVertexBlend> blends;
		for (size_t i = 0; i < mdef->m_NumVertices; i++)
		{
			const SVertexBlend &blend = mdef->m_pVertices[i].m_Blend;
			if (blend.m_Bone[1] == 0xFF)
			{
				mdef->m_pBlendIndices[i] = blend.m_Bone[0];
			}
			else
			{
				// If there's already a vertex using the same blend as this, then
				// reuse its entry from blends; otherwise add the new one to blends
				size_t j;
				for (j = 0; j < blends.size(); j++)
				{
					if (blend == blends[j]) break;
				}
				if (j >= blends.size())
					blends.push_back(blend);
				mdef->m_pBlendIndices[i] = mdef->m_NumBones + j;
			}
		}

		mdef->m_NumBlends = blends.size();
		mdef->m_pBlends = new SVertexBlend[mdef->m_NumBlends];
		std::copy(blends.begin(), blends.end(), mdef->m_pBlends);
	}

	if (unpacker.GetVersion() >= 2)
	{
		// versions >=2 also have prop point data
		size_t numPropPoints = unpacker.UnpackSize();
		mdef->m_PropPoints.resize(numPropPoints);
		if (numPropPoints)
		{
			for (size_t i = 0; i < numPropPoints; i++)
			{
				unpacker.UnpackString(mdef->m_PropPoints[i].m_Name);
				unpacker.UnpackRaw(&mdef->m_PropPoints[i].m_Position.X, sizeof(mdef->m_PropPoints[i].m_Position));
				unpacker.UnpackRaw(&mdef->m_PropPoints[i].m_Rotation.m_V.X, sizeof(mdef->m_PropPoints[i].m_Rotation));
				unpacker.UnpackRaw(&mdef->m_PropPoints[i].m_BoneIndex, sizeof(mdef->m_PropPoints[i].m_BoneIndex));

				// build prop point transform
				mdef->m_PropPoints[i].m_Transform.SetIdentity();
				mdef->m_PropPoints[i].m_Transform.Rotate(mdef->m_PropPoints[i].m_Rotation);
				mdef->m_PropPoints[i].m_Transform.Translate(mdef->m_PropPoints[i].m_Position);
			}
		}
	}

	if (unpacker.GetVersion() <= 2)
	{
		// Versions <=2 don't include the default 'root' prop point, so add it here

		SPropPoint prop;
		prop.m_Name = "root";
		prop.m_Transform.SetIdentity();
		prop.m_BoneIndex = 0xFF;

		mdef->m_PropPoints.push_back(prop);
	}

	if (unpacker.GetVersion() <= 2)
	{
		// Versions <=2 store the vertexes relative to the bind pose. That
		// isn't useful when you want to do correct skinning, so later versions
		// store them in world space. So, fix the old models by skinning each
		// vertex:

		if (mdef->m_NumBones) // only do skinned models
		{
			std::vector<CMatrix3D> bindPose (mdef->m_NumBones);

			for (size_t i = 0; i < mdef->m_NumBones; ++i)
			{
				bindPose[i].SetIdentity();
				bindPose[i].Rotate(mdef->m_Bones[i].m_Rotation);
				bindPose[i].Translate(mdef->m_Bones[i].m_Translation);
			}

			for (size_t i = 0; i < mdef->m_NumVertices; ++i)
			{
				mdef->m_pVertices[i].m_Coords = SkinPoint(mdef->m_pVertices[i], &bindPose[0]);
				mdef->m_pVertices[i].m_Norm = SkinNormal(mdef->m_pVertices[i], &bindPose[0]);
			}
		}
	}

	return mdef.release();
}

// Save: write the given CModelDef to the given file
void CModelDef::Save(const VfsPath& filename, const CModelDef* mdef)
{
	CFilePacker packer(FILE_VERSION, "PSMD");

	// pack everything up
	const size_t numVertices = mdef->GetNumVertices();
	packer.PackSize(numVertices);
	packer.PackRaw(mdef->GetVertices(), sizeof(SModelVertex) * numVertices);

	const size_t numFaces = mdef->GetNumFaces();
	packer.PackSize(numFaces);
	packer.PackRaw(mdef->GetFaces(), sizeof(SModelFace) * numFaces);
	
	const size_t numBones = mdef->m_NumBones;
	packer.PackSize(numBones);
	if (numBones)
		packer.PackRaw(mdef->m_Bones, sizeof(CBoneState) * numBones);

	const size_t numPropPoints = mdef->m_PropPoints.size();
	packer.PackSize(numPropPoints);
	for (size_t i = 0; i < numPropPoints; i++)
	{
		packer.PackString(mdef->m_PropPoints[i].m_Name);
		packer.PackRaw(&mdef->m_PropPoints[i].m_Position.X, sizeof(mdef->m_PropPoints[i].m_Position));
		packer.PackRaw(&mdef->m_PropPoints[i].m_Rotation.m_V.X, sizeof(mdef->m_PropPoints[i].m_Rotation));
		packer.PackRaw(&mdef->m_PropPoints[i].m_BoneIndex, sizeof(mdef->m_PropPoints[i].m_BoneIndex));
	}
	
	// flush everything out to file
	packer.Write(filename);
}

// SetRenderData: Set the render data object for the given key,
void CModelDef::SetRenderData(const void* key, CModelDefRPrivate* data)
{
	delete m_RenderData[key];
	m_RenderData[key] = data;
}

// GetRenderData: Get the render data object for the given key,
// or 0 if no such object exists.
// Reference count of the render data object is automatically increased.
CModelDefRPrivate* CModelDef::GetRenderData(const void* key) const
{
	RenderDataMap::const_iterator it = m_RenderData.find(key);
	
	if (it != m_RenderData.end())
		return it->second;
	
	return 0;
}
