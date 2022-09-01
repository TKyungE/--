#include "stdafx.h"
#include "..\Public\House.h"
#include "GameInstance.h"

CHouse::CHouse(LPDIRECT3DDEVICE9 _pGraphic_Device)
	: CGameObject(_pGraphic_Device)
{
}

CHouse::CHouse(const CHouse& rhs)
	: CGameObject(rhs)
{
}

HRESULT CHouse::Initialize_Prototype(void)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHouse::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	memcpy(&m_IndexPos, pArg, sizeof(INDEXPOS));
	
	Set_vPos();

	return S_OK;
}

void CHouse::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	OnTerrain();

}

void CHouse::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);



	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHABLEND, this);
}

HRESULT CHouse::Render(void)
{
	if (FAILED(__super::Render()))
		return E_FAIL;
	Off_SamplerState();
	

	if (FAILED(SetUp_RenderState()))
		return E_FAIL;

	House_Render();
	
	if (FAILED(Release_RenderState()))
		return E_FAIL;
	On_SamplerState();
	return S_OK;
}

HRESULT CHouse::SetUp_Components(void)
{
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBuffer2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBufferCube1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCube)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_VIBufferCube2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"), (CComponent**)&m_pVIBufferCube2)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_House_Body"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_House_Head"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform2"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom2, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform3"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom3, &TransformDesc)))
		return E_FAIL;
	if (FAILED(__super::Add_Components(TEXT("Com_Transform4"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom4, &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CHouse::SetUp_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	return S_OK;
}

HRESULT CHouse::Release_RenderState(void)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}
HRESULT CHouse::On_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CHouse::Off_SamplerState()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	return S_OK;
}
void CHouse::OnTerrain(void)
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

	vPosition.y = pVIBuffer_Terrain->Compute_Height(vPosition, pTransform_Terrain->Get_WorldMatrix(), 1.f);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	Safe_Release(pGameInstance);
}

void CHouse::OnBillBoard(void)
{
	_float4x4	ViewMatrix;
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, *(_float3*)&ViewMatrix.m[0][0]);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, *(_float3*)&ViewMatrix.m[2][0]);

}

void CHouse::Set_vPos()
{
	m_pTransformCom->Set_Scaled({ 2.f,2.f,2.f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_IndexPos.vPos);
	_float3 vUp = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vUp.y += 3.f;
	m_pTransformCom2->Set_Scaled({ 2.f,2.f,2.f });
	m_pTransformCom2->Set_State(CTransform::STATE_POSITION, vUp);
	_float3 vUpTrun = { 0.f,1.f,0.f };
	_float3 vLookTrun = { 0.f,0.f,1.f };
	_float4x4 vOrginPos = m_pTransformCom->Get_WorldMatrix();
	_float3 vPos = *(_float3*)&vOrginPos.m[3][0];
	vPos.x += 0.45f;
	vPos.y += 2.9f;
	m_pTransformCom3->Set_Scaled({ 2.f,2.3f,2.f });
	m_pTransformCom3->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom3->Turn(vUpTrun, 1.f);
	m_pTransformCom3->Turn(vLookTrun, 0.3f);
	vPos.x -= 0.9f;
	m_pTransformCom4->Set_Scaled({ 2.f,2.3f,2.f });
	m_pTransformCom4->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTransformCom4->Turn(vUpTrun, -1.f);
	m_pTransformCom4->Turn(vLookTrun, -0.30f);
}

HRESULT CHouse::House_Render()
{
	if (FAILED(m_pTransformCom->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(0)))
		return E_FAIL;
	m_pVIBufferCube->Render();
	if (FAILED(m_pTransformCom2->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_OnGraphicDev(1)))
		return E_FAIL;
	m_pVIBufferCube2->Render();
	if (FAILED(m_pTransformCom3->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(0)))
		return E_FAIL;
	m_pVIBuffer->Render();
	if (FAILED(m_pTransformCom4->Bind_OnGraphicDev()))
		return E_FAIL;
	if (FAILED(m_pTextureCom2->Bind_OnGraphicDev(0)))
		return E_FAIL;
	m_pVIBuffer2->Render();

}

CHouse * CHouse::Create(LPDIRECT3DDEVICE9 _pGraphic_Device)
{
	CHouse* pInstance = new CHouse(_pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CHouse::Clone(void * pArg)
{
	CHouse* pInstance = new CHouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

_float4x4 CHouse::Get_World(void)
{
	return m_pTransformCom->Get_WorldMatrix();;
}

void CHouse::Free(void)
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransformCom2);
	Safe_Release(m_pTransformCom3);
	Safe_Release(m_pTransformCom4);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pVIBuffer2);
	Safe_Release(m_pVIBufferCube);
	Safe_Release(m_pVIBufferCube2);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom2);
}
