#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
	ZeroMemory(&m_tTerrainInfo, sizeof(TERRAININFO));
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
	memcpy(&m_tTerrainInfo, &rhs.m_tTerrainInfo, sizeof(TERRAININFO));
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(m_pOnTerrain->Set_TerrainVIBuffer(m_pVIBufferCom)))
		return E_FAIL;
	
	//여기서 링크를 가져와서 대입하는 방식이 현명해보임,, 밑에는 임시방편

	if(FAILED(OnLoadData(TEXT("../../Data/Test.dat"))))
	{ 
		ERR_MSG(TEXT("Failed to OnLoadData"));
		return E_FAIL;
	}



	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);

	//if (pInstance->Get_DIMKeyState(DIMK_LBUTTON) < 0)
	//{
		_float4x4 matWorld = Get_World();
		D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

		LPDIRECT3DVERTEXBUFFER9 VB = m_pVIBufferCom->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = m_pVIBufferCom->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);
		IB->Lock(0, 0, (void**)&pIndices, 0);

		for (_uint i = 0; i < m_pVIBufferCom->Get_VIBInfo().m_iNumPrimitive; ++i)
		{
			_float3 LU = pVertices[pIndices[i]._0].vPosition;
			_float3 RU = pVertices[pIndices[i]._1].vPosition;
			_float3 RD = pVertices[pIndices[i]._2].vPosition;

			if (!FAILED(pInstance->Intersect(matWorld, &LU, &RU, &RD)))
			{
				/*_tchar m_szFPS[MAX_PATH] = L"";
				wsprintf(m_szFPS, L"좌표 : %d, %d, %d", (int)pInstance->Get_TargetPos().x, (int)pInstance->Get_TargetPos().y, (int)pInstance->Get_TargetPos().z);
				ERR_MSG(m_szFPS);*/

			//	pVertices[pIndices[i]._0].vPosition.y += 1.f;
			//	pVertices[pIndices[i]._1].vPosition.y += 1.f;
			//	pVertices[pIndices[i]._2].vPosition.y += 1.f;
				break;
			}
		}

		VB->Unlock();
		IB->Unlock();
	//}

	Safe_Release(pInstance);
}

void CTerrain::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	
	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::OnLoadData(const _tchar* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;

	CGameInstance* pInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pInstance);

	//CTerrain* pTerrain = dynamic_cast<CTerrain*>(pInstance->Find_Object(TEXT("Layer_BackGround"), 0));
	/*if (nullptr == pTerrain)
	{
		ERR_MSG(TEXT("Failed to Load"));
		return;
	}*/

	CVIBuffer::VIBINFO VIBInfo;
	CVIBuffer_Terrain::VIBINFO_DERIVED VIBInfo_Derived;
	ZeroMemory(&VIBInfo, sizeof(CVIBuffer::VIBINFO));
	ZeroMemory(&VIBInfo_Derived, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED));

	ReadFile(hFile, &VIBInfo, sizeof(CVIBuffer::VIBINFO), &dwByte, nullptr);
	ReadFile(hFile, &VIBInfo_Derived, sizeof(CVIBuffer_Terrain::VIBINFO_DERIVED), &dwByte, nullptr);


	m_pVIBufferCom->Release_Buffer();
	m_pVIBufferCom->Set_VIBInfo(VIBInfo);
	m_pVIBufferCom->Set_VIBInfoDerived(VIBInfo_Derived);

	if (FAILED(m_pVIBufferCom->Load_Terrain()))
	{
		ERR_MSG(TEXT("Failed to Load Terrain"));
		return E_FAIL;
	}

	LPDIRECT3DVERTEXBUFFER9 VB = m_pVIBufferCom->Get_VB();
	LPDIRECT3DINDEXBUFFER9 IB = m_pVIBufferCom->Get_IB();

	VTXTEX* pVertices = nullptr;
	FACEINDICES32* pIndices = nullptr;

	VB->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < m_pVIBufferCom->Get_VIBInfo().m_iNumVertices; ++i)
	{
		_float3 vPos;
		_float2 vTex;
		ReadFile(hFile, vPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, vTex, sizeof(_float2), &dwByte, nullptr);

		pVertices[i].vPosition = vPos;
		pVertices[i].vTexture = vTex;
	}

	VB->Unlock();

	IB->Lock(0, 0, (void**)&pIndices, 0);

	for (_uint i = 0; i < m_pVIBufferCom->Get_VIBInfo().m_iNumPrimitive; ++i)
	{
		ReadFile(hFile, &pIndices[i]._0, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._1, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._2, sizeof(_uint), &dwByte, nullptr);
	}

	IB->Unlock();

	Safe_Release(pInstance);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Onterrain"), LEVEL_STATIC, TEXT("Prototype_Component_Onterrain"), (CComponent**)&m_pOnTerrain)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTerrain::SetUp_RenderState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;	

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return S_OK;
}

HRESULT CTerrain::Release_RenderState()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphic_Device->SetTexture(0, nullptr);

	return S_OK;
}

CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain*	pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone(void* pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTerrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CTerrain::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pOnTerrain);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTextureCom);
}
