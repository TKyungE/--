#include "stdafx.h"
#include "..\Public\House2.h"

#include "GameInstance.h"

CHouse2::CHouse2(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CHouse2::CHouse2(const CHouse2 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHouse2::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHouse2::Initialize(void * pArg)
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

void CHouse2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHouse2::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHouse2::Render(void)
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

HRESULT CHouse2::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBufferCube"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCube)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer1)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer3)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer3"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer4)))
		return E_FAIL;


	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseCube"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HouseRect"), (CComponent**)&m_pTextureCom2)))
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
	
	return S_OK;
}

HRESULT CHouse2::SetUp_RenderState(void)
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

HRESULT CHouse2::Release_RenderState(void)
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

void CHouse2::SetPos(void)
{
	_float3 vPos = _float3(1.f, 0.f, 1.f);

	_float Scale = 1.f;

	vPos.y += 0.5f;
	m_pTransformCom1->Set_Scaled(_float3(Scale, Scale, Scale));
	
	m_pTransformCom1->Set_State(CTransform::STATE_POSITION, vPos * Scale);
	

	_float3 vPos1 = _float3(0.f,1.f,-0.5f);
	m_pTransformCom2->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom2->Turn(_float3(1.f, 0.f, 0.f), 0.3f);
	vPos1.y -= 0.13f;
	vPos1.z += 0.35f;
	m_pTransformCom2->Set_State(CTransform::STATE_POSITION, (m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos1));


	_float3 vPos2 = _float3(0.f, 1.f, 0.5f);
	m_pTransformCom3->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom3->Turn(_float3(1.f, 0.f, 0.f), -0.3f);
	vPos2.y -= 0.13f;
	vPos2.z -= 0.35f;
	m_pTransformCom3->Set_State(CTransform::STATE_POSITION, (m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos2));


	_float3 vPos3 = _float3(0.5f, 1.f, 0.f);
	m_pTransformCom4->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom4->Turn(_float3(0.f, 1.f, 0.f),1);
	m_pTransformCom4->Turn(_float3(0.f, 0.f, 1.f), 0.5f);
	m_pTransformCom4->Set_State(CTransform::STATE_POSITION, (m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos3));


	_float3 vPos4 = _float3(-0.5f, 1.f, 0.f);
	m_pTransformCom5->Set_Scaled(_float3(Scale, Scale, Scale));
	m_pTransformCom5->Turn(_float3(0.f, 1.f, 0.f),1);
	m_pTransformCom5->Turn(_float3(0.f, 0.f, 1.f), -0.5f);
	m_pTransformCom5->Set_State(CTransform::STATE_POSITION, (m_pTransformCom1->Get_State(CTransform::STATE_POSITION) + vPos4));
}

HRESULT CHouse2::RectHouse_Render(void)
{
	if (FAILED(m_pTransformCom1->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom1->Bind_OnGraphicDev(0)))
		return E_FAIL;
	m_pVIBufferCube->Render();



	if (FAILED(m_pTransformCom2->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer1->Render();

	

	if (FAILED(m_pTransformCom3->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer2->Render();


	if (FAILED(m_pTransformCom4->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer3->Render();


	if (FAILED(m_pTransformCom5->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBuffer4->Render();
}

CHouse2 * CHouse2::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CHouse2* pInstance = new CHouse2(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHouse2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHouse2::Clone(void * pArg)
{
	CHouse2* pInstance = new CHouse2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHouse2"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CHouse2::Get_World(void)
{
	return _float4x4();
}

void CHouse2::Free(void)
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


	Safe_Release(m_pTransformCom4);
	Safe_Release(m_pVIBuffer4);
	
	Safe_Release(m_pTransformCom5);

	Safe_Release(m_pVIBufferCube);
	
}
