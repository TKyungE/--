#include "stdafx.h"
#include "..\Public\BackGroundTree.h"

#include "GameInstance.h"

CBackGroundTree::CBackGroundTree(LPDIRECT3DDEVICE9 _pGraphic_Device)
	:CGameObject(_pGraphic_Device)
{
}

CBackGroundTree::CBackGroundTree(const CBackGroundTree & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGroundTree::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT CBackGroundTree::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	_float3 vPos = _float3{ 5.f,0.f,5.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pTransformCom->Set_Scaled(_float3(1.f,1.f,1.f));

	return S_OK;
}

void CBackGroundTree::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	OnTerrain();
}

void CBackGroundTree::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CBackGroundTree::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	m_pVIBuffer->Render();

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGroundTree::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BackGroundTree"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGroundTree::SetUp_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	return S_OK;
}

HRESULT CBackGroundTree::Release_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

void CBackGroundTree::OnTerrain(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return;

	Safe_AddRef(pGameInstance);
	CVIBuffer_Terrain* pVIBuffer_Terrain = (CVIBuffer_Terrain*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == pVIBuffer_Terrain)
		return;

	CTransform* pTransform_Terrain = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform"), 0);
	if (nullptr == pTransform_Terrain)
		return;

	_float3 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition.y = pVIBuffer_Terrain->Compute_Height(vPosition, pTransform_Terrain->Get_WorldMatrix(), 0.5f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	Safe_Release(pGameInstance);
}
CBackGroundTree * CBackGroundTree::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CBackGroundTree* pInstance = new CBackGroundTree(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CBackGroundTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGroundTree::Clone(void * pArg)
{
	CBackGroundTree* pInstance = new CBackGroundTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CBackGroundTree"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CBackGroundTree::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();
}

void CBackGroundTree::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pTextureCom);
}
