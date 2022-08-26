#include "..\Public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
{
	ZeroMemory(&m_tVIBInfo_Derived, sizeof(VIBINFO_DERIVED));
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
{
	memcpy(&m_tVIBInfo_Derived, &rhs.m_tVIBInfo_Derived, sizeof(VIBINFO_DERIVED));
}

void CVIBuffer_Terrain::Set_VIBInfoDerived(VIBINFO_DERIVED & tVIBInfo_Derived)
{
	m_tVIBInfo_Derived.m_iNumVerticesX = tVIBInfo_Derived.m_iNumVerticesX;
	m_tVIBInfo_Derived.m_iNumVerticesZ = tVIBInfo_Derived.m_iNumVerticesZ;
}

HRESULT CVIBuffer_Terrain::Load_Terrain(void)
{
	m_tVIBInfo.m_iNumVertices = m_tVIBInfo_Derived.m_iNumVerticesX * m_tVIBInfo_Derived.m_iNumVerticesZ;
	m_tVIBInfo.m_iStride = m_tVIBInfo.m_iStride;
	m_tVIBInfo.m_dwFVF = m_tVIBInfo.m_dwFVF;
	m_tVIBInfo.m_ePrimitiveType = m_tVIBInfo.m_ePrimitiveType;
	m_tVIBInfo.m_iNumPrimitive = (m_tVIBInfo_Derived.m_iNumVerticesX - 1) * (m_tVIBInfo_Derived.m_iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Ready_Vertex_Buffer()))
		return E_FAIL;

	m_tVIBInfo.m_iIndicesByte = m_tVIBInfo.m_iIndicesByte;
	m_tVIBInfo.m_eIndexFormat = m_tVIBInfo.m_eIndexFormat;

	if (FAILED(__super::Ready_Index_Buffer()))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Terrain::Release_Buffer(void)
{
	m_pVB->Release();
	m_pIB->Release();
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_tVIBInfo_Derived.m_iNumVerticesX = iNumVerticesX;
	m_tVIBInfo_Derived.m_iNumVerticesZ = iNumVerticesZ;

	m_tVIBInfo.m_iNumVertices = m_tVIBInfo_Derived.m_iNumVerticesX * m_tVIBInfo_Derived.m_iNumVerticesZ;
	m_tVIBInfo.m_iStride = sizeof(VTXTEX);
	m_tVIBInfo.m_dwFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_tVIBInfo.m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_tVIBInfo.m_iNumPrimitive = (m_tVIBInfo_Derived.m_iNumVerticesX - 1) * (m_tVIBInfo_Derived.m_iNumVerticesZ - 1) * 2;

	if (FAILED(__super::Ready_Vertex_Buffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_tVIBInfo_Derived.m_iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_tVIBInfo_Derived.m_iNumVerticesX; ++j)
		{
			_uint iIndex = i * m_tVIBInfo_Derived.m_iNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, 0.f, (_float)i);
			pVertices[iIndex].vTexture = _float2((_float)j, (_float)i);
		}
	}

	m_pVB->Unlock();

	m_tVIBInfo.m_iIndicesByte = sizeof(FACEINDICES32);
	m_tVIBInfo.m_eIndexFormat = D3DFMT_INDEX32;

	if (FAILED(__super::Ready_Index_Buffer()))
		return E_FAIL;

	FACEINDICES32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint iNumFaces = 0;

	for (_uint i = 0; i < m_tVIBInfo_Derived.m_iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_tVIBInfo_Derived.m_iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * m_tVIBInfo_Derived.m_iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + m_tVIBInfo_Derived.m_iNumVerticesX,
				iIndex + m_tVIBInfo_Derived.m_iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[1];
			pIndices[iNumFaces]._2 = iIndices[2];
			++iNumFaces;

			pIndices[iNumFaces]._0 = iIndices[0];
			pIndices[iNumFaces]._1 = iIndices[2];
			pIndices[iNumFaces]._2 = iIndices[3];
			++iNumFaces;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain*	pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();

}
