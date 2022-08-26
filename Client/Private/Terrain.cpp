#include "stdafx.h"
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject(pGraphic_Device)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{
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

	m_tInfo.pTerrain = this;
	memcpy(pArg, &m_tInfo, sizeof(INFO));

	return S_OK;
}

void CTerrain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	CGameInstance* pInstance = CGameInstance::Get_Instance();

	if (nullptr == pInstance)
		return;

	Safe_AddRef(pInstance);

	if (pInstance->Get_DIMKeyState(DIMK_LBUTTON) < 0)
	{
		_float4x4 matWorld = Get_World();
		D3DXMatrixInverse(&matWorld, nullptr, &matWorld);

		LPDIRECT3DVERTEXBUFFER9 VB = m_pVIBufferCom->Get_VB();
		LPDIRECT3DINDEXBUFFER9 IB = m_pVIBufferCom->Get_IB();

		VTXTEX* pVertices = nullptr;
		FACEINDICES32* pIndices = nullptr;

		VB->Lock(0, 0, (void**)&pVertices, 0);
		IB->Lock(0, 0, (void**)&pIndices, 0);

		for (_uint i = 0; i < m_pVIBufferCom->Get_NumPrimitive(); ++i)
		{
			_float3 LU = pVertices[pIndices[i]._0].vPosition;
			_float3 RU = pVertices[pIndices[i]._1].vPosition;
			_float3 RD = pVertices[pIndices[i]._2].vPosition;

			if (!FAILED(pInstance->Intersect(matWorld, &LU, &RU, &RD)))
			{
				/*_tchar m_szFPS[MAX_PATH] = L"";
				wsprintf(m_szFPS, L"ÁÂÇ¥ : %d, %d, %d", (int)pInstance->Get_TargetPos().x, (int)pInstance->Get_TargetPos().y, (int)pInstance->Get_TargetPos().z);
				ERR_MSG(m_szFPS);*/

			//	pVertices[pIndices[i]._0].vPosition.y += 1.f;
			//	pVertices[pIndices[i]._1].vPosition.y += 1.f;
			//	pVertices[pIndices[i]._2].vPosition.y += 1.f;
				break;
			}
		}

		VB->Unlock();
		IB->Unlock();
	}

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

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(TEXT("Prototype_Component_Texture_Terrain"),0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain_CollTile"), (CComponent**)&m_pTextureCom)))
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
