#include "stdafx.h"
#include "..\Public\RectHouse.h"

#include "GameInstance.h"

CRectHouse::CRectHouse(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CRectHouse::CRectHouse(const CRectHouse & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRectHouse::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRectHouse::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	//memcpy(&m_IndexPos, pArg, sizeof(INDEXPOS));

	SetPos();

	//m_pTransformCom1->Set_State(CTransform::STATE_POSITION, m_IndexPos.vPos);

	return S_OK;
}

void CRectHouse::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CRectHouse::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CRectHouse::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	if (FAILED(RectHouse_Render()))
		return E_FAIL;

	if (FAILED(Release_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRectHouse::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer1)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer3)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer3"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer4)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer4"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer5)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer5"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer6)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseRect"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseRect"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseRect"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseRect"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom1, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform1"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom2, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform2"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom3, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform3"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom4, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform4"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom5, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform5"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom6, &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CRectHouse::SetUp_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
	return S_OK;
}

HRESULT CRectHouse::Release_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetTexture(0, nullptr);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	return S_OK;
}

void CRectHouse::SetPos(void)
{
	_float3 vPos = _float3(1.f, 0.f, 1.f);
	
	_float Scale = 4.f;

	vPos.y += 0.5f;
	m_pTransformCom1->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom1->Set_State(CTransform::STATE_POSITION, vPos * Scale);


	m_pTransformCom2->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom2->Turn(_float3(0.f, 1.f, 0.f),1.f);
	_float3 vPos2 = _float3(0.477f, 0.f, 0.3425f);
	m_pTransformCom2->Set_State(CTransform::STATE_POSITION, m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos2* Scale);


	m_pTransformCom3->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom3->Turn(_float3(0.f, 1.f, 0.f),1.f);
	_float3 vPos3 = _float3(0.477f, 0.f, -0.3425f);
	m_pTransformCom3->Set_State(CTransform::STATE_POSITION, m_pTransformCom1->Get_State(CTransform::STATE_POSITION) - vPos3* Scale);

	m_pTransformCom4->Set_Scaled(_float3(Scale, Scale, Scale));
	_float3 vPos4 = _float3(0.f, 0.f, 0.685f);
	m_pTransformCom4->Set_State(CTransform::STATE_POSITION, m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos4* Scale);

	m_pTransformCom5->Set_Scaled(_float3(Scale - 0.195f, Scale, Scale));
	m_pTransformCom5->Turn(_float3(1.f, 0.f, 0.f),0.3f);
	_float3 vPos5 = _float3(0.f, 0.4875f, 0.f);
	m_pTransformCom5->Set_State(CTransform::STATE_POSITION, m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos5* Scale);
}

HRESULT CRectHouse::RectHouse_Render(void)
{
	if (FAILED(m_pTransformCom1->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom1->Bind_OnGraphicDev(0)))
		return E_FAIL;
	m_pVIBuffer1->Render();

	if (FAILED(m_pTransformCom2->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer2->Render();

	if (FAILED(m_pTransformCom3->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom3->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer3->Render();

	if (FAILED(m_pTransformCom4->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom3->Bind_OnGraphicDev(2)))
		return E_FAIL;
	m_pVIBuffer4->Render();

	if (FAILED(m_pTransformCom5->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom4->Bind_OnGraphicDev(3)))
		return E_FAIL;
	m_pVIBuffer5->Render();
}

CRectHouse * CRectHouse::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CRectHouse* pInstance = new CRectHouse(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CRectHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRectHouse::Clone(void * pArg)
{
	CRectHouse* pInstance = new CRectHouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CRectHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CRectHouse::Get_World(void)
{
	return _float4x4();
}

void CRectHouse::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom1);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBuffer1);
	Safe_Release(m_pTextureCom1);

	Safe_Release(m_pTransformCom2);
	Safe_Release(m_pVIBuffer2);
	Safe_Release(m_pTextureCom2);

	Safe_Release(m_pTransformCom3);
	Safe_Release(m_pVIBuffer3);
	Safe_Release(m_pTextureCom3);

	Safe_Release(m_pTransformCom4);
	Safe_Release(m_pVIBuffer4);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTransformCom5);
	Safe_Release(m_pVIBuffer5);

	Safe_Release(m_pTransformCom6);
	Safe_Release(m_pVIBuffer6);
}
